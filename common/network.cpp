/*
 * Copyright distributed.net 1997-2000 - All Rights Reserved
 * For use in distributed.net projects only.
 * Any other distribution or use of this source violates copyright.
 *
*/
const char *network_cpp(void) {
return "@(#)$Id: network.cpp,v 1.121 2000/07/05 21:09:17 mfeiri Exp $"; }

//----------------------------------------------------------------------

#define TRACE
#define DEBUGTHIS

#include "cputypes.h"
#include "baseincs.h"  // standard stuff
#include "sleepdef.h"  // Fix sleep()/usleep() macros there! <--
#include "autobuff.h"  // Autobuffer class
#include "logstuff.h"  // LogScreen()
#include "base64.h"    // base64_encode()
#include "clitime.h"   // CliGetTimeString(NULL,1);
#include "triggers.h"  // CheckExitRequestTrigger()
#include "util.h"      // trace
#include "netres.h"    // NetResolve()
#include "network.h"   // thats us
#include "netio.h"


#if (CLIENT_OS == OS_QNX) || !defined(offsetof)
  #undef offsetof
  #define offsetof(__typ,__id) ((size_t)&(((__typ*)0)->__id))
#endif

extern int NetCheckIsOK(void); // used before doing i/o

//----------------------------------------------------------------------

#define VERBOSE_OPEN //print cause of ::Open() errors
#define UU_DEC(Ch) (char) (((Ch) - ' ') & 077)
#define UU_ENC(Ch) (char) (((Ch) & 077) != 0 ? ((Ch) & 077) + 0x20 : '`')

//----------------------------------------------------------------------

#ifndef MIPSpro
# pragma pack(1)               // no padding allowed
#endif /* ! MIPSpro */

// SOCKS4 protocol spec:  http://www.socks.nec.com/protocol/socks4.protocol

typedef struct _socks4 {
  unsigned char VN;           // version == 4
  unsigned char CD;           // command code, CONNECT == 1
  u16 DSTPORT;                // destination port, network order
  u32 DSTIP;                  // destination IP, network order
  char USERID[1];             // variable size, null terminated
} SOCKS4;

//----------------------------------------------------------------------

// SOCKS5 protocol RFC:  http://www.socks.nec.com/rfc/rfc1928.txt
// SOCKS5 authentication RFC:  http://www.socks.nec.com/rfc/rfc1929.txt

typedef struct _socks5methodreq {
  unsigned char ver;          // version == 5
  unsigned char nMethods;     // number of allowable methods following
  unsigned char Methods[2];   // this program will request at most two
} SOCKS5METHODREQ;

//----------------------------------------------------------------------

typedef struct _socks5methodreply {
  unsigned char ver;          // version == 1
  unsigned char Method;       // server chose method ...
  char end;
} SOCKS5METHODREPLY;

//----------------------------------------------------------------------

typedef struct _socks5userpwreply {
  unsigned char ver;          // version == 1
  unsigned char status;       // 0 == success
  char end;
} SOCKS5USERPWREPLY;

//----------------------------------------------------------------------

typedef struct _socks5 {
  unsigned char ver;          // version == 5
  unsigned char cmdORrep;     // cmd: 1 == connect, rep: 0 == success
  unsigned char rsv;          // reserved, must be 0
  unsigned char atyp;         // address type (IPv4 == 1, fqdn == 3)
  u32 addr;                   // network order
  u16 port;                   // network order
  char end;
} SOCKS5;

#ifndef MIPSpro
# pragma pack()
#endif /* ! MIPSpro */

//----------------------------------------------------------------------

const char *Socks5ErrorText[9] =
{
 /* 0 */ "" /* success */,
  "general SOCKS server failure",
  "connection not allowed by ruleset",
  "Network unreachable",
  "Host unreachable",
  "Connection refused",
  "TTL expired",
  "Command not supported",
  "Address type not supported"
};

//======================================================================

#ifdef DEBUGTHIS
// Logs a hexadecimal dump of the specified raw buffer of data.
static void __print_packet( const char *label,
    const char *apacket, unsigned int alen )
{
  unsigned int i;
  for (i = 0; i < alen; i += 16)
  {
    char buffer[128];
    char *p, *q; unsigned int n;
    sprintf(buffer,"%s %04x: ", label, i );
    q = 48 + (p = &buffer[strlen(buffer)]);
    for (n = 0; n < 16; n++)
    {
      unsigned int c = ' ';
      p[0] = p[1] = ' ';
      if (( n + i ) < alen )
      {
        static const char *tox="0123456789abcdef";
        c = (((unsigned int)apacket[n+i]) & 0xff);
        p[0] = (char)tox[c>>4];
        p[1] = (char)tox[c&0x0f];
        if (!isprint(c) || /*isctrl(c)*/ c=='\r' || c=='\n' || c=='\t')
          c = '.';
      }
      p += 2;
      *p++ = ' ';
      *q++ = (char)c;
    }
    *q = '\0';
    LogRaw("\n%s\n",buffer);
  }
  LogRaw("%s total len: %d\n",label, alen);
}
#endif


// Performs a safe string copy of a given network hostname into a fixed-size
// target buffer.  The result is guaranteed to be null terminated, and
// stripped of leading whitespace.  The hostname is additionally forced
// into lowercase letters during copying.  Copying is terminated at a null,
// a whitespace character, or length limit of the destination buffer.
static void __hostnamecpy( char *dest,
    const char *source, unsigned int maxlen)
{
  unsigned int len = 0;
  while (*source && isspace(*source))
    source++;
  while (((++len) < maxlen) && *source && !isspace(*source))
    *dest++ = (char)tolower(*source++);
  *dest = 0;
  return;
}


// Performs a number of cleanup operations on a specified hostname and
// analysis operations to determine some implied network connectivity
// settings.  Returns non-zero if the "autofind" mode for identifying
// the target server.
static int __fixup_dnethostname( char *host, int *portP, int mode,
                                  int *nofallback )
{
  static char *dnet = ".distributed.net";
  bool faultport = false, autofind = false;

  char *p = host;
  int len = 0;

  while (*p && isspace(*p))
    p++;
  while (*p && !isspace(*p))
    host[len++] = (char)tolower(*p++);
  host[len]=0;

  if (len==0 || strcmp(host,"auto")==0 || strcmp(host,"(auto)")==0 ||
                strcmp( host, dnet ) == 0 || strcmp( host, dnet+1 ) == 0 )
    faultport = autofind = 1;
  else if ( len < 16 )
    ; //not distributed.net, do nothing
  else if ( strcmp( &host[len-16], dnet ) != 0 )
    ; //not distributed.net, do nothing
  else if ( len == 21 && memcmp( host, "n0cgi.", 6 ) == 0 )
    *nofallback |= (mode & MODE_HTTP); //for (win32gui) newer-client check
  else if ( len > 22 && memcmp( &host[len-22], ".proxy.", 7 ) == 0 )
    *nofallback |= (mode & MODE_HTTP); // direct connect or redir service
  else if ( len < 20 || memcmp( &host[len-20], ".v27.", 5 ) != 0 )
    faultport = autofind = true; // rc5proxy.distributed.net et al
  else
  {
    int i; const char *dzones[]={"us","euro","asia","aussie","jp"};
    autofind = true; /* assume this until we know better */
    for (i=0;(autofind && i<((int)(sizeof(dzones)/sizeof(dzones[0]))));i++)
    {
      int len2 = strlen(dzones[i]);
      if ( memcmp( dzones[i], host, len2 )==0)
      {
        int foundport = ((host[len2] == '.') ? (2064) : (atoi(&host[len2])));
        if (foundport != 80 && foundport != 23 && foundport != 2064)
          break;
        else if (*portP == 0) //note: the hostname determines port
          *portP = foundport; //not viceversa.
        else if (*portP != 3064 && (*portP != foundport))
          break;
        autofind = false; /* everything checks out */
      }
    }
  }
  if (autofind)
  {
    host[0] = '\0';
    if (faultport && *portP == 0 && (mode & MODE_HTTP) != 0 )
      *portP = 80;
  }
  return autofind;
}


//======================================================================

// Initializes a new network object with the specified connectivity
// options.

Network::Network( const char * servname, int servport, bool _nofallback,
                  int _iotimeout, int _enctype, const char *_fwallhost,
                  int _fwallport, const char *_fwalluid)
{
  // check that the packet structures have been correctly packed
  size_t dummy;
  if (((dummy = offsetof(SOCKS4, USERID[0])) != 8) ||
     ((dummy = offsetof(SOCKS5METHODREQ, Methods[0])) != 2) ||
     ((dummy = offsetof(SOCKS5METHODREPLY, end)) != 2) ||
     ((dummy = offsetof(SOCKS5USERPWREPLY, end)) != 2) ||
     ((dummy = offsetof(SOCKS5, end)) != 10))
    LogScreen("Network::Socks Incorrectly packed structures.\n");

  // intialize communication parameters.
  server_name[0] = '\0';
  if (servname != NULL)
    __hostnamecpy( server_name, servname, sizeof(server_name));
  server_port = servport;

  // Reset the general connectivity flags.
  reconnected = false;
  shown_connection = false;
  nofallback = _nofallback;
  sock = INVALID_SOCKET;
  iotimeout = _iotimeout; /* if iotimeout is <0, use blocking calls */
  isnonblocking = false;      /* used later */

  // Reset the encoding state flags.
  gotuubegin = gothttpend = false;
  httplength = 0;

  // Reset firewall and other hostname buffers.
  fwall_hostaddr = svc_hostaddr = conn_hostaddr = 0;
  fwall_hostname[0] = fwall_userpass[0] = '\0';
  resolve_hostname[0] = '\0';
  resolve_addrcount = -1; /* uninitialized */

#ifndef HTTPUUE_WORKS
  // BUGBUG: the HTTP+UUE combination apparently does not work,
  // so we just silently force the use of HTTP only as appropriate.
  if (_enctype == 3 ) /*http+uue*/
    _enctype = 2; /* http only */
#endif

  // Store and initialize the modes depending on the connection type.
  mode = startmode = 0;
  if (_enctype == 1 /*uue*/ || _enctype == 3 /*http+uue*/)
  {
    iotimeout = -1;
    startmode |= MODE_UUE;
  }
  if (_enctype == 2 /*http*/ || _enctype == 3 /*http+uue*/)
  {
    iotimeout = -1;
    startmode |= MODE_HTTP;
    if (_fwallhost != NULL && _fwallhost[0])
    {
      fwall_hostport = _fwallport;
      if (_fwalluid != NULL)
        strncpy( fwall_userpass, _fwalluid, sizeof(fwall_userpass));
      __hostnamecpy( fwall_hostname, _fwallhost, sizeof(fwall_hostname));
    }
  }
  else if (_enctype == 4 /*socks4*/ || _enctype == 5 /*socks5*/)
  {
    if (_fwallhost != NULL && _fwallhost[0])
    {
      startmode |= ((_enctype == 4)?(MODE_SOCKS4):(MODE_SOCKS5));
      fwall_hostport = _fwallport;
      __hostnamecpy(fwall_hostname, _fwallhost, sizeof(fwall_hostname));
      if (_fwalluid)
        strncpy(fwall_userpass, _fwalluid, sizeof(fwall_userpass));
      if (fwall_hostport == 0)
        fwall_hostport = 1080;
    }
  }
  mode = startmode;

  // Do final validation and sanity checks on the hostname, port,
  // and connection mode flags, possibly adjusting them if needed.
  autofindkeyserver = __fixup_dnethostname(server_name, &server_port,
                                           startmode, &nofallback);

  // Set and validate the connection timeout value.
#if (CLIENT_OS == OS_RISCOS)
  iotimeout = -1;     // always blocking please
#else
  if (iotimeout < 0)
    iotimeout = -1;           // blocking mode
  else if (iotimeout < 5)
    iotimeout = 5;            // 5 seconds minimum.
  else if (iotimeout > 300)
    iotimeout = 300;          // 5 minutes maximum.
#endif

  // Adjust the verbosity level depending on the compilation mode.
#ifdef NETDEBUG
  verbose_level = 2;
#elif defined(VERBOSE_OPEN)
  verbose_level = 1;
#else
  verbose_level = 0; //quiet
#endif
}

//----------------------------------------------------------------------

Network::~Network(void)
{
  Close();
}

//----------------------------------------------------------------------

// returns -1 on error, 0 on success
int Network::Reset(u32 thataddress)
{
  reconnected = 1;
  svc_hostaddr = thataddress;
  return Open();
}

/* ---------------------------------------------------------------------- */

// Displays the current hostname (and possibly firewall/proxy being used)
// that we are connecting to.  This will only have an effect if the verbosity
// level is high enough.  Additionally, only the first call to this function
// will have an effect; subsequent calls during the same connection will
// produce no log output.

void Network::ShowConnection(void)
{
  if (verbose_level > 0 && !shown_connection)
  {
    unsigned int len = strlen( svc_hostname );
    char sig[]=".distributed.net";
    char scratch[sizeof(sig) + 20];
    char *targethost = svc_hostname;

    // change the targethost to lowercase.
    while (*targethost)
    {
      *targethost = (char)tolower(*targethost);
      targethost++;
    }
    targethost = svc_hostname;

    // if connecting to a hostname that ends in distributed.net, then
    // change the beautified display name to "distributed.net xx.yy.zz.ww"
    if ((len > (sizeof( sig ) - 1) && autofindkeyserver &&
      strcmp( &targethost[(len-(sizeof( sig )-1))], sig ) == 0))
    {
      targethost = sig + 1;
      if (svc_hostaddr != 0)
      {
        sprintf(scratch, "%s %s", sig+1, netio_ntoa(svc_hostaddr));
        targethost = scratch;
      }
    }

    // finally display the connection string.
    if ((startmode & MODE_PROXIED) == 0 || fwall_hostname[0] == '\0' /*http*/)
    {
      LogScreen("Connected to %s:%u...\n", targethost,
               ((unsigned int)(svc_hostport)) );
    }
    else
    {
      LogScreen( "Connected to %s:%u\nvia %s proxy %s:%u\n",
                 ( autofindkeyserver ? "distributed.net" : server_name ),
                 ((unsigned int)(svc_hostport)),
                 ((startmode & MODE_SOCKS5)?("SOCKS5"):
                 ((startmode & MODE_SOCKS4)?("SOCKS4"):("HTTP"))),
                 fwall_hostname, (unsigned int)fwall_hostport );
    }
  }
  shown_connection = true;
}

/* ----------------------------------------------------------------------- */

// returns -1 on error, 0 on success
int Network::Open( SOCKET insock )
{
  sock = insock;

  // set communications settings
  gethttpdone = puthttpdone = false;
  netbuffer.Clear();
  uubuffer.Clear();
  gotuubegin = gothttpend = false;
  httplength = 0;
  mode = startmode;

  return 0;
}

/* ----------------------------------------------------------------------- */

// Initiates a connection opening sequence, and performs the initial
// negotiation for SOCKS4/SOCKS5 connections.  Blocks until the operation
// completes or times out.
// returns -1 on error, 0 on success

int Network::Open( void )
{
  bool didfallback;
  int whichtry, maxtries;

  // set communications settings
  gethttpdone = puthttpdone = false;
  netbuffer.Clear();
  uubuffer.Clear();
  gotuubegin = gothttpend = false;
  httplength = 0;
  mode = startmode;

  didfallback = false; /* have we fallen back already? */
  maxtries = 3; /* two for preferred, one for fallback (if permitted) */

  // Ensure that if we are in SOCKS mode that we have a servername.
  if ((startmode & (MODE_SOCKS4 | MODE_SOCKS5)) != 0)
  {
    if (fwall_hostname[0] == '\0' || fwall_hostport == 0)
    {
      Log("Network::Invalid SOCKS proxy hostname or port.\n"
          "Connect cancelled.\n");
      return -1;
    }
  }

  for (whichtry = 0; whichtry < maxtries; whichtry++)
  {
    bool success = false;
    const char *netcheckfailed =
        "Network::Open Error - TCP/IP Connection Lost.\n";

    // If we failed before then pause for a little.
    if (whichtry > 0)
    {
      if (CheckExitRequestTriggerNoIO())
        break;
      LogScreen( "Network::Open Error - sleeping for 3 seconds\n" );
      sleep(1);
      if (CheckExitRequestTriggerNoIO())
        break;
      sleep(1);
      if (CheckExitRequestTriggerNoIO())
        break;
      sleep(1);
      if (CheckExitRequestTriggerNoIO())
        break;
    }

    if (!NetCheckIsOK()) /* connection broken */
    {
      LogScreen(netcheckfailed);
      break; /* return -1; */
    }

    /* ---------- create a new socket --------------- */

    if (sock != INVALID_SOCKET) {
      netio_close(sock);
    }

    success = (netio_createsocket(sock, false) == 0);
    isnonblocking = 0;

    if (!success)
    {
      if (verbose_level > 0)
      {
        #if (CLIENT_OS == OS_WIN32) || (CLIENT_OS == OS_WIN16)
        LogScreen("Network::failed to create network socket. (err=%d)\n",
                   WSAGetLastError());
        #else
        LogScreen("Network::failed to create network socket.\n");
        #endif
      }
      break; /* return -1; */
    }

    /* --- resolve the addresses(es) --- */

    if (success)
    {
      if (!reconnected || svc_hostaddr == 0)
      {
        if (!didfallback && !nofallback && whichtry == (maxtries-1) &&
           ((startmode & MODE_PROXIED) == 0 || fwall_hostname[0] == 0))
        {                                              /* last chance */

//BUGBUG: didn't __fixup_dnethostname already check for these?
          if (autofindkeyserver || server_name[0] == '\0' ||
              strstr(server_name,".distributed.net"))
          {
            /* can't fallback further than a fullserver */
          }
          else
          {
            resolve_addrcount = -1;
            didfallback = 1;
            server_name[0] = '\0';
            maxtries = 1;
            whichtry = 0;
          }
        }
        svc_hostname = server_name;
        svc_hostport = server_port;

        if (svc_hostname[0] == '\0')
        {
//BUGBUG: didn't __fixup_dnethostname already check for these?
          svc_hostaddr = 0;
          svc_hostname = "rc5proxy.distributed.net"; /* special name for resolve */
          if (svc_hostport != 80 && svc_hostport != 23 &&
            svc_hostport != 2064 && svc_hostport != 3064 /* &&
            svc_hostport != 21 && svc_hostport != 25 && svc_hostport != 110 */)
          {
            svc_hostport = 0;
          }
          autofindkeyserver = 1;
        }
        if (svc_hostport == 0)
        {
          svc_hostport = DEFAULT_PORT;
          if ((startmode & MODE_PROXIED) != 0 && fwall_hostname[0] != '\0')
          {
            if ((startmode & MODE_HTTP) != 0)
              svc_hostport = 80;
            else if ((startmode & MODE_UUE) != 0)
              svc_hostport = 23;
          }
        }
      }


      if (!NetCheckIsOK())
      {
        success = false;
        maxtries = 0;
        LogScreen(netcheckfailed);
      }
      else if ((startmode & MODE_PROXIED) != 0 && fwall_hostname[0] != '\0')
      {
        if ((startmode & MODE_HTTP) == 0)
        {
          // we always re-resolve unless http.
          // socks5 needs a 'good' hostname
          // (obtained from resolve_hostname)
          // if name resolution fails - cyp
          svc_hostaddr = 0;
        }
        if (NetResolve( fwall_hostname, fwall_hostport, 0,
                        &fwall_hostaddr, 1, NULL, 0 ) < 0)
        {
          success = false;
          fwall_hostaddr = 0;
          if (verbose_level > 0)
            LogScreen("Network::failed to resolve name \"%s\"\n",
                       fwall_hostname );

          // unrecoverable error. retry won't help
          maxtries = 0;
        }
        else if (svc_hostaddr == 0) /* always true unless http */
        {
          if (resolve_addrcount < 1)
          {
            resolve_addrcount =
               NetResolve( svc_hostname, svc_hostport,
                           autofindkeyserver,
                           &resolve_addrlist[0],
                           sizeof(resolve_addrlist)/sizeof(resolve_addrlist[0]),
                           resolve_hostname, sizeof(resolve_hostname) );
            if (resolve_addrcount > maxtries)
              maxtries = resolve_addrcount;
          }
          if (resolve_addrcount > 0) /* found */
          {
            svc_hostaddr = resolve_addrlist[whichtry % resolve_addrcount];
          }
          else /* not found */
          {
            svc_hostaddr = 0;
            if ((startmode & (MODE_SOCKS4 /*| MODE_SOCKS5*/)) != 0)
            {
              success = 0; // socks needs the address to resolve now.
              if (verbose_level > 0)
                LogScreen("Network::failed to resolve hostname \"%s\"\n",
                           svc_hostname);
            }
          }
          svc_hostname = resolve_hostname; //socks5 will use this
        }
        conn_hostaddr = fwall_hostaddr;
        conn_hostname = fwall_hostname;
        conn_hostport = fwall_hostport;
      }
      else /* not proxied */
      {
        if (resolve_addrcount < 1)
        {
          resolve_addrcount =
             NetResolve( svc_hostname, svc_hostport, autofindkeyserver,
                         &resolve_addrlist[0],
                         sizeof(resolve_addrlist)/sizeof(resolve_addrlist[0]),
                         resolve_hostname, sizeof(resolve_hostname) );
          if (resolve_addrcount > maxtries) /* found! */
            maxtries = resolve_addrcount;
//LogScreen("resolved: \"%s\" => %d addresses\n", resolve_hostname, resolve_addrcount);
        }
        if (resolve_addrcount > 0) /* found */
        {
          if (!reconnected || !svc_hostaddr)
          {
            svc_hostaddr = resolve_addrlist[whichtry % resolve_addrcount];
//LogScreen("Try %d: picked %s\n", whichtry+1, __inet_ntoa__(svc_hostaddr));
          }
        }
        else
        {
          success = false;
          svc_hostaddr = 0;
          if (verbose_level > 0)
            LogScreen("Network::failed to resolve name \"%s\"\n",
                      resolve_hostname );
        }
        svc_hostname = resolve_hostname;
        conn_hostaddr = svc_hostaddr;
        conn_hostname = svc_hostname;
        conn_hostport = svc_hostport;
      }
    }

    /* ------ connect ------- */

    isnonblocking = false;
    if (success)
    {
      #ifndef ENSURE_CONNECT_WITH_BLOCKING_SOCKET
      if (iotimeout > 0)
      {
        isnonblocking = ( netio_setsockopt( sock, CONDSOCK_BLOCKMODE, 0 ) == 0 );
        #ifdef DEBUGTHIS
        Log("Debug::Connecting with %sblocking socket.\n",
              ((isnonblocking) ? ("non-") : ("")) );
        #endif
      }
      #endif

      success = ( netio_lconnect( sock, conn_hostaddr, conn_hostport ) == 0 );

      #ifdef ENSURE_CONNECT_WITH_BLOCKING_SOCKET
      if (success && iotimeout > 0)
      {
        isnonblocking = ( netio_setsockopt( sock, CONDSOCK_BLOCKMODE, 0 ) == 0 );
        #ifdef DEBUGTHIS
        Log("Debug::Connected (%sblocking).\n",
            ((isnonblocking) ? ("non-") : ("")) );
        #endif
      }
      #endif

      if (iotimeout > 0 && CheckExitRequestTriggerNoIO())
      {
        success = false;
        maxtries = 0;
      }
      else if (!success)   /* connect failed */
      {
        if (verbose_level > 0)
        {
          const char *errreasonbuf = netio_describe_error();
          LogScreen( "Connect to host %s:%u failed.\n%s",
             netio_ntoa(conn_hostaddr), (unsigned int)(conn_hostport),
             errreasonbuf ? errreasonbuf : "unknown error" );
        }
      } //connect failed
    } // resolve succeeded

    /* ---- initialize the connection ---- */

    if (success)
    {
      int rc = InitializeConnection();
      if (rc != 0)
        success = false;
      if (rc < 0)           /* unrecoverable error (negotiation failure) */
        maxtries = 0; /* so don't retry */
    }

    /* ---- clean up ---- */

    if (success)
    {
      reconnected = true;
      return 0;
    }

    Close();
    svc_hostaddr = 0;

  }

  return -1;
}

// -----------------------------------------------------------------------

// Internal function used to negotiate the connection establishment
// sequence (only really does something significant for SOCKS4/SOCKS5
// connections).  Blocks until the establishment is complete.
// returns -1 on error, 0 on success

int Network::InitializeConnection(void)
{
  // set communications settings
  gotuubegin = gothttpend = puthttpdone = gethttpdone = false;
  httplength = 0;
  netbuffer.Clear();
  uubuffer.Clear();

  if (!NetCheckIsOK())
    return -1;
  if (sock == INVALID_SOCKET)
    return -1;

  if (startmode & MODE_HTTP)
  {
    netio_setsockopt( sock, CONDSOCK_KEEPALIVE, 1 );
  }
  else if (startmode & MODE_SOCKS5)
  {
    int success = 0; //assume failed
    int recoverable = 0; //assume non-recoverable error (negotiation failure)

    char socksreq[600];  // room for large username/pw (255 max each)
    SOCKS5METHODREQ *psocks5mreq = (SOCKS5METHODREQ *)socksreq;
    SOCKS5METHODREPLY *psocks5mreply = (SOCKS5METHODREPLY *)socksreq;
    SOCKS5USERPWREPLY *psocks5userpwreply = (SOCKS5USERPWREPLY *)socksreq;
    SOCKS5 *psocks5 = (SOCKS5 *)socksreq;
    u32 len;

    // transact a request to the SOCKS5 proxy requesting
    // authentication methods.  If the username/password
    // is provided we ask for no authentication or user/pw.
    // Otherwise we ask for no authentication only.

    psocks5mreq->ver = 5;
    psocks5mreq->nMethods = (unsigned char) (fwall_userpass[0] ? 2 : 1);
    psocks5mreq->Methods[0] = 0;  // no authentication
    psocks5mreq->Methods[1] = 2;  // username/password

    int authaccepted = 0;

    len = 2 + psocks5mreq->nMethods;
    if (netio_send(sock, socksreq, (int) len,
                   (isnonblocking ? iotimeout : 0),
                  CheckExitRequestTriggerNoIO) != (int) len)
    {
      if (verbose_level > 0)
        LogScreen("SOCKS5: error sending negotiation request\n");
      recoverable = 1;
    }
    else if (netio_recv(sock, socksreq, 2,
                        (isnonblocking ? iotimeout : 0),
                        CheckExitRequestTriggerNoIO) != 2)
    {
      if (verbose_level > 0)
        LogScreen("SOCKS5: failed to get negotiation request ack.\n");
      recoverable = 1;
    }
    else if (psocks5mreply->ver != 5)
    {
      if (verbose_level > 0)
        LogScreen("SOCKS5: authentication has wrong version, %d should be 5\n",
                            psocks5mreply->ver);
    }
    else if (psocks5mreply->Method == 2)  // username and pw
    {
      char username[255];
      char password[255];
      char *pchSrc, *pchDest;
      int userlen, pwlen;

      pchSrc = fwall_userpass;
      pchDest = username;
      while (*pchSrc && *pchSrc != ':')
        *pchDest++ = *pchSrc++;
      *pchDest = 0;
      userlen = pchDest - username;
      if (*pchSrc == ':')
        pchSrc++;
      strcpy(password, pchSrc);
      pwlen = strlen(password);

      //   username/password request looks like
      // +----+------+----------+------+----------+
      // |VER | ULEN |  UNAME   | PLEN |  PASSWD  |
      // +----+------+----------+------+----------+
      // | 1  |  1   | 1 to 255 |  1   | 1 to 255 |
      // +----+------+----------+------+----------+

      len = 0;
      socksreq[len++] = 1;    // username/pw subnegotiation version
      socksreq[len++] = (char) userlen;
      memcpy(socksreq + len, username, (int) userlen);
      len += userlen;
      socksreq[len++] = (char) pwlen;
      memcpy(socksreq + len, password, (int) pwlen);
      len += pwlen;

      if (netio_send(sock, socksreq, len,
                     (isnonblocking ? iotimeout : 0),
                     CheckExitRequestTriggerNoIO) != (int) len)
      {
        if (verbose_level > 0)
          LogScreen("SOCKS5: failed to send sub-negotiation request.\n");
        recoverable = 1;
      }
      else if (netio_recv(sock, socksreq, 2,
                          (isnonblocking ? iotimeout : 0),
                        CheckExitRequestTriggerNoIO) != 2)
      {
        if (verbose_level > 0)
          LogScreen("SOCKS5: failed to get sub-negotiation response.\n");
        recoverable = 1;
      }
      else if (psocks5userpwreply->ver != 1 ||
           psocks5userpwreply->status != 0)
      {
        if (verbose_level > 0)
          LogScreen("SOCKS5: user %s rejected by server.\n", username);
      }
      else
      {
        authaccepted = 1;
      }
    } //username and pw
    else if (psocks5mreply->Method == 1)  // GSSAPI
    {
      if (verbose_level > 0)
        LogScreen("SOCKS5: GSSAPI per-message authentication is\n"
                  "not supported. Please use SOCKS4 or HTTP.\n");
    }
    else if (psocks5mreply->Method == 0)       // no authentication required
    {
      // nothing to do for no authentication method
      authaccepted = 1;
    }
    else //if (psocks5mreply->Method > 2)
    {
      if (verbose_level > 0)
        LogScreen("SOCKS5 authentication method rejected.\n");
    }

    if (authaccepted)
    {
      // after subnegotiation, send connect request
      psocks5->ver = 5;
      psocks5->cmdORrep = 1;   // connnect
      psocks5->rsv = 0;   // must be zero
      psocks5->atyp = 1;  // IPv4 = 1
      psocks5->addr = svc_hostaddr;
      psocks5->port = (u16)htons((u16)svc_hostport); //(u16)(htons((server_name[0]!=0)?((u16)port):((u16)(DEFAULT_PORT))));
      int packetsize = 10;

      if (svc_hostaddr == 0)
      {
        psocks5->atyp = 3; //fully qualified domainname
        char *p = (char *)(&psocks5->addr);
        // at this point svc_hostname is a ptr to a resolve_hostname.
        strcpy( p+1, svc_hostname );
        *p = (char)(len = strlen( p+1 ));
        p += (++len);
        u16 xx = (u16)htons((u16)svc_hostport);
        *(p+0) = *(((char*)(&xx)) + 0);
        *(p+1) = *(((char*)(&xx)) + 1);
        packetsize = (10-sizeof(u32))+len;
      }

      if (netio_send(sock, socksreq, packetsize,
                        (isnonblocking ? iotimeout : 0),
                        CheckExitRequestTriggerNoIO) != packetsize)
      {
        if (verbose_level > 0)
          LogScreen("SOCKS5: failed to send connect request.\n");
        recoverable = 1;
      }
      else if (netio_recv(sock,  socksreq, packetsize,
                        (isnonblocking ? iotimeout : 0),
                        CheckExitRequestTriggerNoIO) < 10 /*ok for both atyps*/)
      {
        if (verbose_level > 0)
          LogScreen("SOCKS5: failed to get connect request ack.\n");
        recoverable = 1;
      }
      else if (psocks5->ver != 5)
      {
        if (verbose_level > 0)
           LogScreen("SOCKS5: reply has wrong version, %d should be 5\n",
                       psocks5->ver);
      }
      else if (psocks5->cmdORrep == 0)  // 0 is successful connect
      {
        success = 1;
        if (psocks5->atyp == 1)  // IPv4
          svc_hostaddr = psocks5->addr;
      }
      else if (verbose_level > 0)
      {
        const char *p = ((psocks5->cmdORrep >=
                         (sizeof Socks5ErrorText / sizeof Socks5ErrorText[0]))
                         ? ("") : (Socks5ErrorText[ psocks5->cmdORrep ]));
        LogScreen("SOCKS5: server error 0x%02x%s%s%s\n"
                  "connecting to %s:%u\n",
                 ((int)(psocks5->cmdORrep)),
                 ((*p) ? (" (") : ("")), p, ((*p) ? (")") : ("")),
                 svc_hostname, (unsigned int)svc_hostport );
      }
    } //if (authaccepted)

    return ((success) ? (0) : ((recoverable) ? (+1) : (-1)));
  } //if (startmode & MODE_SOCKS5)

  if (startmode & MODE_SOCKS4)
  {
    int success = 0; //assume failed
    int recoverable = 0; //assume non-recoverable error (negotiation failure)

    char socksreq[128];  // min sizeof(fwall_userpass) + sizeof(SOCKS4)
    SOCKS4 *psocks4 = (SOCKS4 *)socksreq;
    u32 len;

    // transact a request to the SOCKS4 proxy giving the
    // destination ip/port and username and process its reply.

    psocks4->VN = 4;
    psocks4->CD = 1;  // CONNECT
    psocks4->DSTPORT = (u16)htons((u16)svc_hostport); //(u16)htons((server_name[0]!=0)?((u16)port):((u16)DEFAULT_PORT));
    psocks4->DSTIP = svc_hostaddr;   //lasthttpaddress;
    strncpy(psocks4->USERID, fwall_userpass, sizeof(fwall_userpass));

    len = sizeof(*psocks4) - 1 + strlen(fwall_userpass) + 1;
    if (netio_send(sock, socksreq, len,
                  (isnonblocking ? iotimeout : 0),
                  CheckExitRequestTriggerNoIO) != (int) len)
    {
      if (verbose_level > 0)
        LogScreen("SOCKS4: Error sending connect request\n");
      recoverable = 1;
    }
    else
    {
      len = sizeof(*psocks4) - 1;  // - 1 for the USERID[1]
      int gotlen = netio_recv(sock, socksreq, len,
                          (isnonblocking ? iotimeout : 0),
                          CheckExitRequestTriggerNoIO);
      if (((u32)(gotlen)) != len )
      {
        if (verbose_level > 0)
          LogScreen("SOCKS4:%s response from server.\n",
                                     ((gotlen<=0)?("No"):("Invalid")));
        recoverable = 1;
      }
      else //if ( (u32)(gotlen)) == len)
      {
        if (psocks4->VN == 0 && psocks4->CD == 90) // 90 is successful return
        {
          success = 1;
        }
        else if (verbose_level > 0)
        {
          LogScreen("SOCKS4: request rejected%s.\n",
            (psocks4->CD == 91) ? ""
             : (psocks4->CD == 92) ? ", no identd response"
             : (psocks4->CD == 93) ? ", invalid identd response"
             : ", unexpected response");
        }
      }
    }

    return ((success) ? (0) : ((recoverable) ? (+1) : (-1)));
  }

  return 0;
}

// -----------------------------------------------------------------------

// Closes the connection and clears all flags and communication buffers.

int Network::Close(void)
{
  netio_close(sock);

  // set communications settings
  gethttpdone = puthttpdone = false;
  netbuffer.Clear();
  uubuffer.Clear();
  gotuubegin = gothttpend = false;
  httplength = 0;

  return 0;
}

// -----------------------------------------------------------------------

// Receives data from the connection with any necessary decoding.
// Returns length of read buffer.

int Network::Get( char * data, int length )
{
  time_t starttime = 0;
  bool need_close = false;
  bool timed_out = false;    //only used with blocking sockets
  char lcbuf[32]; unsigned int lcbufpos;

  bool tmp_isnonblocking = (isnonblocking != 0); //we handle timeout ourselves
  isnonblocking = false;                 //so stop LowLevelGet() from doing it.

  while (netbuffer.GetLength() < (u32)length)
  {
    bool nothing_done = true;

    if (iotimeout > 0 && CheckExitRequestTrigger())
      break;

    if (starttime == 0) /* first pass through */
      time(&starttime);
    else if (!tmp_isnonblocking) /* we are blocking, so no more chances */
      ; //keep going till socket close or timeout or we have data
    else if ((time(NULL) - starttime) > iotimeout)
      break;

    if ((mode & MODE_HTTP) && !gothttpend)
    {
      // []---------------------------------[]
      // |  Process HTTP headers on packets  |
      // []---------------------------------[]
      uubuffer.Reserve(500);
      int numRead = netio_recv(sock, uubuffer.GetTail(),
                               (int)uubuffer.GetTailSlack(),
                               (isnonblocking ? iotimeout : 0),
                              CheckExitRequestTriggerNoIO);
      if (numRead > 0) uubuffer.MarkUsed((u32)numRead);
      else if (numRead == 0) need_close = 1;       // connection closed
      else if (numRead < 0 && !tmp_isnonblocking) timed_out = 1;

      AutoBuffer line;
      while (uubuffer.RemoveLine(&line))
      {
        nothing_done = false;

        strncpy( lcbuf, line.GetHead(), sizeof(lcbuf) );
        lcbuf[sizeof(lcbuf)-1] = '\0';
        for (lcbufpos=0;lcbuf[lcbufpos] && lcbufpos<sizeof(lcbuf);lcbufpos++)
          lcbuf[lcbufpos] = (char)tolower(lcbuf[lcbufpos]);

        if (memcmp(lcbuf, "content-length: ", 16) == 0) //"Content-Length: "
        {
          httplength = atoi((const char*)line + 16);
        }
        else if ( (svc_hostaddr == 0) &&
          (memcmp(lcbuf, "x-keyserver: ", 13) == 0)) //"X-KeyServer: "
        {
          u32 newaddr = 0;
          char newhostname[64];
          if (NetResolve( (const char*)line + 13, svc_hostport, 0, &newaddr, 1,
                          newhostname, sizeof(newhostname) ) > 0)
          {
            resolve_addrlist[0] = svc_hostaddr = newaddr;
            resolve_addrcount = 1;
            strncpy( resolve_hostname, newhostname, sizeof(resolve_hostname));
            resolve_hostname[sizeof(resolve_hostname)-1]='\0';
            #ifdef DEBUGTHIS
            Log("Debug:X-Keyserver: %s\n", netio_ntoa(svc_hostaddr));
            #endif
          }
        }
        else if (line.GetLength() < 1)
        {
          // got blank line separating header from body
          gothttpend = true;
          if (uubuffer.GetLength() >= 6 &&
            memcmp(uubuffer.GetHead(), "begin ", 6) == 0)
          {
            mode |= MODE_UUE;
            gotuubegin = false;
          }
          if (!(mode & MODE_UUE))
          {
            if (httplength > uubuffer.GetLength())
            {
              netbuffer += uubuffer;
              httplength -= uubuffer.GetLength();
              uubuffer.Clear();
            } else {
              netbuffer += AutoBuffer(uubuffer, httplength);
              uubuffer.RemoveHead(httplength);
              gothttpend = false;
              httplength = 0;

              // our http only allows one packet per socket
              nothing_done = gethttpdone = true;
            }
          }
          break;
        }
      } // while
    }
    else if (mode & MODE_UUE)
    {
      // []----------------------------[]
      // |  Process UU Encoded packets  |
      // []----------------------------[]
      uubuffer.Reserve(500);
      int numRead = netio_recv(sock, uubuffer.GetTail(),
                               (int)uubuffer.GetTailSlack(),
                               (isnonblocking ? iotimeout : 0),
                              CheckExitRequestTriggerNoIO);
      if (numRead > 0) uubuffer.MarkUsed((u32)numRead);
      else if (numRead == 0) need_close = true;       // connection closed
      else if (numRead < 0 && !tmp_isnonblocking) timed_out = true;

      AutoBuffer line;
      while (uubuffer.RemoveLine(&line))
      {
        nothing_done = false;

        strncpy( lcbuf, line, sizeof(lcbuf) );
        lcbuf[sizeof(lcbuf)-1] = '\0';
        for (lcbufpos=0;lcbuf[lcbufpos] && lcbufpos<sizeof(lcbuf);lcbufpos++)
          lcbuf[lcbufpos] = (char)tolower(lcbuf[lcbufpos]);

        if (memcmp(lcbuf, "begin ", 6) == 0) gotuubegin = true;
        else if (memcmp(lcbuf, "post ", 5) == 0) mode |= MODE_HTTP; //"POST "
        else if (memcmp(lcbuf, "end", 3) == 0)
        {
          gotuubegin = gothttpend = false;
          httplength = 0;
          break;
        }
        else if (gotuubegin && line.GetLength() > 0)
        {
          // start decoding this single line
          char *p = line.GetHead();
          int n = UU_DEC(*p);
          for (++p; n > 0; p += 4, n -= 3)
          {
            char ch;
            if (n >= 3)
            {
              netbuffer.Reserve(3);
              ch = char((UU_DEC(p[0]) << 2) | (UU_DEC(p[1]) >> 4));
              netbuffer.GetTail()[0] = ch;
              ch = char((UU_DEC(p[1]) << 4) | (UU_DEC(p[2]) >> 2));
              netbuffer.GetTail()[1] = ch;
              ch = char((UU_DEC(p[2]) << 6) | UU_DEC(p[3]));
              netbuffer.GetTail()[2] = ch;
              netbuffer.MarkUsed(3);
            } else {
              netbuffer.Reserve(2);
              if (n >= 1) {
                ch = char((UU_DEC(p[0]) << 2) | (UU_DEC(p[1]) >> 4));
                netbuffer.GetTail()[0] = ch;
                netbuffer.MarkUsed(1);
              }
              if (n >= 2) {
                ch = char((UU_DEC(p[1]) << 4) | (UU_DEC(p[2]) >> 2));
                netbuffer.GetTail()[0] = ch;
                netbuffer.MarkUsed(1);
              }
            }
          }
        }
      } // while
    }
    else
    {
      // []--------------------------------------[]
      // |  Processing normal, unencoded packets  |
      // []--------------------------------------[]
      AutoBuffer tempbuffer;
      int wantedSize = ((mode & MODE_HTTP) && httplength) ? (int)httplength : 500;
      tempbuffer.Reserve((u32)wantedSize);

      int numRead = netio_recv(sock, tempbuffer.GetTail(),
                               wantedSize,
                               (isnonblocking ? iotimeout : 0),
                              CheckExitRequestTriggerNoIO);
      if (numRead < 0 && !tmp_isnonblocking)
        timed_out = true; // timed out
      else if (numRead == 0)
        need_close = true;
      else if (numRead > 0)
      {
        nothing_done = false;
        tempbuffer.MarkUsed(numRead);

        // decrement from total if processing from a http body
        if ((mode & MODE_HTTP) && httplength)
        {
          httplength -= numRead;

          // our http only allows one packet per socket
          if (httplength == 0) nothing_done = gethttpdone = true;
        }

        if ((lcbufpos = tempbuffer.GetLength()) > sizeof(lcbuf))
          lcbufpos = sizeof(lcbuf);
        strncpy( lcbuf, tempbuffer.GetHead(), lcbufpos );
        lcbuf[sizeof(lcbuf)-1] = '\0';
        for (lcbufpos = 0; lcbuf[lcbufpos] && lcbufpos<sizeof(lcbuf); lcbufpos++)
          lcbuf[lcbufpos] = (char)tolower(lcbuf[lcbufpos]);

        // see if we should upgrade to different mode
        if ( memcmp( lcbuf, "begin ",  6 ) == 0)
        {
          mode |= MODE_UUE;
          uubuffer = tempbuffer;
          gotuubegin = false;
        }
        else if ( memcmp( lcbuf, "post ",  5 ) == 0)
        {
          mode |= MODE_HTTP;
          uubuffer = tempbuffer;
          gothttpend = false;
          httplength = 0;
        }
        else netbuffer += tempbuffer;
      }
    }

    if (nothing_done)
    {
      if (need_close || gethttpdone || (timed_out && tmp_isnonblocking))
        break;
      #if (CLIENT_OS == OS_VMS) || (CLIENT_OS == OS_SOLARIS) || (CLIENT_OS == OS_ULTRIX)
        sleep(1); // full 1 second due to so many reported network problems.
      #else
        usleep( 100000 );  // Prevent racing on error (1/10 second)
      #endif
      if (iotimeout > 0 && CheckExitRequestTrigger())
        break;
    }
  } // while (netbuffer.GetLength() < blah)

  isnonblocking = (tmp_isnonblocking != 0); //restore the old state

  if (need_close)
    Close();

  // transfer back what was read in
  int bytesfilled = length;
  if (((u32)(netbuffer.GetLength())) < ((u32)(length)))
    bytesfilled = netbuffer.GetLength();
  if (bytesfilled != 0)
  {
    memmove(data, netbuffer.GetHead(), bytesfilled);
    netbuffer.RemoveHead((u32)bytesfilled);
    #ifdef DEBUGTHIS
    __print_packet("Get", data, bytesfilled );
    #endif
  }

  #ifdef DEBUGTHIS
  Log("Get: toread:%d read:%d\n", length, bytesfilled );
  #endif

  return bytesfilled;
}

//--------------------------------------------------------------------------

// Sends data over the connection, with any necessary encoding.
// returns bytes sent, -1 on error

int Network::Put( const char * data, int length )
{
  AutoBuffer outbuf;
  int requested_length = length;

  // if the connection is closed, try to reopen it once.
  if ((sock == INVALID_SOCKET) || puthttpdone)
  {
    if (svc_hostaddr == 0)
      return -1;
    if (Reset(svc_hostaddr) != 0) //Open(sock)
      return -1;
  }

  #if (CLIENT_OS != OS_390) /* can't do UUE with EBCDIC */
  if (mode & MODE_UUE)
  {
    /**************************/
    /***  Need to uuencode  ***/
    /**************************/
    outbuf += AutoBuffer("begin 644 query.txt\r\n");

    while (length > 0)
    {
      char line[80];
      char *b = line;

      int n = (int) (length > 45 ? 45 : length);
      length -= n;
      *b++ = UU_ENC(n);

      for (; n > 2; n -= 3, data += 3)
      {
        *b++ = UU_ENC((char)(data[0] >> 2));
        *b++ = UU_ENC((char)(((data[0] << 4) & 060) | ((data[1] >> 4) & 017)));
        *b++ = UU_ENC((char)(((data[1] << 2) & 074) | ((data[2] >> 6) & 03)));
        *b++ = UU_ENC((char)(data[2] & 077));
      }
      if (n != 0)
      {
        char c2 = (char)(n == 1 ? 0 : data[1]);
        char ch = (char)(data[0] >> 2);
        *b++ = UU_ENC(ch);
        *b++ = UU_ENC((char)(((data[0] << 4) & 060) | ((c2 >> 4) & 017)));
        *b++ = UU_ENC((char)((c2 << 2) & 074));
        *b++ = UU_ENC(0);
      }

      *b++ = '\r';
      *b++ = '\n';
      outbuf += AutoBuffer(line, b - line);
    }
    outbuf += AutoBuffer("end\r\n");
  }
  else
  #endif
  {
    outbuf = AutoBuffer(data, length);
  }

  if (mode & MODE_HTTP)
  {
    AutoBuffer header;
    char userpass[(((sizeof(fwall_userpass)+1)*4)/3)];

    userpass[0] = '\0';
    if (fwall_userpass[0])
    {
      if ( base64_encode( userpass, fwall_userpass,
                  sizeof(userpass), strlen(fwall_userpass)) < 0 )
        userpass[0] = '\0';
      userpass[sizeof(userpass)-1] = '\0';
    }

    header.Reserve(1024);
    header.MarkUsed(
            sprintf(header.GetHead(),
            "POST http://%s:%u/cgi-bin/rc5.cgi HTTP/1.0\r\n"
            "Proxy-Connection: Keep-Alive\r\n"
            "%s%s%s"
            "Content-Type: application/octet-stream\r\n"
            "Content-Length: %lu\r\n\r\n",
            ((!nofallback && svc_hostaddr)?
                  netio_ntoa(svc_hostaddr) : svc_hostname),
            ((unsigned int)(svc_hostport)),
            ((userpass[0])?("Proxy-authorization: Basic "):("")),
            ((userpass[0])?(userpass):("")),
            ((userpass[0])?("\r\n"):("")),
            (unsigned long) outbuf.GetLength()));
    #if (CLIENT_OS == OS_OS390)
      __etoa(header.GetHead());
    #endif
    header += outbuf;
    outbuf = header;

    puthttpdone = true;
  }

  #ifdef DEBUGTHIS
  __print_packet("Put", outbuf, outbuf.GetLength() );
  #endif

  int towrite = (int)outbuf.GetLength();
  int written = netio_send(sock, outbuf, towrite,
                           (isnonblocking ? iotimeout : 0),
                            CheckExitRequestTriggerNoIO);

  #ifdef DEBUGTHIS
  Log("Put: towrite:%d written:%d success:%d\n",
      towrite, written, (towrite == written) );
  #endif

  return ((towrite == written) ? (requested_length) : (-1));
}

//------------------------------------------------------------------------


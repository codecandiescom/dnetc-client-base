/* 
 * Copyright distributed.net 1997-1999 - All Rights Reserved
 * For use in distributed.net projects only.
 * Any other distribution or use of this source violates copyright.
*/
const char *selftest_cpp(void) {
return "@(#)$Id: selftest.cpp,v 1.61 1999/12/31 20:29:36 cyp Exp $"; }

#include "cputypes.h"
#include "client.h"    // CONTEST_COUNT
#include "baseincs.h"  // standard #includes
#include "problem.h"   // Problem class
#include "triggers.h"  // CheckExitRequestTriggerNoIO()
#include "logstuff.h"  // LogScreen()
#include "cpucheck.h"  // GetNumberOfDetectedProcessors() [for RISCOS]
#include "clicdata.h"  // CliGetContestNameFromID() 
#include "clievent.h"  // ClientEventSyncPost()
#include "util.h"      // ogr_stubstr()

// --------------------------------------------------------------------------

#define TEST_CASE_COUNT 32
#define TEST_CASE_DATA  10

// note this is in .lo, .hi, .lo... order...

// RC5-32/12/8 test cases -- generated by rc5/gentests64.cpp:
static const u32 rc5_test_cases[TEST_CASE_COUNT][TEST_CASE_DATA] = { // seed = 691713
  {0x9CC718F9,0x82E51B9F,0xF839A5D9,0xC41F78C1,0x20656854,0x6E6B6E75,0xB74BE041,0x496DEF29},
  {0xC3A303DB,0x7602EDDB,0xCD51A16E,0xFF716C97,0x8E7D42C5,0x5B227595,0x43848FB4,0x128FA11D},
  {0x8A00EAE3,0x59DA3369,0x123CE8CB,0xBBA53837,0xFDF318DE,0x7CEE3A16,0x0A6A82A6,0x6B1078F6},
  {0x0000348F,0xA2098FD6,0x8F31AAC0,0x4BC9E293,0x8BE8DD8C,0xDCB84729,0x5522BD19,0xB8F0949C},
  {0x0000E77F,0x28A00B00,0x9EA7C076,0xD217C125,0xB8FF4A1E,0x75CD7B7B,0x7231EBCA,0xEAB2CC92},
  {0x000076B5,0x47FC0000,0x25FC8FF2,0x9CEE4653,0x4F47BDC3,0xC2BF5981,0xAE46D122,0x3935B960},
  {0x0000ECBB,0xAE000000,0xA369A703,0x21EE64EB,0x2DF84521,0x8C527F60,0x76D25053,0xBA546EB3},
  {0xDD46A410,0xE5DC48A8,0x36F59F5F,0xD03C6630,0xF930EE83,0xEA4B8519,0x8778048C,0x19347BD5},
  {0xCF1275DB,0xB7641FBB,0xB292FD2C,0x7890908E,0x79176B39,0x1DCE14B1,0x28AD98AD,0xF769DDF6},
  {0x6B560E85,0x8C3630C1,0xF011C9CB,0x8E02B0CA,0xA530AA90,0x39E08B4B,0xECFAADFB,0xA3BD0596},
  {0xAB57A618,0xD1402487,0xBBE93EE4,0x382510D9,0xF6C54560,0xAF0F1E91,0x3BFE74D2,0xC759A7E4},
  {0x5D38883E,0xCD04EA43,0x100CFC11,0x01153176,0x1417ECDB,0xE59C33AD,0xA960BF31,0xA67463CA},
  {0x9A938F24,0x4DA86E02,0xD37181BB,0xDF64F671,0xD7A89A56,0xEC4326DA,0x85E09477,0x317A8DCA},
  {0xE9C9780D,0x30A0B992,0x483DA700,0x1BA00000,0x7A96BDFF,0x22ED4B59,0x87B9D046,0x59D99663},
  {0x9C7BFED1,0x281FA835,0xC2FAAD9B,0x54BAEB35,0x8D9142F2,0xB1C62C4E,0x1DDD520A,0x2C3F93AD},
  {0x50FEE97D,0x7BEF2A15,0xBBF8DF05,0xC6F4EF03,0x1EFC3F39,0x96A61CC3,0xA72DE1FB,0x3010282D},
  {0x0D691484,0xFFE95C09,0x3583BE61,0xC14A8027,0x0D424B0B,0x3F46B409,0x86841CD4,0x84DD5ECC},
  {0xA59BB6DF,0xF2D08D69,0x37D15B24,0x45F52C6D,0x2B9EA9ED,0xB5CDD924,0xEBFAD91F,0xFF880CE3},
  {0x721E3686,0xB5995544,0x183BA27F,0xF7188C9E,0xD01EFD58,0x50056139,0x4DF3675A,0x389E2865},
  {0x8610D36F,0x629D759B,0x10D10F75,0xC5125255,0x7CEDAEE3,0x0C6DEC4A,0x10CAE693,0xD32FC562},
  {0xF90892D9,0xBE186551,0x1597DC92,0xE9AB660E,0x9952BE9F,0xE4573AB2,0x9E7497DF,0xBB3049EF},
  {0xDBA2FC96,0xE88F2D6C,0xA97ED579,0x0C086E5C,0xD3081021,0x88494D74,0xFB8A2952,0x3EBF9030},
  {0x73769D18,0x32A9F8B3,0x454BD8AD,0xCC56201E,0x60100E1D,0x1C14045D,0x7717FC80,0x8D33A5F4},
  {0x0274C68F,0x2DF37A38,0x88BC211D,0x57115FE7,0x9A056963,0xE969501F,0xCA61A1BD,0xFFF6F434},
  {0x49E0B858,0xCF516BE9,0x6F272388,0x7AD96A09,0xB1B6D2AD,0x1771541B,0xA374F004,0xC1ED6FB3},
  {0x09D3CD1B,0x3BC646BB,0xB9567DC5,0xAA8B716A,0xEB548F0A,0xFE493050,0x2AB10A76,0x75314F04},
  {0x13AB3D93,0x459E9D6C,0xC7D84F59,0x70F7F017,0x40F19337,0x6EA94E5F,0xDFB56293,0x4EFF6EB7},
  {0xF432E5AC,0xFEB6F6E4,0xF6AB68E5,0xB19C5429,0x1AF4EE53,0x9207DD4D,0x1F1F8F3C,0xC420A949},
  {0xC504E8C3,0x5F293912,0xBF226E51,0xB87BB3C2,0x25B8A82B,0x1AFC97B7,0x0AFC8A26,0x3639E24A},
  {0x28644A3B,0x58E8C1BD,0xF594A3BA,0xA0A2F1AA,0x965B47DD,0xCC95CDB5,0x8E6C738D,0x744AF0E4},
  {0x18F5DDB6,0xF1F7A699,0x26A7CE09,0xC501FDA3,0x18044DC9,0xBD4A8B38,0xDD494282,0x181B2A90},
  {0x01F3D5B8,0x06CEBA70,0xFFBE0E64,0x7A32BA15,0x89C3CAFF,0x18CE275D,0x37D0DE81,0x4BF14187}
};

#ifdef HAVE_DES_CORES
#include "convdes.h"   // convert_key_from_des_to_inc 
// DES test cases -- key/iv/plain/cypher -- generated by des/gentestsdes.c
// DES test cases are now true DES keys, and can be checked with any DES package
static const u32 des_test_cases[TEST_CASE_COUNT][TEST_CASE_DATA] = {
  {0x54159B85,0x316B02CE,0xB401FC0E,0x737B34C9,0x96CB8518,0x4A6649F9,0x34A74BC8,0x47EE6EF2},
  {0x1AD9EA4A,0x15527592,0x805A8EF3,0x21F68C4C,0xC4B9980E,0xD5C3BD8A,0x5AFD5D57,0x335981AD},
  {0x5B199D6E,0x40C40E6D,0xF5756816,0x36088043,0x4EF1DF24,0x6BF462EC,0xC657515F,0xABB6EBB0},
  {0xAD8C0DEC,0x68385DF1,0x19FB0D4F,0x288D2CD6,0x03FA0F6F,0x038E92F8,0x2FA04E4C,0xBFAB830A},
  {0xC475C22C,0xDFE3B67A,0x5614A37E,0xD70F8E2D,0xCA620ACE,0xA1CF54BB,0xB5BF73A1,0xB2BB55BD},
  {0x2FABC40D,0xE03B8CE6,0xF825C0CF,0x47BDC4A9,0x639F0904,0x354EFC8B,0xC745E11C,0x698BF15F},
  {0x80940E61,0xDCBC7F73,0xA30685EA,0x67CDA3FE,0x6E538AA3,0xC34993BB,0xF6DBDCE9,0x6FCE1832},
  {0x4A701329,0x450D5D0B,0x93D406FA,0x96C9CD56,0xAF7D2E73,0xA1A9F844,0x9428CB49,0x1F93460F},
  {0x2A73B06E,0x8C855D6B,0x3FC6F9D5,0x3F07BC65,0x9A311C3B,0x8FC62B22,0x0E71ECD9,0x003B4F0B},
  {0x255DFBB0,0xB5290115,0xE4663D24,0x702B8D86,0xC082814F,0x6DFA89AC,0xB76E630D,0xF54F4D24},
  {0xBA1A3B6E,0x9158E3C4,0x4C3E8CBC,0xA19D4133,0x7F8072EC,0x6A19424E,0xE09F06DA,0x6508CD88},
  {0xFB32138A,0xF4F73175,0x87C55A28,0xC5FAA7A2,0xDAE86B44,0x629B5CAE,0xAEC607BC,0x9DD8816D},
  {0x5B0BDA4F,0x025B2502,0x1F6A43E5,0x0006E17E,0xB0E11412,0x64EB87EB,0x2AED8682,0x6A8BC154},
  {0xB05837B3,0xFBE083AE,0x3248AE33,0xD92DDA07,0xFAF4525F,0x4E90B16D,0x6725DBD5,0x746A4432},
  {0x76BC4570,0xBFB5941F,0x8F2A8068,0xCE638F26,0xA21EBDF0,0x585A6F8A,0x65A3766E,0x95B6663A},
  {0xC7610E85,0x5DDCBC51,0xB0747E7F,0x8A52D246,0x3825CE90,0xD70EA566,0x50BC63A5,0xDF9DD8FA},
  {0xB9B02615,0x017C3745,0x21BAECAC,0x4771B2AA,0x32703B09,0x0CBEF2BC,0x69907E24,0x0B3928A6},
  {0x0D7C8F0D,0xFDC2DF6E,0x3BBCE282,0x7C62A9D8,0x4E18FA5A,0x2D942C4E,0x5BF53685,0x23E40E20},
  {0xBAA426B6,0xAED92F13,0xC0DAC03C,0x3382923A,0x25F6F952,0x3C691477,0x49B7862A,0x6520E509},
  {0x7C37682A,0x164A43B3,0x9D31C0D1,0x884B1EE5,0x2DCBB169,0xB4530B74,0x3C93D6C3,0x9A9CE765},
  {0x79B55B8F,0x6B8AC2B5,0xE9545371,0x004E203E,0xA3170E57,0x9F71563D,0xF5DE356F,0xBD0191DF},
  {0xC8F80132,0xD532972F,0xBC2145BC,0x42E174FE,0xBA4DCA59,0x6F65FA58,0xB276ADD5,0xA0A9F7B1},
  {0x6E497043,0x7C402CC2,0x0039BB42,0xBD8438A2,0x508592BF,0x1A2F40D6,0x0F1EB5BC,0x6B0C42E7},
  {0xB3C4FD31,0xD619314A,0x39B2DBF7,0x0295F93A,0x4D547967,0x36149936,0x44B02FEE,0xEECC0B2D},
  {0x7FA12954,0x08737CA8,0x8ECDCE90,0x5DACCF36,0x7AA693B0,0x62C8CA9C,0x948CB25E,0xF4781028},
  {0x01BFDC08,0x7558CD0E,0x7D6D82DA,0x19ACD958,0x1EDF3781,0x195110A7,0x021EB315,0xE2EA34C9},
  {0x5161A2C4,0x4F043B43,0x17D76130,0xDCB7695C,0xA70ADBC0,0x843A8801,0xAEE16715,0xE1AF0F07},
  {0x943DF4E3,0xB6D6CEF2,0xC763AAA3,0xA0179248,0xEB61626F,0x1B130032,0x5630226F,0x1C9DBFB2},
  {0xE997049E,0x37D5E085,0x07C372A8,0x3669C801,0x689B4583,0xDA05F0A2,0xFA70DACD,0x3F031F6C},
  {0x4C2F1083,0x5D8A6B32,0xC38544FA,0x017883F5,0xD06D9EAA,0xEE0DFBF6,0xB1A728B7,0x12C311C4},
  {0x5225BCB0,0xE51C98B6,0x2B7ABF2D,0xD714717E,0xC867B0B7,0xF24322B6,0x0A6BF211,0xB0B7C1CA},
  {0xCE6823E9,0x16A8A476,0xCDC4DBA4,0xD93B6603,0xC6E231B9,0xD84C2204,0xDB623F7C,0x3477E4B2},
};
#endif

#ifdef HAVE_OGR_CORES
// OGR test cases
// [0] - expected number of nodes (~ if no solution expected to be found)
// [1] - number of marks
// [2..7] - first differences
static const s32 ogr_test_cases[TEST_CASE_COUNT][TEST_CASE_DATA] = {
  { 0x000D1B52, 21, 2, 22, 32, 21, 5, 1},
  {~0x0057102A, 21, 1, 2, 4, 5, 8, 10},
  {~0x00A8EE70, 21, 2, 22, 32, 21, 5, 2},
  {~0x007D1FD7, 22, 1, 2, 4, 5, 8, 10},
  { 0x0015ACEC, 22, 1, 8, 5, 29, 27, 36},
  {~0x001BE4B3, 22, 1, 8, 5, 29, 27, 37},
  {~0x00441720, 23, 1, 2, 4, 5, 8, 10},
  { 0x000EFA83, 23, 3, 4, 10, 44, 5, 25},
  {~0x0027C638, 23, 3, 4, 10, 44, 5, 26},
  {~0x01298F41, 24, 1, 2, 4, 5, 8, 10, 14},
  { 0x000ADD64, 24, 9, 24, 4, 1, 59, 25},
  {~0x003C3A07, 24, 9, 24, 4, 1, 59, 26},
  {~0x0057102A, 21, 1, 2, 4, 5, 8, 10},
  { 0x000D1B52, 21, 2, 22, 32, 21, 5, 1},
  {~0x00A8EE70, 21, 2, 22, 32, 21, 5, 2},
  {~0x007D1FD7, 22, 1, 2, 4, 5, 8, 10},
  { 0x0015ACEC, 22, 1, 8, 5, 29, 27, 36},
  {~0x001BE4B3, 22, 1, 8, 5, 29, 27, 37},
  {~0x00441720, 23, 1, 2, 4, 5, 8, 10},
  { 0x000EFA83, 23, 3, 4, 10, 44, 5, 25},
  {~0x0027C638, 23, 3, 4, 10, 44, 5, 26},
  {~0x01298F41, 24, 1, 2, 4, 5, 8, 10, 14},
  { 0x000ADD64, 24, 9, 24, 4, 1, 59, 25},
  {~0x003C3A07, 24, 9, 24, 4, 1, 59, 26},
  {~0x0057102A, 21, 1, 2, 4, 5, 8, 10},
  { 0x000D1B52, 21, 2, 22, 32, 21, 5, 1},
  {~0x00A8EE70, 21, 2, 22, 32, 21, 5, 2},
  {~0x007D1FD7, 22, 1, 2, 4, 5, 8, 10},
  { 0x0015ACEC, 22, 1, 8, 5, 29, 27, 36},
  {~0x001BE4B3, 22, 1, 8, 5, 29, 27, 37},
  {~0x00441720, 23, 1, 2, 4, 5, 8, 10},
  { 0x000EFA83, 23, 3, 4, 10, 44, 5, 25},
  //{0, 25, 12, 17, 10, 33, 19, 55},
  //{0, 26, 1, 32, 50, 21, 6, 14},
  //{0, 27, 3, 12, 26, 25, 29, 2},
  //{0, 28, 3, 12, 26, 25, 29, 2},
  //{0, 29, 3, 25, 5, 8, 54, 61},
};
#endif

#ifdef HAVE_CSC_CORES
#include "convcsc.h"   // convert_key_from_csc_to_inc 
// CSC test cases -- key/iv/plain/cypher -- generated by csc/gentestscsc.cpp
static const u32 csc_test_cases[TEST_CASE_COUNT][TEST_CASE_DATA] = {
  // seed = 0x3719b2a8 - Linux 2.2.5 / debian libc6 (glibc2) 2.0.7.19981211
  // the first two cases are the official 56-bit CSC test cases
  {0x785d9100,0x5f643ec4,0x57bd90ac,0x1324fe68,0x73656372,0x54686520,0x5cc42fb0,0x35328d4e},
  {0x467cb900,0x2153adf9,0x9922a644,0xf367b835,0x73656372,0x54686520,0x9ca6a3d7,0x7C9bb5e5},
//{0xd1f9d100,0xd3f02d18,0x1d8f15c3,0xe2dcd95a,0x94a8a709,0x9d030194,0xcd1427e2,0x6d0607a5},
//{0x66ca2300,0x9dd1054d,0x335e638f,0x9d58e586,0x828a35f2,0x2b13b551,0x51b3b9e0,0xb64fba21},
  {0xb74c9000,0xf70f3787,0xe74b5223,0xea7fc85a,0xf36d9d9d,0x7a62374a,0xa56a755d,0x1805f962},
  {0x05112500,0xd4d0ad33,0x02e64a93,0x08cbec20,0x66823930,0x94358f18,0x6a3a788d,0x20c62fe7},
  {0xb4532500,0x3d170088,0x9f5ba14e,0x2785fb80,0x90b52e73,0xd8f65a96,0xd73a3559,0xfb8258f9},
  {0xae1a1d00,0x0775d0ed,0xbc47f797,0x5742e905,0xc1fb17dd,0x4135e095,0xc2f2bfe2,0x7c6609f9},
  {0xabed5600,0x877c4ce2,0x3cfd1e21,0xf4ca7aff,0xbb341c92,0xf9cbe2e4,0xfb735dbd,0x59aa4329},
  {0x66140400,0xdf4f1d26,0x2aef190c,0x9b492bd3,0x259f0cd3,0xfd807d36,0x26cb59ee,0xd1a6dc5f},
  {0x2b5e6400,0xd40f40d2,0x195f146a,0x3e6a1968,0x16e99695,0x4db3647e,0x9d0cafa6,0x1b156d01},
  {0xcc22fc00,0x39c00928,0xb5eefa82,0xf619372d,0x84e01a6c,0xd80bc877,0xc2bfc88d,0x65fc869d},
  {0x1b800500,0x10856b70,0xa783209c,0xb98e92de,0x6ae96ffb,0x85c5e2b0,0xfccad0ec,0xe481b83a},
  {0xca77fe00,0xa4b07d92,0xf2b354b2,0x87b89ffd,0x24c2d463,0x4d83f738,0xf97de4ad,0xaa48fee4},
  {0x7abc3d00,0xbf0a9b42,0x92be1486,0x6093b8f1,0x057589f6,0x16eef30a,0x63bcb6d2,0x6b3ff92d},
  {0xbc9d4f00,0x38e849eb,0x267d830b,0x5c0ad3dc,0xeaca1988,0x6e599fc6,0xdf46c0c2,0xbd6c5a91},
  {0xa3943d00,0x881a8046,0x6d5a34e8,0xcc73e207,0x50600c20,0x2a25b810,0x3092c692,0x4294153f},
  {0x60362300,0x1e47b39b,0x4c7e490b,0xbeebe142,0x0b67048e,0x6b3d057f,0xb5dfe4b9,0x49254b35},
  {0x079f1500,0x9afb6bf4,0x97139bde,0xbda3bc7c,0x28ba9c93,0xf05c59d9,0xc875f249,0x2dbecba0},
  {0xe38fb200,0xa6df4624,0xb6484b8c,0x2959b533,0xa58f0f21,0xeeae1ef9,0xc32a81c9,0xf11b12df},
  {0x55e11a00,0x73b94687,0x3323c184,0x877adc5d,0xca0b511c,0x34f9ab09,0x274cd745,0x2e477da7},
  {0xe25ed000,0x66d313af,0x1c8d4825,0xd2b4df73,0x136cc796,0x6e895995,0x004f95df,0xb6a12d9c},
  {0x1af15200,0x12c93035,0xe22a40ae,0x58b107d7,0x76613bc0,0x9e250710,0xe7b35110,0xe8ecd4ee},
  {0xa4627600,0x53dc1ae0,0x43aa2821,0x74f95afb,0xf9204aba,0xc940a9eb,0x83c4d79a,0xd6aa3f0f},
  {0xd7c28800,0x9e9c82c8,0x75cb061a,0x159f096c,0xd2ac1fb6,0x8565f501,0x573a4353,0xba1460c8},
  {0x43636400,0xfff93baa,0xbffdc021,0x01e7d852,0x5eb7b5ff,0x6501c9aa,0x20ded75f,0x2189f14a},
  {0x93d31400,0x9274cd5d,0x4d9dbb68,0xd3e951e1,0xb6e21647,0xae0d7d4c,0xb063dbad,0x6a54c3bb},
  {0xbcdf1500,0x19071f16,0x50b441af,0x71aa1de3,0xcbf0cd00,0xdd8acf7d,0x03b2bd25,0x0be74b79},
  {0x644cdc00,0xf74d9195,0x3d2639ef,0x65ec9f91,0x7c1b0ca4,0xcce37ccb,0x32f013be,0x5fc57fe8},
  {0xc221b900,0x04f4dfb9,0xb4230dc2,0x18026b8a,0x376d30ef,0x7107ad4f,0xc2ae1538,0x28a715c1},
  {0x38165400,0x892cddb0,0x407eff77,0x1b060a8d,0x7b94edcd,0x2748d427,0x7dc65255,0x8cb16eca},
  {0xb2f24900,0x3781e55a,0x83dbd2e4,0x7b51ad7f,0xd4c85702,0x74752b68,0x33bad2b2,0x78fcfd06},
  {0x30789900,0x4e4279cc,0xfa9276a5,0xaf8ccc53,0x40c925fa,0xa71a0a7e,0x7d3b1b9f,0x2202ef04},
  {0xd43f2800,0x4b7d6d26,0x59cfd9b7,0x8d319bf2,0x420d634d,0xb86b59aa,0x7d3c1417,0x2e6344c1},
};
#endif

// ---------------------------------------------------------------------------

// returns 0 if not supported, <0 on failed or break
int SelfTest( unsigned int contest )
{
  int threadpos, threadcount = 1;
  int successes = 0;
  const char *contname;
  int userbreak = 0;

  if (CheckExitRequestTrigger())
    return 0;

  if (contest >= CONTEST_COUNT)
  {
    LogScreen("test::error. invalid contest %u\n", contest );
    return 0;
  }
  if (!IsProblemLoadPermitted(-1, contest)) /* also checks HAVE_xxx_CORES */
    return 0;

  contname = CliGetContestNameFromID( contest );
  for ( threadpos = 0; 
        !userbreak && successes >= 0 && threadpos < threadcount;
        threadpos++ )
  {
    char lastmsg[100];
    unsigned int testnum;

    ClientEventSyncPost( CLIEVENT_SELFTEST_STARTED, (long)contest );
    successes = 0;
    lastmsg[0] = '\0';

    for ( testnum = 0 ; !userbreak && testnum < TEST_CASE_COUNT ; testnum++ )
    {
      const u32 (*test_cases)[TEST_CASE_COUNT][TEST_CASE_DATA] = NULL;
      int resultcode; const char *resulttext = NULL;
      u32 expectedsolution_hi, expectedsolution_lo;
      ContestWork contestwork;
      Problem *problem = new Problem();

      u32 tslice = 0x1000;
      int non_preemptive_env = 0;

      #if (CLIENT_OS == OS_NETWARE)
      non_preemptive_env = (!nwCliIsPreemptiveEnv());
      if (non_preemptive_env)
        tslice = 2048;
      #elif (CLIENT_OS == OS_WIN16 || CLIENT_OS == OS_WIN32) /* win32s */
      non_preemptive_env = (winGetVersion() < 400);
      if (non_preemptive_env)
        tslice = 2048;
      #elif (CLIENT_OS == OS_RISCOS)
      non_preemptive_env = riscos_check_taskwindow();
      if (non_preemptive_env)
        tslice = 2048;
      #elif (CLIENT_OS == OS_MACOS)
      non_preemptive_env = 1;
      tslice = 2048;
      #endif

      if (contest == RC5)
      { 
        test_cases = (const u32 (*)[TEST_CASE_COUNT][TEST_CASE_DATA])rc5_test_cases;
        expectedsolution_lo = (*test_cases)[testnum][0];
        expectedsolution_hi = (*test_cases)[testnum][1];

        /*
        test case 1 is the RSA pseudo-contest solution
        test cases 2,3,4,5,6,7 are specially made to 
        stress the key incrementation system
        the other test cases are generic (random)
  
        if test case N fails, then key & K isn't properly 
        incremented when key & W wrap :
  
        N         K                 W
               (hi:lo)           (hi:lo)
  
        2 00000000:00FF0000 00000000:0000FFFF
        3 00000000:FF000000 00000000:00FF0000
        4 000000FF:00000000 00000000:FF000000
        5 0000FF00:00000000 000000FF:00000000
        6 00FF0000:00000000 0000FF00:00000000
        7 FF000000:00000000 00FF0000:00000000
  
        another way of explaining this algorithm :
                                             __
        2 the solution is :       7602EDDB:C3A303DB
          we're starting from :   7602EDDB:C3A20000
                                           __
        3 the solution is :       59DA3369:8A00EAE3
          we're starting from :   59DA3369:89FF0000
                                        __
        4 the solution is :       A2098FD6:0000348F
          we're starting from :   A2098FD5:FFFF0000
                                      __
        5 the solution is :       28A00B00:0000E77F
          we're starting from :   28A00AFF:FFFF0000
                                    __
        6 the solution is :       47FC0000:000076B5
          we're starting from :   47FBFFFF:FFFF0000
                                  __
        7 the solution is :       AE000000:0000ECBB
          we're starting from :   ADFFFFFF:FFFF0000
  
        remember, in cores the running key is reversed, ie:
        keybyte_inside_core[0] == keybyte_outside_core[7] == key.hi & 0xFF000000,
        keybyte_inside_core[1] == keybyte_outside_core[6] == key.hi & 0x00FF0000,
        keybyte_inside_core[2] == keybyte_outside_core[5], etc...
        */

        contestwork.crypto.key.lo = expectedsolution_lo & 0xFFFF0000L;
        contestwork.crypto.key.hi = expectedsolution_hi;
        if (testnum>1 && testnum<=6) 
        {
          contestwork.crypto.key.lo -= 0x00010000;
          if ((expectedsolution_lo & 0xFFFF0000L) == 0)
            contestwork.crypto.key.hi--;
        }
      }
#ifdef HAVE_DES_CORES
      if (contest == DES)
      {
        test_cases = (const u32 (*)[TEST_CASE_COUNT][TEST_CASE_DATA])des_test_cases;
        expectedsolution_lo = (*test_cases)[testnum][0];
        expectedsolution_hi = (*test_cases)[testnum][1];

        convert_key_from_des_to_inc ( (u32 *) &expectedsolution_hi, 
                                      (u32 *) &expectedsolution_lo);

        // to test also success on complementary keys
        if (expectedsolution_hi & 0x00800000L)
        {
          expectedsolution_hi ^= 0x00FFFFFFL;
          expectedsolution_lo = ~expectedsolution_lo;
        }
        contestwork.crypto.key.lo = expectedsolution_lo & 0xFFFF0000L;
        contestwork.crypto.key.hi = expectedsolution_hi;
      }
#endif      
#ifdef HAVE_OGR_CORES
      if (contest == OGR)
      {
        test_cases = (const u32 (*)[TEST_CASE_COUNT][TEST_CASE_DATA])ogr_test_cases;
        expectedsolution_lo = (*test_cases)[testnum][0];
      }
#endif
#ifdef HAVE_CSC_CORES
      if (contest == CSC) // CSC
      {
        test_cases = (const u32 (*)[TEST_CASE_COUNT][TEST_CASE_DATA])csc_test_cases;
        expectedsolution_lo = (*test_cases)[testnum][0];
        expectedsolution_hi = (*test_cases)[testnum][1];

        convert_key_from_csc_to_inc ( (u32 *) &expectedsolution_hi,
                                      (u32 *) &expectedsolution_lo);

        contestwork.crypto.key.lo = expectedsolution_lo & 0xFFFF0000L;
        contestwork.crypto.key.hi = expectedsolution_hi;
      }
#endif

      switch (contest) {
        case RC5:
        case DES:
        case CSC:
          contestwork.crypto.iv.lo =  ( (*test_cases)[testnum][2] );
          contestwork.crypto.iv.hi =  ( (*test_cases)[testnum][3] );
          contestwork.crypto.plain.lo = ( (*test_cases)[testnum][4] );
          contestwork.crypto.plain.hi = ( (*test_cases)[testnum][5] );
          contestwork.crypto.cypher.lo = ( (*test_cases)[testnum][6] );
          contestwork.crypto.cypher.hi = ( (*test_cases)[testnum][7] );
          contestwork.crypto.keysdone.lo = ( 0 );
          contestwork.crypto.keysdone.hi = ( 0 );
          contestwork.crypto.iterations.lo = ( 0x00020000L ); // 17 bits instead of 16
          contestwork.crypto.iterations.hi = ( 0 );
          break;
        case OGR:
          contestwork.ogr.workstub.stub.marks = (u16)((*test_cases)[testnum][1]);
          contestwork.ogr.workstub.stub.length = 6;
          for (int i = 0; i < TEST_CASE_DATA-2; i++) {
            contestwork.ogr.workstub.stub.diffs[i] = (u16)((*test_cases)[testnum][2+i]);
            if (contestwork.ogr.workstub.stub.diffs[i] == 0) {
              contestwork.ogr.workstub.stub.length = (u16)i;
              break;
            }
          }
          contestwork.ogr.workstub.worklength = contestwork.ogr.workstub.stub.length;
          contestwork.ogr.nodes.lo = contestwork.ogr.nodes.hi = 0;
          break;
      }
  
      problem->LoadState( &contestwork, contest, tslice, 0, 0, 0, 0 );

      ClientEventSyncPost( CLIEVENT_SELFTEST_TESTBEGIN, (long)(problem) );

      do
      {
        if (non_preemptive_env)
        {
          #if (CLIENT_OS == OS_WIN16) || (CLIENT_OS == OS_WIN32) /* win32s */
          w32Yield(); /* pump waiting messages */
          #elif (CLIENT_OS == OS_MACOS)
          sched_yield(); /* posix <sched.h> */
          #elif (CLIENT_OS == OS_RISCOS)
          riscos_upcall_6();
          #elif (CLIENT_OS == OS_NETWARE)
          nwCliThreadSwitchLowPriority();
          #endif
        }
        if (CheckExitRequestTrigger())
        {
          userbreak = 1;
          break;
        }
        if (contest == OGR) /* show /some/ activity (the time changes) */
          LogScreen("\r%s: Test %02d working...", contname, testnum + 1 );
      } while ( problem->Run() == RESULT_WORKING );
  
      resultcode = RESULT_WORKING;
      if (!userbreak)
      {
        resultcode = problem->RetrieveState( &contestwork, NULL, 1 );

        switch (contest) 
        {
          case RC5:
          case DES:
          case CSC:
            if ( resultcode != RESULT_FOUND )                /* no solution */
            {
              contestwork.crypto.key.hi = contestwork.crypto.key.lo = 0;
              resulttext = "FAILED";
              resultcode = -1;
            }
            else if (contestwork.crypto.key.lo != expectedsolution_lo || 
                     contestwork.crypto.key.hi != expectedsolution_hi)   
            {                                                /* wrong solution */
              resulttext = "FAILED";
              resultcode = -1;
            } 
            else                                             /* correct solution */
            {
              resulttext = "passed";
              successes++;
              #if 0
              if (contest == DES)
              {
                /* original expected solution */
                expectedsolution_hi = (*test_cases)[testnum][1]; 
                expectedsolution_lo = (*test_cases)[testnum][0];
                convert_key_from_inc_to_des(&(contestwork.crypto.key.hi), 
                                            &(contestwork.crypto.key.lo));
              } 
              #endif
            }
            LogScreen( "\r%s: Test %02d %s: %08X:%08X-%08X:%08X\n", 
               contname, testnum + 1, resulttext,
               contestwork.crypto.key.hi, contestwork.crypto.key.lo, 
               expectedsolution_hi, expectedsolution_lo );
            break;

          case OGR:
            if (expectedsolution_lo & 0x80000000) { // no solution
              expectedsolution_lo = ~expectedsolution_lo;
              if (resultcode != RESULT_NOTHING ||
                  contestwork.ogr.nodes.lo != expectedsolution_lo) {
                resulttext = "FAILED";
                resultcode = -1;
              } else {
                resulttext = "passed";
                successes++;
              }
            } else {
              if (resultcode != RESULT_FOUND ||
                  contestwork.ogr.nodes.lo != expectedsolution_lo) {
                resulttext = "FAILED";
                resultcode = -1;
              } else {
                resulttext = "passed";
                successes++;
              }
            }
            LogScreen( "\r%s: Test %02d %s: %s %08X-%08X\n", 
               contname, testnum + 1, resulttext,
               ogr_stubstr(&contestwork.ogr.workstub.stub),
               contestwork.ogr.nodes.lo, expectedsolution_lo );
            break;
        } /* switch */

      } /* if (!userbreak) */

      ClientEventSyncPost( CLIEVENT_SELFTEST_TESTEND, (long)resultcode );
      delete problem;

    } /* for ( testnum = 0 ; testnum < TEST_CASE_COUNT ; testnum++ ) */

    if (userbreak)
      successes = -1;
    else 
    {
      if (successes > 0)
      {
        LogScreen( "\n%s: %d/%d Tests Passed\n", contname,
          (int) successes, (int) TEST_CASE_COUNT );
      }
      if (successes != TEST_CASE_COUNT)
      {
        LogScreen( "\n%s: WARNING WARNING WARNING: %d Tests FAILED!!!\n", 
          contname, (int) (TEST_CASE_COUNT - successes) );
          successes=-successes;
      }
    }
    
    ClientEventSyncPost( CLIEVENT_SELFTEST_FINISHED, (long)(successes) );

  } /* for ( threadpos = 0; threadpos < threadcount; threadpos++ ) */
  
  return (successes);
}

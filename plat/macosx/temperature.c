/* * Copyright distributed.net 2003 - All Rights Reserved * For use in distributed.net projects only. * Any other distribution or use of this source violates copyright. * * int macosx_cputemp(); * * Returns the CPUs temperature in Kelvin, else -1. * For multiple cpus, gets one with highest temperature. * A test main() is at the end of this file. * * Currently we try 2 different sources: *   - host_processor_info(), which uses a CPUs builtin Thermal Assist Unit *   - AppleCPUThermo, an IOKit Object that provides "temp-monitor" from a dedicated sensor near the CPU * ( - IOHWSensor, an IOKit Object that provides "temp-sensor" data from a dedicated sensor ) * * TODO:Read all temperatures and report the highest one *      Add IOHW Sensor *	cache IOKit objects * */#include <stdio.h>/* for _readTAU() */#include <mach/mach.h>#include <mach/mach_error.h>/* for IOKit Objects */#include <IOKit/IOKitLib.h>#include <CoreFoundation/CFNumber.h>#ifdef __cplusplus /* to ensure gcc -lang switches don't mess this up */extern "C" {#endif    int macosx_cputemp();#ifdef __cplusplus}#endifint _readTAU() {    int                    cputemp = -1;    kern_return_t          ret;    natural_t              processorCount;    processor_info_array_t processorInfoList;    mach_msg_type_number_t processorInfoCount;    // pass a message to the kernel that we need some info    ret = host_processor_info( mach_host_self(), // get info from this host                               PROCESSOR_TEMPERATURE, // want temperature                               &processorCount,	// get processor count                               &processorInfoList,                               &processorInfoCount);    if (ret==KERN_SUCCESS) {        // get temperature for 1st processor, -1 on failure        cputemp = ((int*)processorInfoList)[0];        if (vm_deallocate(mach_task_self(),                          (vm_address_t)processorInfoList,                          processorInfoCount)!=KERN_SUCCESS) {            //deallocation failed?            cputemp = -1;        }                if (cputemp!=-1) {            cputemp += 273/*.15*/; /* C -> K */        }    }    return cputemp;}int _readAppleCPUThermo() {    SInt32 temp = -1;    CFNumberRef number = NULL;    io_object_t handle = NULL;     io_iterator_t objectIterator = NULL;    CFMutableDictionaryRef properties = NULL;    if (kIOReturnSuccess == IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceNameMatching("AppleCPUThermo"), &objectIterator)) {        if ( (handle = IOIteratorNext(objectIterator)) ) {            if (kIOReturnSuccess == IORegistryEntryCreateCFProperties(handle, &properties, kCFAllocatorDefault, kNilOptions)) {                if(CFDictionaryGetValueIfPresent(properties, CFSTR("temperature"), (const void **)&number)) {                    if (CFNumberGetValue(number, kCFNumberSInt32Type, &temp))                        temp = (temp / 256) + 273/*.15*/;                         //printf("  Temperature : %2.2f °C ( == 0x%04X / 256)\n", temp / 256.0, (unsigned) temp);                    else                        temp = -2; // Conversion error                    CFRelease(number);                }                CFRelease(properties);            }            IOObjectRelease(handle);        }        IOObjectRelease(objectIterator);    }        return temp;}int macosx_cputemp() {    static int source = 0;    int temp = -1;        switch (source) {        case 1:            return _readTAU();        case 2:            return _readAppleCPUThermo();        default:            temp = _readTAU();            if (temp >= 0) {source = 1; break;}            temp = _readAppleCPUThermo();            if (temp >= 0) {source = 2; break;}    }    return temp;    }#if 0int main(int argc,char *argv[]){    printf("TAU %d Kelvin %d Celsius\n",_readTAU(),_readTAU()-273/*.15*/);    printf("AppleCPUThermo: %d Kelvin (%d Celsius)\n",_readAppleCPUThermo(),_readAppleCPUThermo()-273/*.15*/);    while (1) { // test for leaks        printf("Temp %d\n",macosx_cputemp());    }        return 0;}#endif
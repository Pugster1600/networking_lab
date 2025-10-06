#ifdef _WIN32
  #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
  // Compiling on a non-Windows platform
  #include "Types.h"
#endif

#include <stdio.h> //location resolved by the compiler when you tell it where to look for includes
#include <stdlib.h>
#include <stdint.h>
#include "FTD3XX.h"
#include <inttypes.h>

void displayDevices();

int main() {
  printf("What is going on guys\n"); //sanity check
  displayDevices();
  system("powershell.exe");
  return 0;
}

void displayDevices(){
  FT_STATUS ftStatus; 
 DWORD numDevs = 0; 
 
 ftStatus = FT_CreateDeviceInfoList(&numDevs); 
 printf("testing");
 printf("Number of devices: %lu\n", numDevs);
 if (!FT_FAILED(ftStatus) && numDevs > 0) 
 { 
  FT_HANDLE ftHandle = NULL; 
  DWORD Flags = 0; 
  DWORD Type = 0; 
  DWORD ID = 0; 
  char SerialNumber[16] = { 0 }; 
  char Description[32] = { 0 }; 
 
  printf("List of Connected Devices!\n\n"); 
  for (DWORD i = 0; i < numDevs; i++) 
  { 
   ftStatus = FT_GetDeviceInfoDetail(i, &Flags, &Type, &ID, NULL,  
SerialNumber, Description, &ftHandle); 
   if (!FT_FAILED(ftStatus)) 
   { 
    printf("Device[%d]\n", i); 
    printf("\tFlags: 0x%x %s | Type: %d | ID: 0x%08X | ftHandle=0x%x\n", 
     Flags, 
     Flags & FT_FLAGS_SUPERSPEED ? "[USB 3]" : 
     Flags & FT_FLAGS_HISPEED ? "[USB 2]" : 
     Flags & FT_FLAGS_OPENED ? "[OPENED]" : "", 
     Type, 
     ID, 
     ftHandle); 
    printf("\tSerialNumber=%s\n", SerialNumber); 
    printf("\tDescription=%s\n", Description); 
   } 
  } 
 }

 else {
  printf("failed");
 }
}


#include <stdio.h> //location resolved by the compiler when you tell it where to look for includes
#include <stdlib.h>
#include <stdint.h>

#ifdef _WIN32
  #include <windows.h>
#elif defined(__linux__) || defined(__APPLE__)
  // Compiling on a non-Windows platform
  #include "Types.h"
#endif


#include "FTD3XX.h"
#include <inttypes.h>

void displayDevices();

typedef struct Result {
  uint32_t quotient;
  uint32_t remainder;
} Result;

Result division(uint32_t dividend, uint32_t divisor) {
  uint64_t combined = (uint64_t) dividend;
  uint32_t remainder = 0;
  Result result;
  //printf("%lu", &mask)
  //combined |= dividend; //remainder : dividend
  for (uint32_t i = 0; i < 32u; i++) {
    combined <<= 1;
    remainder = (uint32_t)(combined >> 32); //bottom 32 all 0s

    //remainder - divisor >= 0
    if (remainder >= divisor) {
      combined |= 0x1u; //set LSB = 1
      remainder = remainder - divisor;
      combined &= 0x00000000FFFFFFFFULL; // Clear top 32 bits
      combined |= ((uint64_t)remainder << 32); // Set new remainder

      //printf("Value: %" PRIu32 "\n", combined);
    }
  }

  result.quotient = (uint32_t)(combined & 0xFFFFFFFF);
  result.remainder = (uint32_t)(combined >> 32);
  return result;
}
int main() {
  printf("hello world\n");
  //displayDevices();
  Result result = division(1000, 3);
  printf("Quotient: %" PRIu32 " Remainder: %" PRIu32, result.quotient, result.remainder); //expects actual value, not pointer
  //printf("quotient: %d, remainder: %d", result, result+1); //addresses
  return 0;
}

void displayDevices(){
  FT_STATUS ftStatus; 
 DWORD numDevs = 0; 
 
 ftStatus = FT_CreateDeviceInfoList(&numDevs); 
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
}


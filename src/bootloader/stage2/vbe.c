#include "vbe.h"
#include <stdint.h>
#include "x86.h"
#include "memory.h"
// #include "stdio.h"


bool VBE_GetControllerInfo(VbeInfoBlock* info) {
// printf("VBE_GetControllerInfo: %x\r\n",x86_Video_GetVbeInfo(info));
if (x86_Video_GetVbeInfo(info)==0x004f) {
    // seg:off to linear
  // printf("worked!!");
  info->VideoModePtr = SEGOFF_LIN(info->VideoModePtr);
  return true;
}
return false;
}

bool VBE_GetModeInfo(uint16_t mode, VbeModeInfo* info) {
  if (x86_Video_GetModeInfo(mode, info)==0x004f){
    return true;
}
return false;

}
bool VBE_SetModeInfo(uint16_t mode) {
  return x86_Video_SetModeInfo(mode)==0x004f;
}

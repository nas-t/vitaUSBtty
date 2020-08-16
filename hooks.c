/* 
 * This file is part of the vitaUsbTty distribution (https://github.com/nas-t/vitaUSBtty).
 * Copyright (c) 2020 Erik Fernholz.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <psp2kern/types.h>
#include <psp2kern/kernel/sysmem.h>
#include <taihen.h>
#include <coredump.h>
#include <koffsets.h>


// Hook variables
static tai_hook_ref_t ksceSblACMgrIsGameProgramRef;
int hook;
static tai_hook_ref_t threadInfoRef;
int hook2;
static tai_hook_ref_t SceBacktraceRef;
int hook4;
static tai_hook_ref_t threadRegRef;
int hook5;
static tai_hook_ref_t moduleInfoRef;
int hook6;
static tai_hook_ref_t ksceSblACMgr991fdc15Ref;
int hook7;

int hook8;
int hook9;

static tai_hook_ref_t ksceSblACMgrIsShellRef;
int hook10;
static tai_hook_ref_t stackInfoRef;
int hook11;

uint32_t nop_nop = 0xBF00BF00;





// patches return of ksceSblACMgrIsGameProgram
static int ksceSblACMgrIsGameProgramPatched(SceUID uid)
{

	int ret = TAI_CONTINUE(int, ksceSblACMgrIsGameProgramRef, uid);
	ret = 1;
	
	return ret;
}

static int ksceSblACMgrIsShellPatched(int unk)
{

	int ret = TAI_CONTINUE(int, ksceSblACMgrIsShellRef, unk);
	ret = 1;
	
	return ret;
}


static int ksceSblACMgr991fdc15Patched(int unk)
{

	int ret = TAI_CONTINUE(int, ksceSblACMgr991fdc15Ref, unk);
	ret = 1;
	
	return ret;
}


static char *getCauseName(int cause) {
  switch (cause) {
    case 0:
      return "None";
	  
    case 0x30002:
      return "Undefined instruction exception";
      
    case 0x30003:
      return "Prefetch abort exception";
    
    case 0x30004:
      return "Data abort exception";
      
    case 0x60080:
      return "Division by zero";
  }
  
  return "Unknown cause";
}

// patches coredump threadInfo Function
static int threadInfoPatched(uint32_t pid, SceUID uid, void * unk, ThreadInfo * info) {

	int ret = TAI_CONTINUE(int, threadInfoRef, pid, uid, unk, info);
	
	ksceDebugPrintf("================= THREAD INFO [%s] ================\nPC: 0x%08x\nUid: 0x%x\nCause: %s (0x%x)\nPID: 0x%x\n", info->name, info->pc, info->uid, getCauseName(info->cause), info->cause, pid);
	
	
	return ret;
}

// patches coredump threadInfo Function
static int threadRegPatched(uint32_t pid, void * unk, ThreadRegInfo * info) {

	int i;
	int ret = TAI_CONTINUE(int, threadRegRef, pid, unk, info);
	
	ksceDebugPrintf("=============== THREAD REGISTERS [UID: %x]=============\nBad Address: 0x%08x\n", info->uid, info->bad_v_addr);
	
	for(i=0;i<16;i++)
	{
		if(i%2)
			ksceDebugPrintf("r%2d: 0x%08x\n", i, info->regs[i]);
		else
			ksceDebugPrintf("r%2d: 0x%08x   ", i, info->regs[i]);
		
	}
	
	return ret;
}

// patches coredump moduleInfo Function
static int moduleInfoPatched(ModuleInfo1 * info, ModuleSegment * segment, int num, ModuleInfo1 * addrs, uint32_t * size) {

	int i;
	int ret = TAI_CONTINUE(int, moduleInfoRef, info, segment, num, addrs, size);
	
	ksceDebugPrintf("================= MODULE INFO [%s] ================\n", info->name);
	ksceDebugPrintf("NID:    %x    SDK Version: %x\n", info->nid, info->sdk_version);
	
	ksceDebugPrintf("[Seg #] ADDRESS     SIZE\n");
	for(i=0;i<num;i++)
	{
		ksceDebugPrintf("[Seg %d] 0x%08x  0x%08x\n", i, segment->addr, segment->size);
		segment += 0x14;
	}
	
	return ret;
}

// patches backtrace Function
static int SceBacktracePatched(uint32_t tid, uint32_t unk1, uint32_t unk2, uint32_t mem) {

	int ret = TAI_CONTINUE(int, SceBacktraceRef, tid, unk1, unk2, mem);
	ret = 1;
	
	return ret;
}

// patches coredump stackInfo Function
static int stackInfoPatched(int pid, uint32_t unk, char * buffer) {

	int i;
	int ret = TAI_CONTINUE(int, stackInfoRef, pid, unk, buffer);
	
	ksceDebugPrintf("================= STACK INFO ================\n");
	hexdump(buffer, 0x10);
	
	return ret;
}

void installHooks()
{
	

	// patches ksceSblACMgrIsGameProgram for SceUsbSerial calls
	hook = taiHookFunctionImportForKernel(KERNEL_PID, &ksceSblACMgrIsGameProgramRef, "SceUsbSerial",
											0x9AD8E213, 0x62CAEB2, ksceSblACMgrIsGameProgramPatched);
	hook4 = taiHookFunctionExportForKernel(KERNEL_PID, &SceBacktraceRef, "SceKernelModulemgr",
											0x77CB3DD6, 0xbf371a98, SceBacktracePatched);
	// hook core dup functions
	tai_module_info_t tai_info;
	tai_info.size = sizeof(tai_module_info_t);
	taiGetModuleInfoForKernel(KERNEL_PID, "SceCoredump", &tai_info);											
											
											
	hook2 = taiHookFunctionOffsetForKernel(KERNEL_PID, &threadInfoRef, tai_info.modid, 0, THREADINFO, 1, threadInfoPatched);
	hook5 = taiHookFunctionOffsetForKernel(KERNEL_PID, &threadRegRef, tai_info.modid, 0, THREADREG, 1, threadRegPatched);
	hook6 = taiHookFunctionOffsetForKernel(KERNEL_PID, &moduleInfoRef, tai_info.modid, 0, MODULEINFO, 1, moduleInfoPatched);
	hook11 = taiHookFunctionOffsetForKernel(KERNEL_PID, &stackInfoRef, tai_info.modid, 0, STACKINFO, 1, stackInfoPatched);
	
	
	//process related patches for e.g. list, spawn, etc.
	hook7 = taiHookFunctionImportForKernel(KERNEL_PID, &ksceSblACMgr991fdc15Ref, "SceAppMgr",
											0x9AD8E213, 0x991fdc15, ksceSblACMgr991fdc15Patched);
											
	tai_info.size = sizeof(tai_module_info_t);
	taiGetModuleInfoForKernel(KERNEL_PID, "SceAppMgr", &tai_info);
	
	hook8 = taiInjectDataForKernel(KERNEL_PID, tai_info.modid, 0, APPMGRPATCH1, &nop_nop, 2);
	hook9 = taiInjectDataForKernel(KERNEL_PID, tai_info.modid, 0, APPMGRPATCH2, &nop_nop, 2);
	
	hook10 = taiHookFunctionImportForKernel(KERNEL_PID, &ksceSblACMgrIsShellRef, "SceAppMgr",
											0x9AD8E213, 0x8612B243, ksceSblACMgrIsShellPatched);
}


void removeHooks()
{
	taiHookReleaseForKernel(hook, ksceSblACMgrIsGameProgramRef);
	taiHookReleaseForKernel(hook2, threadInfoRef);
	taiHookReleaseForKernel(hook4, SceBacktraceRef);
	taiHookReleaseForKernel(hook5, threadRegRef);
	taiHookReleaseForKernel(hook6, moduleInfoRef);
	taiHookReleaseForKernel(hook7, ksceSblACMgr991fdc15Ref);
	taiInjectReleaseForKernel(hook8);
	taiInjectReleaseForKernel(hook9);
	taiHookReleaseForKernel(hook10, ksceSblACMgrIsShellRef);
	taiHookReleaseForKernel(hook11, ksceSblACMgrIsShellRef);
}
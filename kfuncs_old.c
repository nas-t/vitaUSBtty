#include <psp2kern/types.h>
#include <psp2/appmgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <utils.h>

// References to USB Serial functions without exports on 3.60
int (*ksceUsbSerialStart)(void) = (void*)0x168D;
int (*ksceUsbSerialSetup)(int unk) = (void*)0x17A1;
int (*ksceUsbSerialStatus)(void) = (void*)0x1E79;
int (*ksceUsbSerialClose)(void) = (void*)0x1739;
unsigned int (*ksceUsbSerialGetRecvBufferSize)(void) = (void*)0x1D31;
unsigned int (*ksceUsbSerialSend)(const void *buffer, unsigned int len, int unk1, int unk2) = (void*)0x1A69;
unsigned int (*ksceUsbSerialRecv)(void *buffer, unsigned int max_len, int unk1, int unk2) = (void*)0x1A59;


// References to AppMgr functions without exports on 3.60
int (*ksceAppMgrLaunchAppByName2ForShell)(const char * name, const char * param, SceAppMgrLaunchAppOptParam * optParam) = (void*)0xD795; 	
int (*ksceAppMgrGetRunningAppIdListForShell)(SceUID *appIds, int count) = (void*)0x11A1D;
int (*ksceAppMgrGetPidListForShell)(SceUID *pids, int count) = (void*)0x1117D;
int (*ksceAppMgrDestroyOtherAppByAppIdForShell)(SceUID appId) = (void*)0xF565;
int (*ksceAppMgrDestroyAppByName)(char *titleId) = (void*)0x32405;
int (*ksceAppMgrGetNameById)(SceUID pid, char *name) = (void*)0x10235;
int (*ksceAppMgrLaunchAppByUri)(int flags, char *uri) = (void*)0x322E5;


// resolves some SceAppMgr kernel functions
int initAppMgr()
{
	int ret;
	uint32_t addr;
	
	ret = getModuleSegmentBase("SceAppMgr", 0, &addr);
	if(ret != 0)
	{
		ksceDebugPrintf("getModuleSegmentBase returned: 0x%x\n", ret);
		return 0;
	}	

	ksceAppMgrLaunchAppByName2ForShell += addr;
	ksceAppMgrGetRunningAppIdListForShell += addr;
	ksceAppMgrGetPidListForShell += addr;
	ksceAppMgrDestroyOtherAppByAppIdForShell += addr;
	ksceAppMgrDestroyAppByName += addr;
	ksceAppMgrGetNameById += addr;
	ksceAppMgrLaunchAppByUri += addr;
	
	return 1;
}

// resolves UsbSerial kernel functions (not exported in 3.60)
int initSerDebug()
{
	int ret;
	uint32_t addr;
	
	ret = getModuleSegmentBase("SceUsbSerial", 0, &addr);
	if(ret != 0)
	{
		ksceDebugPrintf("getModuleSegmentBase returned: 0x%x\n", ret);
		return 0;
	}
	
	
	//set adresses:
	ksceUsbSerialStart += addr;
	ksceUsbSerialSetup += addr;
	ksceUsbSerialStatus += addr;
	ksceUsbSerialClose += addr;
	ksceUsbSerialGetRecvBufferSize += addr;
	ksceUsbSerialSend += addr;
	ksceUsbSerialRecv += addr;
	
	return 1;
}
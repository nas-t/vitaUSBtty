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
#include <psp2/appmgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <utils.h>
#include <koffsets.h>

// References to USB Serial functions (not exported since dunno)
int (*ksceUsbSerialStart)(void);
int (*ksceUsbSerialSetup)(int unk);
int (*ksceUsbSerialStatus)(void);
int (*ksceUsbSerialClose)(void);
unsigned int (*ksceUsbSerialGetRecvBufferSize)(void);
unsigned int (*ksceUsbSerialSend)(const void *buffer, unsigned int len, int unk1, int unk2);
unsigned int (*ksceUsbSerialRecv)(void *buffer, unsigned int max_len, int unk1, int unk2);


// References to AppMgr functions (not exported since dunno)
int (*ksceAppMgrLaunchAppByName2ForShell)(const char * name, const char * param, SceAppMgrLaunchAppOptParam * optParam); 	
int (*ksceAppMgrGetRunningAppIdListForShell)(SceUID *appIds, int count);
int (*ksceAppMgrGetPidListForShell)(SceUID *pids, int count);
int (*ksceAppMgrDestroyOtherAppByAppIdForShell)(SceUID appId);
int (*ksceAppMgrDestroyAppByName)(char *titleId);
int (*ksceAppMgrGetNameById)(SceUID pid, char *name);
int (*ksceAppMgrLaunchAppByUri)(int flags, char *uri);


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

	ksceAppMgrLaunchAppByName2ForShell = addr + KSCEAPPMGRLAUNCHAPPBYNAME2FORSHELL;
	ksceAppMgrGetRunningAppIdListForShell = addr + KSCEAPPMGRGETRUNNINGAPPIDLISTFORSHELL;
	ksceAppMgrGetPidListForShell = addr + KSCEAPPMGRGETPIDLISTFORSHELL;
	ksceAppMgrDestroyOtherAppByAppIdForShell = addr + KSCEAPPMGRDESTROYOTHERAPPBYAPPIDFORSHELL;
	ksceAppMgrDestroyAppByName = addr + KSCEAPPMGRDESTROYAPPBYNAME;
	ksceAppMgrGetNameById = addr + KSCEAPPMGRGETNAMEBYID;
	ksceAppMgrLaunchAppByUri = addr + KSCEAPPMGRLAUNCHAPPBYURI;
	
	return 1;
}

// resolves UsbSerial kernel functions (not exported since dunno)
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
	
	
	//set adresses
	ksceUsbSerialStart = addr + KSCEUSBSERIALSTART;
	ksceUsbSerialSetup = addr + KSCEUSBSERIALSETUP;
	ksceUsbSerialStatus = addr + KSCEUSBSERIALSTATUS;
	ksceUsbSerialClose = addr + KSCEUSBSERIALCLOSE;
	ksceUsbSerialGetRecvBufferSize = addr + KSCEUSBSERIALGETRECVBUFFERSIZE;
	ksceUsbSerialSend = addr + KSCEUSBSERIALSEND;
	ksceUsbSerialRecv = addr + KSCEUSBSERIALRECV;
	
	return 1;
}
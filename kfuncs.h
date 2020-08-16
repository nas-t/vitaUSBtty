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

#ifndef _H_KFUNCS_
#define _H_KFUNCS_

#include <stdint.h>
#include <psp2/appmgr.h>


extern int (*ksceUsbSerialStart)(void);
extern int (*ksceUsbSerialSetup)(int unk);
extern int (*ksceUsbSerialStatus)(void);
extern int (*ksceUsbSerialClose)(void);
extern unsigned int (*ksceUsbSerialGetRecvBufferSize)(void);
extern unsigned int (*ksceUsbSerialSend)(const void *buffer, unsigned int len, int unk1, int unk2);
extern unsigned int (*ksceUsbSerialRecv)(void *buffer, unsigned int max_len, int unk1, int unk2);



extern int (*ksceAppMgrLaunchAppByName2ForShell)(const char * name, const char * param, SceAppMgrLaunchAppOptParam * optParam); 	
extern int (*ksceAppMgrGetRunningAppIdListForShell)(SceUID *appIds, int count);
extern int (*ksceAppMgrGetPidListForShell)(SceUID *pids, int count);
extern int (*ksceAppMgrDestroyOtherAppByAppIdForShell)(SceUID appId);
extern int (*ksceAppMgrDestroyAppByName)(char *titleId);
extern int (*ksceAppMgrGetNameById)(SceUID pid, char *name);
extern int (*ksceAppMgrLaunchAppByUri)(int flags, char *uri);

int initSerDebug();
int initAppMgr();

#endif
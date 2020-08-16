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
#include <string.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/modulemgr.h>
#include <serial.h>
#include <hooks.h>
#include <kfuncs.h>


void _start() __attribute__ ((weak, alias ("module_start")));


//threads
SceUID gThreadMain;
int run = 1;

void initMainThread()
{
	gThreadMain = ksceKernelCreateThread("vitaUsbtty_main", (SceKernelThreadEntry)usbSerial_thread, 0x3C, 0x10000, 0, 0x10000, 0);
	ksceKernelStartThread(gThreadMain, 0, NULL);
}


int module_start(SceSize argc, const void *args) {
	
	// hook isGame function of SblAclmgr for SceUsbSerial
	installHooks();
	
	// resolve some kernel functions
	initSerDebug();
	initAppMgr();
	
	// register Handler
	initUsbSerialConsole();
	
	// start delay thread
	initMainThread();
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	
	run = 0;
	ksceKernelWaitThreadEnd(gThreadMain, NULL, NULL);

	deinitUsbSerialConsole();
	removeHooks();
	return SCE_KERNEL_STOP_SUCCESS;
}

void module_exit(void) {

}
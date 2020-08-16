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

#include <stdio.h>
#include <psp2kern/types.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2/appmgr.h>
#include <psp2kern/power.h> 
#include <kfuncs.h>
#include <serial.h>
#include <cmd.h>
#include <syscon_trace.h>
#include <utils.h>


int cmd_help(char ** cmd, int argnum)
{
	ksceDebugPrintf("vitaUsbtty help:\n");
	ksceDebugPrintf("\tb cold: perform cold reboot\n");
	ksceDebugPrintf("\tb soft: perform soft reboot\n");
	ksceDebugPrintf("\tp: list processes\n");
	ksceDebugPrintf("\tt: toggle Syscon trace\n");
	ksceDebugPrintf("\ts titleid: spawn process by titleid (e.g. PCBS00111)\n");
	ksceDebugPrintf("\tk titleid: kill process by titleid (e.g. PCBS00111)\n\n");
	return 0;
}

int cmd_kill(char ** cmd, int argnum)
{
	int ret = 0;
	
	if(argnum == 2)
	{
		ret = ksceAppMgrDestroyAppByName(cmd[1]);
		
		if(ret < 0)
			ksceDebugPrintf("cmd_kill: ksceAppMgrDestroyAppByName(\"%s\") returned (0x%x)\n", cmd[1], ret);
		else
		{
			ksceDebugPrintf("KILLING PROCESS [%s] (%d)\n", cmd[1], ret);
		}
	}
	else
	{
		ksceDebugPrintf("cmd_kill: invalid number of arguments (%d)\n", argnum);
	}
	
	
	return ret;
}

int cmd_proclist(char ** cmd, int argnum)
{
	SceUID pids[0x50];
	char name[0x100];
	int ret;
	int num;
	int i;
	
	num = ksceAppMgrGetPidListForShell(pids, 0x50);
	
	ksceDebugPrintf("RUNNING PROCS [%d]:\n", num);
	
	if(num > 0)
	{
		for(i=0;i<num;i++)
		{
			ret = ksceAppMgrGetNameById(pids[i], name);
			
			if(ret > -1)
				ksceDebugPrintf("\t[%x] %s\n", pids[i], name);
			else
				ksceDebugPrintf("\t[%x] unknown (%x)\n", pids[i], ret);
		}
	}
	
	ksceDebugPrintf("\n");
	
	return 0;
}

int cmd_spawn(char ** cmd, int argnum)
{
	int ret = -1;
	char uri[0x100];
	
	if(argnum == 2)
	{
		snprintf(uri, 0x100, "psgm:play?titleid=%s", cmd[1]);
		ret = ksceAppMgrLaunchAppByUri(0x20000, uri);
		
		if(ret < 0)
			ksceDebugPrintf("cmd_spawn: ksceAppMgrLaunchAppByName2ForShell(0x20000, \"%s\") returned (0x%x)\n", uri, ret);
		else
		{
			ksceDebugPrintf("SPAWN [%s] (%x)\n", cmd[1], ret);
		}
	}
	else
	{
		ksceDebugPrintf("cmd_spawn: invalid number of arguments (%d)\n", argnum);
	}
	
	return ret;
}

void cmd_toggle_scTrace()
{
	static int sc_enabled = 0;
	
	if (!sc_enabled)
	{
		initSysconDebug();
		sc_enabled = 1;
	}
	else
	{
		finiSysconDebug();
		sc_enabled = 0;
	}
}

void cmd_boot(char ** cmd, int argnum)
{
	if(argnum == 2)
	{
		switch(cmd[1][0])
		{
			case 'c':
				kscePowerRequestColdReset();
				break;
			case 's':
				kscePowerRequestSoftReset();
				break;
			default:
				ksceDebugPrintf("cmd_boot: invalid argument (%s)\n", cmd[1]);
				break;
		}
	}
	else
	{
		ksceDebugPrintf("cmd_boot: invalid number of arguments (%d)\n", argnum);
	}
}

int cmd_thread(int arglen, void *argp)
{
	char * cmd[10];
	int argnum;
	char * cmdBuf = (char *)argp;
	
	argnum = prepArgs(cmdBuf, cmd);
	
	switch(cmd[0][0])
	{
		case '?': //help
			cmd_help(cmd, argnum);
			break;
		case 's':
			//spawn
			cmd_spawn(cmd, argnum);
			break;
		case 'p':
			//proc list
			cmd_proclist(cmd, argnum);
			break;
		case 'k':
			cmd_kill(cmd, argnum);
			break;
		case 't':
			cmd_toggle_scTrace();
			break;
		case 'b':
			cmd_boot(cmd, argnum);
			break;
		default:
		ksceDebugPrintf("UNKNOWN CMD: %s\n", cmd[0]);
		ksceDebugPrintf("'?' to display help\n", cmd[0]);
			
	}

	//end
	ksceKernelExitDeleteThread(0);
	return 0;
}

void create_cmdParseThread(char * cmd)
{
	SceUID threadCmd;
	
	threadCmd = ksceKernelCreateThread("cmd", (SceKernelThreadEntry)cmd_thread, 0x3C, 0x10000, 0, 0x10000, 0);
	ksceKernelStartThread(threadCmd, RECV_BUFFER_SIZE, cmd);	
}
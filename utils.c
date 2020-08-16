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
#include <psp2kern/kernel/modulemgr.h>
#include <taihen.h>

int prepArgs(char * buffer, char ** list)
{
	char * ptr = buffer;
	int i = 1;
	int work = 1;
	
	list[0] = buffer;
	
	do
	{
		ptr = strchr(ptr, ' ');
		
		if(ptr != 0)
		{
			*(ptr) = 0;
			list[i] = ptr + 1;
			i++;
		}
		else
		{
			work = 0;
		}
	}
	while(work);
		
	return i;
}

uint32_t getModuleSegmentBase(char * name, int segment_num, uint32_t * addr)
{
	int ret;
	tai_module_info_t tai_info;	
	tai_info.size = sizeof(tai_module_info_t);
	
	ret = taiGetModuleInfoForKernel(KERNEL_PID, name, &tai_info);
	if (ret < 0)
		return ret;
	
	SceKernelModuleInfo modinfo;
	memset(&modinfo, 0, sizeof(modinfo));
	ret = ksceKernelGetModuleInfo(KERNEL_PID, tai_info.modid, &modinfo);
	
	if (ret < 0)
		return ret;
	
	SceKernelSegmentInfo *seginfo = &modinfo.segments[segment_num];
	
	*(addr) = (uint32_t)seginfo->vaddr;

	return 0;
}

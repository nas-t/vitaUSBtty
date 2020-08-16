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
#include <psp2kern/syscon.h>

//globals
SceSysconDebugHandlers sysconDebugHandlers;


void scPrintPacket(SceSysconPacket *packet, int end)
{
	//ToDo: interpret packet and provide useful info
	ksceDebugPrintf("SYSCON PKG CMD %02x%02x end:%d\n", packet->tx[0], packet->tx[1], end);
}

void scPrintPacketStart(SceSysconPacket *packet)
{
	scPrintPacket(packet, 0);
}

void scPrintPacketEnd(SceSysconPacket *packet)
{
	scPrintPacket(packet, 1);
}

void initSysconDebug()
{
	sysconDebugHandlers.size = 12;
	sysconDebugHandlers.start = scPrintPacketStart;
	sysconDebugHandlers.end = scPrintPacketEnd;
	ksceSysconSetDebugHandlers(&sysconDebugHandlers);
};

void finiSysconDebug()
{
	ksceSysconSetDebugHandlers(NULL);
}
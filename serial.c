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
#include <string.h>
#include <psp2kern/kernel/sysmem.h>
#include <psp2kern/kernel/threadmgr.h>
#include <kfuncs.h>
#include <cmd.h>
#include <serial.h>
#include <utils.h>
#include <koffsets.h>

// 
int ksceDebugDisableInfoDump(int val);
extern int run;

char gConsoleLineBuffer[BUFFER_SIZE];
unsigned int gConsolePosition;


int usbSerial_thread(int args, void *argp)
{
	char recvBuffer[RECV_BUFFER_SIZE];
	char * pBuffer = recvBuffer;
	
	// ToDo: find better way to determine when MTP can
	// be stopped. MTP startup will crash if UsbSerial 
	// is started too early.
	ksceKernelDelayThread(5000*1000);
	
	// setup Usb Serial interface
	setupSerial();
	
	memset(recvBuffer, 0, RECV_BUFFER_SIZE);
	
	while(run)
	{

		ksceKernelDelayThread(50*1000);
		
		//try to send console buffer
		if(gConsolePosition > 0)
			sendBuffer();
		
		//try to read from serial
		if(readChar(pBuffer) > -1)
		{			
			if(*(pBuffer) == '\r')
			{
				*(pBuffer) = 0;
				pBuffer = recvBuffer;
				ksceDebugPrintf(">%s\n", recvBuffer);
				create_cmdParseThread(recvBuffer);
			}
			else if(*(pBuffer) == '\n')
			{
				*(pBuffer) = 0;
			}
			else
			{
				pBuffer++;
			}
		}
	}
	
	return 0;
}

// setup usb interface, Vita Type D
// debug out is pretty useless here, 
// unless you have other means of debug out
int setupSerial()
{
	uint32_t ret;
	
	
	ret = ksceUsbSerialStart();	
	if(ret != 0)
		ksceDebugPrintf("ksceUsbSerialStart returned: 0x%x\n", ret);
	
	ret = ksceUsbSerialSetup(1);
	if(ret != 0)
		ksceDebugPrintf("ksceUsbSerialSetup returned: 0x%x\n", ret);
	
	return ret;
}

int sendBuffer()
{
	int ret = -1;
	
	if(ksceUsbSerialStatus())
	{
		ret = ksceUsbSerialSend(gConsoleLineBuffer, gConsolePosition, 1, -1);

		if(ret > -1)
		{
			gConsolePosition = 0;
			ret = 0;
		}
	}
	
	return ret;
}

int readChar(char * buffer)
{
    int ret = 0;

    if(ksceUsbSerialGetRecvBufferSize() < 1)
		return -1;
	
    if (ksceUsbSerialStatus() != 1)
        return -1;

    ret = ksceUsbSerialRecv(buffer, 1, 0, -1);

    return ret;
}


int kConsolePutChar_capture(void * args, char c)
{
  	int ret = 1;

	// collect chars in our line buffer
  	gConsoleLineBuffer[gConsolePosition] = c;
  	gConsolePosition++;

	// send line?
  	if(c != 0xA && c != 0xD && (BUFFER_SIZE-1) != gConsolePosition)
  		return ret;
  	else
  	{
		gConsoleLineBuffer[gConsolePosition-1] = '\r';
		gConsoleLineBuffer[gConsolePosition] = '\n';
		gConsolePosition++;


		// send buffer, if connected
		ret = sendBuffer();
		
		if(ret != 0)
		{
			//fail, do something
		}
				
		// reset if buffer filled completely and no serial connectivity
		if((BUFFER_SIZE-1) == gConsolePosition)
			gConsolePosition = 0;
	}
  	return ret;
}

void hexdump(uint8_t * buffer, uint32_t len)
{
	int i = 0;
	
	while(i<len)
	{
		ksceDebugPrintf("%02x", buffer[i]);
		i++;
		
		if(i%4 == 0)
			ksceDebugPrintf(" ");
		
		if(i%16 == 0)
			ksceDebugPrintf("\n");
	}
	
	ksceDebugPrintf("\n");
}

// copy kernel console buffer contents
int copyDebugBuffer()
{
	uint32_t addr;
	char * buffer;
	uint32_t buffer_len;
	int ret;
	
	ret = getModuleSegmentBase("SceSysmem", 1, &addr);
	if(ret != 0)
	{
		ksceDebugPrintf("getModuleSegmentBase returned: 0x%x\n", ret);
		return 1;
	}
	
	buffer = (char*)addr + CONSOLEBUFFER;
	
	buffer_len = 0x1000;
	
	memcpy(gConsoleLineBuffer, buffer, buffer_len);
	gConsolePosition = strlen(gConsoleLineBuffer);
	
	return 0;
}


int initUsbSerialConsole()
{
	gConsolePosition = 0;
	
	//copy existing Buffer
	copyDebugBuffer();
	
	//set PutChar Handler
	ksceDebugRegisterPutcharHandler(kConsolePutChar_capture, 0);
	ksceDebugDisableInfoDump(0);

	return 0;
}


int deinitUsbSerialConsole()
{
	ksceDebugDisableInfoDump(1);
	ksceDebugRegisterPutcharHandler(0, 0);

	return 0;
}

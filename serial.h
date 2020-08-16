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

#ifndef _H_SERIAL_
#define _H_SERIAL_

#include <psp2kern/types.h>

int initUsbSerialConsole();
int deinitUsbSerialConsole();
int setupSerial();
int kConsolePutChar_capture(void * args, char c);
void hexdump(uint8_t * buffer, uint32_t len);
int usbSerial_thread(int args, void *argp);
int sendBuffer();
int readChar(char * buffer);

#endif
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

#ifndef _H_COREDUMP_
#define _H_COREDUMP_

#include <stdint.h>


//structs taken from vitashell, thanks to theflow
typedef struct {
  int size; // 0x00
  int uid; // 0x04
  char name[32]; // 0x08
  int unk_28; // 0x28
  int unk_2C[18]; // 0x2C
  int cause; // 0x74
  int unk_78[9]; // 0x78
  uint32_t pc; // 0x9C
  int unk_A0[10]; // 0xA0
} ThreadInfo; // 0xC8

typedef struct {
  int size; // 0x00
  int uid; // 0x04
  uint32_t regs[16]; // 0x08
  int unk[75]; // 0x48
  uint32_t bad_v_addr; // 0x174
} ThreadRegInfo; // 0x178


typedef struct {
  int unk; // 0x00
  int perm; // 0x04
  uint32_t addr; // 0x08
  uint32_t size; // 0x0C
  int unk_10; // 0x10
} ModuleSegment; // 0x14

typedef struct {
  int unk_0; // 0x00
  int uid; // 0x04
  int sdk_version; // 0x08
  int version; // 0xC
  int unk_10; // 0x10
  int addr_14; // 0x14
  int unk_18; // 0x18
  int addr_1C; // 0x1C
  int unk_20; // 0x20
  char name[28]; // 0x24
  int unk_44; // 0x40
  int unk_48; // 0x44 2
  uint32_t nid; // 0x48
  int num_segments; // 0x4C
} ModuleInfo1; // 0x88

typedef struct {
  uint32_t addr[4]; // 0x??
} ModuleInfo2; // 0x??

#endif
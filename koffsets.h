/* 
 * This file is part of the XXX distribution (https://github.com/nas-t/vitaUSBtty).
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

#ifdef FW360CEX
// References to USB Serial functions without exports on 3.60
 #define KSCEUSBSERIALSTART				0x168D
 #define KSCEUSBSERIALSETUP				0x17A1
 #define KSCEUSBSERIALSTATUS			0x1E79
 #define KSCEUSBSERIALCLOSE				0x1739
 #define KSCEUSBSERIALGETRECVBUFFERSIZE	0x1D31
 #define KSCEUSBSERIALSEND				0x1A69
 #define KSCEUSBSERIALRECV				0x1A59

// References to AppMgr functions without exports on 3.60
 #define KSCEAPPMGRLAUNCHAPPBYNAME2FORSHELL			0xD795 	
 #define KSCEAPPMGRGETRUNNINGAPPIDLISTFORSHELL		0x11A1D
 #define KSCEAPPMGRGETPIDLISTFORSHELL				0x1117D
 #define KSCEAPPMGRDESTROYOTHERAPPBYAPPIDFORSHELL	0xF565
 #define KSCEAPPMGRDESTROYAPPBYNAME					0x32405
 #define KSCEAPPMGRGETNAMEBYID						0x10235
 #define KSCEAPPMGRLAUNCHAPPBYURI					0x322E5
 
// AppMgr Patch Offsets
 #define APPMGRPATCH1	0x111BA
 #define APPMGRPATCH2	0x1024C
 
//Coredump helper functions on 3.60
 #define THREADINFO	0x205C
 #define THREADREG	0x2F40
 #define MODULEINFO	0xAE28
 #define STACKINFO	0x229C
 
//SysMem Offsets
 #define CONSOLEBUFFER	0x4B98;
#endif
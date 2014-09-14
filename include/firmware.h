/*
 *  Copyright (C) 2014  venomDev
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef	_FIRMWARE_H
#define	_FIRMWARE_H

#include "setup.h"

#define FIRMWARE_MAGIC_0	MC_CONST('I', 'O', 'S', 'y')
#define FIRMWARE_MAGIC_1	MC_CONST('s', '3', '2', 0)

#define FIRMWARE_INFO_BASE	0x88000

#define	MSG_MAX_LEN		1024


enum FirmwareService {
	Shutdown
};

struct FirmwareInfo {
	Bit32u magic[2];
	Bit32u version;
	Bit32u entryAddr;
	Bit32u checksum;
	Bit32u extensionCnt;
};

// Firmware DOSBox Module
class FIRMWARE:public Module_base {
public:
	FIRMWARE(Section *sec): Module_base(sec) {};
	const char *propString(const char *prop);

private:
};

// Firmware System control
class FirmwareSystem {
public:
	FirmwareSystem(std::string name, void (*init)(const char *), void (*boot)(const char *)):
		Name(name), Init(init), Boot(boot) {};
	void init(const char *);
	void boot(const char *);
	std::string name();

private:
	std::string Name;
	void (*Boot)(const char *);
	void (*Init)(const char *);
};

void fwShutdown(int status);

#endif		// _FIRMWARE_H

/*
 *  Copyright (C) 2014  BoxOn
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

#include "boxon.h"
#include "firmware.h"

void addFirmware(std::string name, void (*init)(const char *), void (*boot)(const char *));

// Declare supported firmware
void SMB_Boot(const char *rc);
void boxonIOInit(const char *rc);

static std::list<FirmwareSystem *> fwList;

/*
 * Initialize list of supported firmware
 */
char **initFirmwareList()
{
	char **list, *str;
	int li, sz;
	std::string name;
	std::list<FirmwareSystem *>::iterator it;

	// Add firmware to support list
	addFirmware("boxonio", &boxonIOInit, &SMB_Boot);

	// Create list of system names
	sz = fwList.size();
	list = new char *[sz + 1];
	for(li = 0, it = fwList.begin(); it != fwList.end(); it++, li++){
		name = (*it)->name();
		str = new char[name.size() + 1];
		name.copy(str, name.size(), 0);
		list[li] = str;
	}
	list[li] = 0;
	return list;
}

/*
 * Add firmware to the support list
 */
void addFirmware(std::string name, void (*init)(const char *), void (*boot)(const char *))
{
	FirmwareSystem *fw;

	fw = new FirmwareSystem(name, init, boot);
	fwList.push_back(fw);
}

/*
 * Find the firmware in the support list
 */
FirmwareSystem *findFirmware(const char *name)
{
	std::list<FirmwareSystem *>::iterator it;
	FirmwareSystem *fwSys;

	fwSys = NULL;
	for(it = fwList.begin(); it != fwList.end(); it++){
		if((*it)->name() == name){
			fwSys = *it;
			break;
		}
	}
	return fwSys;
}

/*
 * Copyright (c) 2014, venomdev
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
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

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

#include <errno.h>

#include "boxon.h"
#include "bxndevice.h"
#include "firmware.h"

// Device init list
DeviceControl *ataDevInit();

static DeviceControl *devTable[DEV_TYPE_MAX];

/*
 * Initialize the devices
 */
void bxnDeviceInit(void)
{
	devTable[DEV_TYPE_ATA] = ataDevInit();
}

/*
 * Get a device control
 */
DeviceControl *getDeviceControl(unsigned int devId)
{
	unsigned int devType;
	DeviceControl *devCtl;

	devType = DEV_TYPE(devId);

	if(devType > DEV_TYPE_MAX){
		error(ENODEV);
	}

	devCtl = devTable[devType];
	if(devCtl == NULL){
		error(ENODEV);
	}

	return devCtl;
Error:
	return NULL;
}

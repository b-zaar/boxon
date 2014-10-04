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
 * WHATSOEVER RESULTING FROM LOSS OF USE, Dframe buffer OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <errno.h>

#include "boxon.h"
#include "boxonio/sys/boxonio.h"
#include "boxonio/sys/dev/fbdev.h"
#include "firmware.h"

#define	FB_LIST_MAX	1

// Frame buffer init list;
FbDevice *bxnfbInit();

static FbDevice *fbDevList[FB_LIST_MAX];

/*
 * Get the frame buffer device
 */
static FbDevice *getFbDevice(int id)
{
	id = DEV_ID(id);
	if(id >= 4 || fbDevList[id] == NULL){
		error(ENODEV);
	}

	return fbDevList[id];
Error:
	return NULL;
}

/*
 * Open frame buffer device
 */
static int fbOpen(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	FbDevice *fb;

	if((fb = getFbDevice(id)) == NULL){
		error(ENODEV);
	}

	return fb->open(code, id, flags, data);
Error:
	code = -errno;
	return -1;
}

/*
 * Close frame buffer device
 */
static int fbClose(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	FbDevice *fb;

	if((fb = getFbDevice(id)) == NULL){
		error(ENODEV);
	}

	return fb->close(code, id, flags, data);
Error:
	code = -errno;
	return -1;
}

/*
 * Read frame buffer device
 */
static int fbRead(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &data)
{
	FbDevice *fb;

	if((fb = getFbDevice(id)) == NULL){
		error(ENODEV);
	}

	return fb->read(code, id, cnt, data);
Error:
	code = -errno;
	return -1;
}

/*
 * Write frame buffer Device
 */
static int fbWrite(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &data)
{
	FbDevice *fb;

	if((fb = getFbDevice(id)) == NULL){
		error(ENODEV);
	}

	return fb->write(code, id, cnt, data);
Error:
	code = -errno;
	return -1;
}

/*
 * IO control for frame buffer device
 */
static int fbIoctl(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	FbDevice *fb;

	if((fb = getFbDevice(id)) == NULL){
		error(ENODEV);
	}

	return fb->ioctl(code, id, flags, data);
Error:
	code = -errno;
	return -1;
}

static DeviceControl fbDev={
	fbOpen,
	fbClose,
	fbRead,
	fbWrite,
	fbIoctl
};

/*
 * Init the frame buffer device
 */
DeviceControl *fbDevInit()
{
	fbDevList[0] = bxnfbInit();
	return &fbDev;
}

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

#ifndef	BXNIO_SYS_DEVICE_H
#define	BXNIO_SYS_DEVICE_H

#include "boxonio/bxndevice.h"

struct DeviceControl {
	int (*open)(uint32_t &code, uint32_t &devId, uint32_t &flags, uint32_t &ptr);
	int (*close)(uint32_t &code, uint32_t &devId, uint32_t &flags, uint32_t &ptr);
	int (*read)(uint32_t &code, uint32_t &devId, uint32_t &cnt, uint32_t &ptr);
	int (*write)(uint32_t &code, uint32_t &devId, uint32_t &cnt, uint32_t &ptr);
	int (*ioctl)(uint32_t &code, uint32_t &devId, uint32_t &flags, uint32_t &ptr);
};

DeviceControl *getDeviceControl(uint32_t devId);

#endif		// BXNIO_SYS_DEVICE_H

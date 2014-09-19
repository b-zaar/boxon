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

#define	IMAGE_CNT	4;

static int open(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr);
static int close(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr);
static int read(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr);
static int write(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr);
static int ioctl(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr);

static DeviceControl ataDev ={
	open,
	close,
	read,
	write,
	ioctl
};

DeviceControl *devAtaInit()
{
	return &ataDev;
}

static int open(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	LOG_MSG("ATA - open: 0x%08x id: 0x%08x flags: 0x%08x callback: 0x%08x", code, id, flags, ptr);
	code = -ENOSYS;
	return -ENOSYS;
}

static int close(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	LOG_MSG("ATA - close: 0x%08x id: 0x%08x flags: 0x%08x callback: 0x%08x", code, id, flags, ptr);
	code = -ENOSYS;
	return -ENOSYS;
}

static int read(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr)
{
	LOG_MSG("ATA - read: 0x%08x id: 0x%08x flags: 0x%08x callback: 0x%08x", code, id, cnt, ptr);
	code = -ENOSYS;
	return -ENOSYS;
}

static int write(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr)
{
	LOG_MSG("ATA - write: 0x%08x id: 0x%08x flags: 0x%08x callback: 0x%08x", code, id, cnt, ptr);
	code = -ENOSYS;
	return -ENOSYS;
}

static int ioctl(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	LOG_MSG("ATA - ioctl: 0x%08x id: 0x%08x flags: 0x%08x callback: 0x%08x", code, id, flags, ptr);
	code = -ENOSYS;
	return -ENOSYS;
}

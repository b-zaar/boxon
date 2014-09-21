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
#include "bios_disk.h"
#include "boxon.h"
#include "bxndevice.h"
#include "firmware.h"

#define	BLOCK_SZ	512
#define	IMAGE_CNT	4

extern imageDisk *imageDiskList[IMAGE_CNT];

static int open(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr);
static int close(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr);
static int read(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr);
static int write(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr);
static int ioctl(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr);

static DeviceControl ataDev={
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

static imageDisk *getDiskImage(int drv)
{
	drv = DEV_ID(drv);
	if(drv >= 4 || imageDiskList[drv] == NULL){
		error(ENODEV);
	}

	return imageDiskList[drv];
Error:
	return NULL;
}

/*
 * Open ATA device
 */
static int open(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	code = 0;
	return 0;
}

/*
 * Close ATA device
 */
static int close(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	code = 0;
	return 0;
}

/*
 * Read ATA device
 */
static int read(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr)
{
	int i;
	char *rwBuf;
	imageDisk *img;
	uint32_t pos, buf;

	buf = boxReadD(ptr);
	pos = boxReadD(ptr + 4);

	// Count and position need to be block size aligned
	if(cnt % BLOCK_SZ || pos % BLOCK_SZ){
		error(EFAULT);
	}
	pos /= BLOCK_SZ;

	// Read count overflow
	if((int)cnt <= 0){
		error(EFBIG);
	}

	// Find the mounted image
	if((img = getDiskImage(id)) == NULL){
		error(errno);
	}

	// Read blocks
	rwBuf = new char[BLOCK_SZ];
	for(i = 0; i < cnt; i += BLOCK_SZ, buf += BLOCK_SZ, pos += BLOCK_SZ){
		if(img->Read_AbsoluteSector(pos, rwBuf) < 0){
			error(errno);
		}
		boxMemcpy(buf, rwBuf, BLOCK_SZ);
	}
	return cnt;

Error:
	return -1;
}

/*
 * Write ATA Device
 */
static int write(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr)
{
	int i;
	char *rwBuf;
	imageDisk *img;
	uint32_t pos, buf;

	buf = boxReadD(ptr);
	pos = boxReadD(ptr + 4);

	// Count and position need to be block size aligned
	if(cnt % BLOCK_SZ || pos % BLOCK_SZ){
		error(EFAULT);
	}
	pos /= BLOCK_SZ;

	// Read count overflow
	if((int)cnt <= 0){
		error(EFBIG);
	}

	// Find the mounted image
	if((img = getDiskImage(id)) == NULL){
		error(errno);
	}

	// Write blocks
	rwBuf = new char[BLOCK_SZ];
	for(i = 0; i < cnt; i += BLOCK_SZ, buf += BLOCK_SZ, pos += BLOCK_SZ){
		boxMemcpy(rwBuf, buf, BLOCK_SZ);
		if(img->Write_AbsoluteSector(pos, rwBuf) < 0){
			error(errno);
		}
	}
	return cnt;

Error:
	return -1;
}

/*
 * IO control for ATA device
 */
static int ioctl(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	code = -ENOSYS;
	return -ENOSYS;
}

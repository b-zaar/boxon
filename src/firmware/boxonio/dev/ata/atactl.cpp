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
#include "boxonio/sys/boxonio.h"
#include "firmware.h"

#define	BLOCK_SZ	512
#define	ATA_LIST_MAX	4

#define AtaDevice	imageDisk
#define	ataDevList	imageDiskList

extern AtaDevice *ataDevList[ATA_LIST_MAX];

/*
 * Get the ATA device
 */
static AtaDevice *getAtaDevice(int id)
{
	id = DEV_ID(id);
	if(id >= 4 || ataDevList[id] == NULL){
		error(ENODEV);
	}

	return ataDevList[id];
Error:
	return NULL;
}

/*
 * Open ATA device
 */
static int ataOpen(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	AtaDevice *ata;

	if(flags != 0){
		error(EIO);
	}

	if((ata = getAtaDevice(id)) == NULL){
		error(ENODEV);
	}

	return 0;
Error:
	code = -errno;
	return -1;
}

/*
 * Close ATA device
 */
static int ataClose(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	return 0;
}

/*
 * Read ATA device
 */
static int ataRead(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr)
{
	int i;
	char *rwBuf;
	AtaDevice *ata;
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
	if((ata = getAtaDevice(id)) == NULL){
		error(errno);
	}

	// Read blocks
	rwBuf = new char[BLOCK_SZ];
	for(i = 0; i < cnt; i += BLOCK_SZ, buf += BLOCK_SZ, pos += BLOCK_SZ){
		if(ata->Read_AbsoluteSector(pos, rwBuf) < 0){
			error(errno);
		}
		boxMemcpy(buf, rwBuf, BLOCK_SZ);
	}

	return cnt;
Error:
	code = -errno;
	return -1;
}

/*
 * Write ATA Device
 */
static int ataWrite(uint32_t &code, uint32_t &id, uint32_t &cnt, uint32_t &ptr)
{
	int i;
	char *rwBuf;
	AtaDevice *ata;
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
	if((ata = getAtaDevice(id)) == NULL){
		error(errno);
	}

	// Write blocks
	rwBuf = new char[BLOCK_SZ];
	for(i = 0; i < cnt; i += BLOCK_SZ, buf += BLOCK_SZ, pos += BLOCK_SZ){
		boxMemcpy(rwBuf, buf, BLOCK_SZ);
		if(ata->Write_AbsoluteSector(pos, rwBuf) < 0){
			error(errno);
		}
	}

	return cnt;
Error:
	code = -errno;
	return -1;
}

/*
 * IO control for ATA device
 */
static int ataIoctl(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &ptr)
{
	error(ENOSYS);

	return 0;
Error:
	code = -errno;
	return -1;
}

static DeviceControl ataDev={
	ataOpen,
	ataClose,
	ataRead,
	ataWrite,
	ataIoctl
};

/*
 * Init the ATA device
 */
DeviceControl *ataDevInit()
{
	return &ataDev;
}

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
#include "boxonio.h"
#include "callback.h"
#include "bxndevice.h"
#include "firmware.h"

void bxnDeviceInit(void);

static Bitu bxnIOSys(void);
static void createInfoBlock(Bit32u cbAddr);

/*
 * Initialize the BoxOn I/O System
 */
void boxonIOInit(const char *rc)
{
	int cbNo;
	Bit32u cbAddr;

	// Create a callback service
	cbNo = CALLBACK_Allocate();
	CALLBACK_Setup(cbNo, bxnIOSys, CB_RETN, "BoxOnIO service handler");
	cbAddr = CALLBACK_GetAddr(cbNo);
	createInfoBlock(cbAddr);

	// Enable protected mode
	DescriptorTable gdt(GDT_BASE, GDT_LIMIT);
	DescriptorTable idt(IDT_BASE, IDT_LIMIT);
	boxEnablePmode(gdt, idt);

	bxnDeviceInit();
}

/*
 * Set the BoxOn IO System info block
 */
static void createInfoBlock(Bit32u cbAddr)
{
	BoxOnInfoBlock *ib;

	ib = new BoxOnInfoBlock();
	ib->magic[0] = BOXONIO_MAGIC_0;
	ib->magic[1] = BOXONIO_MAGIC_1;
	ib->version = BOXONIO_VERSION;
	ib->entryAddr = cbAddr;
	ib->checksum = 0 - (ib->magic[0] + ib->magic[1] + ib->version + ib->entryAddr);

	boxMemcpy(FIRMWARE_INFO_BASE, ib, sizeof(BoxOnInfoBlock));
	reg_esi = FIRMWARE_INFO_BASE;

	delete[] ib;
}

/*
 * BoxOn IO System service handler
 */
static Bitu bxnIOSys(void)
{
	DeviceControl *dev;

	// Clear errors
	errno = 0;
	CALLBACK_SCF(false);

	// Find device
	dev = getDeviceControl(reg_ebx);
	if(errno){
		error(errno);
	}

	// Find service
	switch(reg_eax){

	case BXN_OPEN:
		dev->open(reg_eax, reg_ebx, reg_ecx, reg_edx);
		if(errno){
			error(errno);
		}
		break;

	case BXN_CLOSE:
		dev->close(reg_eax, reg_ebx, reg_ecx, reg_edx);
		if(errno){
			error(errno);
		}
		break;

	case BXN_READ:
		dev->read(reg_eax, reg_ebx, reg_ecx, reg_edx);
		if(errno){
			error(errno);
		}
		break;

	case BXN_WRITE:
		dev->write(reg_eax, reg_ebx, reg_ecx, reg_edx);
		if(errno){
			error(errno);
		}
		break;

	default:{
		if(reg_eax < BXN_IOCTL || reg_eax >= BXN_IO_RESERVED){
			LOG_MSG("BoxOnIO: Unknown service 0x%08x", reg_eax);
			reg_eax = -ENOSYS;
			CALLBACK_SCF(true);
		}
		dev->ioctl(reg_eax, reg_ebx, reg_ecx, reg_edx);
		if(errno){
			error(errno);
		}
	}};

	return CBRET_NONE;

Error:
	CALLBACK_SCF(true);
	reg_eax = -errno;
	return CBRET_NONE;
}


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
#include "bionix/sys/bionix.h"
#include "callback.h"
#include "firmware.h"

static Bitu bionixSys(void);
static void createInfoBlock(Bit32u cbAddr);

/*
 * Initialize bionix
 */
void bionixInit(const char *rc)
{
	int cbNo;
	Bit32u cbAddr;

	// Create a callback service
	cbNo = CALLBACK_Allocate();
	CALLBACK_Setup(cbNo, bionixSys, CB_RETN, "bionix service handler");
	cbAddr = CALLBACK_GetAddr(cbNo);
	createInfoBlock(cbAddr);

	// Enable protected mode
	DescriptorTable gdt(GDT_BASE, GDT_LIMIT);
	DescriptorTable idt(IDT_BASE, IDT_LIMIT);
	boxEnablePmode(gdt, idt);
}

/*
 * Set the bionix info block
 */
static void createInfoBlock(Bit32u cbAddr)
{
	BionixInfoBlock *info;

	info = new BionixInfoBlock();
	info->magic[0] = BIONIX_MAGIC_0;
	info->magic[1] = BIONIX_MAGIC_1;
	info->version = BIONIX_VERSION;
	info->entryAddr = cbAddr;
	info->checksum = 0 - (info->magic[0] + info->magic[1] + info->version + info->entryAddr);

	boxMemcpy(FIRMWARE_INFO_BASE, info, sizeof(BionixInfoBlock));
	reg_esi = FIRMWARE_INFO_BASE;

	delete[] info;
}

/*
 * bionix service handler
 */
static Bitu bionixSys(void)
{
	// Clear errors
	errno = 0;

	// Find service
	switch(reg_eax){

	case BIONIX_OPEN:
		LOG_MSG("bionix: Open 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	case BIONIX_CLOSE:
		LOG_MSG("bionix: Close 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	case BIONIX_READ:
		LOG_MSG("bionix: Read 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	case BIONIX_WRITE:
		LOG_MSG("bionix: Write 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	case BIONIX_IOCTL:
		LOG_MSG("bionix: I/O ctl 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	default:{
		LOG_MSG("bionix: Unknown service 0x%08x", reg_eax);
		reg_eax = -ENOSYS;
	}};

	return CBRET_NONE;
}

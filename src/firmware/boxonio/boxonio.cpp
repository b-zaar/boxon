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
#include "firmware.h"

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
	// Clear errors
	errno = 0;
	CALLBACK_SCF(false);

	// Find service
	switch(reg_eax){

	case BXN_OPEN:
		LOG_MSG("BoxOnIO: Open 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	case BXN_CLOSE:
		LOG_MSG("BoxOnIO: Close 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	case BXN_READ:
		LOG_MSG("BoxOnIO: Read 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	case BXN_WRITE:
		LOG_MSG("BoxOnIO: Write 0x%08x 0x%08x 0x%08x 0%08x", reg_eax, reg_ebx, reg_ecx, reg_edx);
		break;

	default:{
		LOG_MSG("BoxOnIO: Unknown service 0x%08x", reg_eax);
		reg_eax = -ENOSYS;
		CALLBACK_SCF(true);
	}};

	return CBRET_NONE;
}


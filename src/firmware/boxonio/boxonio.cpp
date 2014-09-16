/*
 *  Copyright (C) 2014  BoxOn
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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


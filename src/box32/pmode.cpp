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

/*
 * Set up protected mode
 *
 * params:
 * 	gdt and idt are descriptor tables with a valid base and limit set
 *
 * return:
 * 	PIC is disabled
 *	GDT contains -
 *		0x00: Null descriptor
 *		0x08: 4GB code descriptor
 *		0x10: 4GB data descriptor
 *
 */
void boxEnablePmode(DescriptorTable &gdt, DescriptorTable &idt)
{
	int i;
	Descriptor *desc;

	// Clear IDT memory - Leave empty
	for(i = 0; i < idt.GetLimit(); i += 4){
		mem_writed(idt.GetBase() + i, 0);
	}

	// Clear GDT memory
	for(i = 0; i < gdt.GetLimit(); i += 4){
		mem_writed(gdt.GetBase() + i, 0);
	}

	// GDT Null descriptor
	desc = new Descriptor(0, 0, DESC_BIG, 0);
	gdt.SetDescriptor(0, *desc);
	delete desc;

	// 4GB Code descriptor
	desc = new Descriptor(0, 0xffffffff, DESC_CODE_R_NC_NA | DESC_P | DESC_D32, 0);
	gdt.SetDescriptor(0x08, *desc);
	delete desc;

	// 4GB Data descriptor
	desc = new Descriptor(0, 0xffffffff, DESC_DATA_EU_RW_NA | DESC_P | DESC_BIG, 0);
	gdt.SetDescriptor(0x10, *desc);
	delete desc;

	// Enable pmode
	IO_Write(0x21,0xff);		// Disable PIC
	IO_Write(0xA1,0xff);
	MEM_A20_Enable(true);
	CPU_LGDT(gdt.GetLimit(), gdt.GetBase());
	CPU_LIDT(idt.GetLimit(), idt.GetBase());
	CPU_SET_CRX(0, CPU_GET_CRX(0) | 1);
	CPU_SetSegGeneral(ds, 0x10);
	CPU_SetSegGeneral(es, 0x10);
	CPU_SetSegGeneral(fs, 0x10);
	CPU_SetSegGeneral(gs, 0x10);
	CPU_SetSegGeneral(ss, 0x10);
	CPU_JMP(false, 0x08, 0x7c00, 0);
	CPU_SetFlags(0, FMASK_ALL);
}

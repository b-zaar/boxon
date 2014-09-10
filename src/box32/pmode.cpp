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

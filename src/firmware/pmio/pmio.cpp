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
#include "callback.h"

#define GDT_BASE	0x1000
#define	GDT_LIMIT	0x0fff
#define IDT_BASE	0x0800
#define IDT_LIMIT	0x07ff

/*
 * PMIO service handler
 */
static Bitu PMIO_Services(void)
{
	LOG_MSG("PMIO Service called");
}

/*
 * Protected Mode I/O Layer
 */
void PMIO_Init(const char *rc)
{
	int cbNo;

	// Create a callback service
	cbNo = CALLBACK_Allocate();
	CALLBACK_Setup(cbNo, PMIO_Services, CB_RETF, "PMIO service handler");

	// Enable protected mode
	DescriptorTable gdt(GDT_BASE, GDT_LIMIT);
	DescriptorTable idt(IDT_BASE, IDT_LIMIT);
	boxEnablePmode(gdt, idt);

	LOG_MSG("PMIO enabled");
}

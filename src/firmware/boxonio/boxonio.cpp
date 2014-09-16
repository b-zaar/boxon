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
#define GDT_LIMIT	0x0fff
#define IDT_BASE	0x0800
#define IDT_LIMIT	0x07ff

static Bitu bxnIOSys(void);

/*
 * Initialise the BoxOn I/O System
 */
void boxonIOInit(const char *rc)
{
	int cbNo;

	// Create a callback service
	cbNo = CALLBACK_Allocate();
	CALLBACK_Setup(cbNo, bxnIOSys, CB_RETN, "BoxOnIO service handler");

	// Enable protected mode
	DescriptorTable gdt(GDT_BASE, GDT_LIMIT);
	DescriptorTable idt(IDT_BASE, IDT_LIMIT);
	boxEnablePmode(gdt, idt);

	LOG_MSG("PMIO enabled");
}

/*
 * BoxOn IO System service handler
 */
static Bitu bxnIOSys(void)
{
	LOG_MSG("BoxOnIO: Unknown service 0x%08x", reg_eax);
}


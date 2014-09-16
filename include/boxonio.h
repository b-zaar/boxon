/*
 *  Copyright (C) 2014  venomDev
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

#ifndef	_BOXONIO_H
#define	_BOXONIO_H

#define BOXONIO_VERSION	0x00000001
#define BOXONIO_MAGIC_0	MC_CONST('B', 'o', 'x', 'O')
#define BOXONIO_MAGIC_1	MC_CONST('n', 'I', 'O', 0)

#define GDT_BASE	0x1000
#define GDT_LIMIT	0x0fff
#define IDT_BASE	0x0800
#define IDT_LIMIT	0x07ff

/*
 * BoxOn I/O System services
 */
enum BoxOnIOServices {
	BXN_OPEN,
	BXN_CLOSE,
	BXN_READ,
	BXN_WRITE
};

/*
 * BoxOn I/O information block
 */
struct BoxOnInfoBlock {
	Bit32u magic[2];
	Bit32u version;
	Bit32u entryAddr;
	Bit32u checksum;
	Bit32u extensionCnt;
};

#endif		// _BXNIOSYS_H

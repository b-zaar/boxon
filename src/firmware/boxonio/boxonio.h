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

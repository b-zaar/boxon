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

#ifndef	BXNIO_H
#define	BXNIO_H

#include "boxonio/bxndevice.h"

// Define a  multi-character constant
#ifndef MC_CONST
#define MC_CONST(a, b, c, d)	((d << 24) | (c << 16) | (b << 8) | (a))
#endif

#define BOXONIO_VERSION	0x00000001
#define BOXONIO_MAGIC_0	MC_CONST('B', 'o', 'x', 'O')
#define BOXONIO_MAGIC_1	MC_CONST('n', 'I', 'O', ' ')

/*
 * BoxOn I/O System services
 */
enum BoxOnIOServices {
	// BoxOnIO system services
	BXNIO_OPEN 		= 0,
	BXNIO_INIT 		= 0,
	BXNIO_CLOSE 		= 1,
	BXNIO_EXIT 		= 1,
	BXNIO_READ,
	BXNIO_WRITE,

	// BoxOnIO common device services
	BXNIO_CTL 		= 0x01000,
	BXNIO_SET_MODE		= BXNIO_CTL,
	BXNIO_GET_INFO,

	// BoxOnIO custom device services
	BXNIO_CUSTOM		= 0x08000,

	// BoxOnIO experimental device services
	BXNIO_EXPERIMENT	= 0x0f000,

	BXNIO_RESERVED 		= 0x10000
};

/*
 * BoxOn I/O information block
 */
struct BoxOnInfoBlock {
	uint32_t magic[2];
	uint32_t version;
	uint32_t entryAddr;
	uint32_t checksum;
	uint32_t extensionCnt;
};

#endif		// BXNIO_H

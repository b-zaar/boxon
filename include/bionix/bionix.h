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

#ifndef BIONIX_H
#define BIONIX_H

// Define a  multi-character constant
#ifndef MC_CONST
#define MC_CONST(a, b, c, d)	((d << 24) | (c << 16) | (b << 8) | (a))
#endif

#define BIONIX_VERSION	0x00000001
#define BIONIX_MAGIC_0	MC_CONST('b', 'i', 'o', 'n')
#define BIONIX_MAGIC_1	MC_CONST('i', 'x', ' ', ' ')

/*
 * bionix services
 */
enum BionixServices {
	BIONIX_OPEN	= 0,
	BIONIX_CLOSE,
	BIONIX_READ,
	BIONIX_WRITE,
	BIONIX_IOCTL
};

/*
 * bionix I/O information block
 */
struct BionixInfoBlock {
	uint32_t magic[2];
	uint32_t version;
	uint32_t entryAddr;
	uint32_t checksum;
	uint32_t extensionCnt;
};

#endif	// BIONIX_H

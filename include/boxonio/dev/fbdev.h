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

#ifndef _FBDEV_H
#define _FBDEV_H

#include "boxonio/bxndevice.h"

/*
 * Frame buffer mode/attribute flags
 */
// Bits Per Pixel
#define FB_1BPP		0x00000001
#define FB_2BPP		0x00000002
#define FB_4BPP		0x00000004
#define FB_8BPP		0x00000008
#define FB_15BPP	0x0000000f
#define FB_16BPP	0x00000010
#define FB_32BPP	0x00000020

// Text or graphics mode
#define FB_TEXT		0x00000000
#define FB_GFX		0x00000100

// Extended attributes
#define FB_MODE_CUSTOM	0x00000800
#define FB_MEM_LINEAR	0x00004000
#define FB_MEM_PRESERVE	0x00008000
#define FB_DAC_8BIT	0x00010000
#define FB_VGA_COMP	0x00020000
#define FB_RAMDAC_BLANK	0x00040000
#define FB_MODE_COLOR	0x00080000
#define FB_HARD_STEREO	0x00100000
#define FB_EVC_STEREO	0x00200000
#define FB_MEM_WINDOW	0x00400000
#define FB_DOUBLE_SCAN	0x01000000
#define FB_INTERLACED	0x02000000
#define FB_TRIPLE_BUF	0x04000000
#define FB_DUAL_DISPLAY	0x10000000

/*
 * Frame buffer information blocks
 */
#define FB_INFO		0x00001000
#define FB_INFO_DEVICE	FB_INFO
#define FB_INFO_MODELIST \
			(0x001 | FB_INFO)

/*
 * Fb device info block
 */
struct FbInfoDevice{
	DEV_INFO_HEAD;
	char devStr[80];
	char vendorStr[80];
	uint32_t modeCnt;
	uint32_t totalMemSz;
	uint32_t linearMemSz;
	uint32_t linearMemBase;
};

/*
 * Fb mode attributes
 */
struct FbMode{
	uint16_t gfxResX;
	uint16_t gfxResY;
	uint16_t textResX;
	uint16_t textResY;
	uint32_t flags;
};

/*
 * Fb Mode list info
 */
struct FbInfoModelist{
	DEV_INFO_HEAD;
	uint32_t modeCnt;
	struct FbMode modeList[1];
};

#endif	// _FBDEV_H

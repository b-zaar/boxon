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

#include <errno.h>

#include "boxon.h"
#include "boxonio.h"
#include "fbdevctl.h"
#include "ints/int10.h"

/*
 * Video modes supported by the BoxOn frame buffer
 */
struct FbMode modeList[] = {
	// id	gx	gy	tx	ty	flags
	{0x000,	360,	400,	40,	25,	FB_TEXT | FB_4BPP},
	{0x001,	360,	400,	40,	25,	FB_TEXT | FB_4BPP},
	{0X002,	720,	400,	80,	25,	FB_TEXT | FB_4BPP},
	{0x003,	720,	400,	80,	25,	FB_TEXT | FB_4BPP},
	{0x004,	320,	200,	40,	25,	FB_GFX | FB_2BPP},
	{0x005,	320,	200,	40,	25,	FB_GFX | FB_2BPP},
	{0x006,	640,	200,	80,	25,	FB_GFX | FB_1BPP},
	{0x007,	720,	400,	80,	25,	FB_TEXT | FB_1BPP},

	{0x00D,	320,	200,	40,	25,	FB_GFX | FB_4BPP},
	{0x00E,	640,	200,	80,	25,	FB_GFX | FB_4BPP},
	{0x00F,	640,	350,	80,	25,	FB_GFX | FB_1BPP},
	{0x010,	640,	350,	80,	25,	FB_GFX | FB_4BPP},
	{0x011,	640,	480,	80,	30,	FB_GFX | FB_1BPP},
	{0x012,	640,	480,	80,	30,	FB_GFX | FB_4BPP},
	{0x013,	320,	200,	40,	25,	FB_GFX | FB_8BPP},

	{0x054,	1056,	344,	132,	43,	FB_TEXT | FB_8BPP},
	{0x055,	1056,	400,	132,	25,	FB_TEXT | FB_8BPP},

	{0x069,	640,	480,	80,	30,	FB_GFX | FB_8BPP},
	{0x06A,	800,	600,	100,	37,	FB_GFX | FB_4BPP},

	{0x100,	640,	400,	80,	25,	FB_GFX | FB_8BPP},
	{0x101,	640,	480,	80,	30,	FB_GFX | FB_8BPP},
	{0x102,	800,	600,	100,	37,	FB_GFX | FB_4BPP},
	{0x103,	800,	600,	100,	37,	FB_GFX | FB_8BPP},
	{0x104,	1024,	768,	128,	48,	FB_GFX | FB_4BPP},
	{0x105,	1024,	768,	128,	48,	FB_GFX | FB_8BPP},
	{0x106,	1280,	1024,	160,	64,	FB_GFX | FB_4BPP},
	{0x107,	1280,	1024,	160,	64,	FB_GFX | FB_8BPP},
	{0x108,	640,	480,	80,	60,	FB_TEXT | FB_8BPP},
	{0x109,	1056,	400,	132,	25,	FB_TEXT | FB_8BPP},
	{0x10A,	1056,	688,	132,	43,	FB_TEXT | FB_8BPP},
	{0x10B,	1056,	400,	132,	50,	FB_TEXT | FB_8BPP},
	{0x10C,	1056,	480,	132,	60,	FB_TEXT | FB_8BPP},
	{0x10D,	320,	200,	40,	25,	FB_GFX | FB_15BPP},
	{0x10E,	320,	200,	40,	25,	FB_GFX | FB_16BPP},
	{0x10F,	320,	200,	40,	25,	FB_GFX | FB_32BPP},
	{0x110,	640,	480,	80,	30,	FB_GFX | FB_15BPP},
	{0x111,	640,	480,	80,	30,	FB_GFX | FB_16BPP},
	{0x112,	640,	480,	80,	30,	FB_GFX | FB_32BPP},
	{0x113,	800,	600,	100,	37,	FB_GFX | FB_15BPP},
	{0x114,	800,	600,	100,	37,	FB_GFX | FB_16BPP},
	{0x115,	800,	600,	100,	37,	FB_GFX | FB_32BPP},
	{0x116,	1024,	768,	128,	48,	FB_GFX | FB_15BPP},
	{0x117,	1024,	768,	128,	48,	FB_GFX | FB_16BPP},
	{0x118,	1024,	768,	128,	48,	FB_GFX | FB_32BPP},

	{0x150,	320,	200,	40,	25,	FB_GFX | FB_8BPP},
	{0x151,	320,	240,	40,	30,	FB_GFX | FB_8BPP},
	{0x152,	320,	400,	40,	50,	FB_GFX | FB_8BPP},
	{0x153,	320,	480,	40,	60,	FB_GFX | FB_8BPP},

	{0x160,	320,	240,	40,	30,	FB_GFX | FB_15BPP},
	{0x161,	320,	400,	40,	50,	FB_GFX | FB_15BPP},
	{0x162,	320,	480,	40,	60,	FB_GFX | FB_15BPP},
	{0x165,	640,	400,	80,	25,	FB_GFX | FB_15BPP},

	{0x170,	320,	240,	40,	30,	FB_GFX | FB_16BPP},
	{0x171,	320,	400,	40,	50,	FB_GFX | FB_16BPP},
	{0x172,	320,	480,	40,	60,	FB_GFX | FB_16BPP},
	{0x175,	640,	400,	80,	25,	FB_GFX | FB_16BPP},

	{0x190,	320,	240,	40,	30,	FB_GFX | FB_32BPP},
	{0x191,	320,	400,	40,	50,	FB_GFX | FB_32BPP},
	{0x192,	320,	480,	40,	60,	FB_GFX | FB_32BPP},

	{0x207,	1152,	864,	160,	64,	FB_GFX | FB_8BPP},
	{0x209,	1152,	864,	160,	64,	FB_GFX | FB_15BPP},
	{0x20A,	1152,	864,	160,	64,	FB_GFX | FB_16BPP},
	{0x213,	640,	400,	80,	25,	FB_GFX | FB_32BPP},

	// End of list
	{0xFFFF}
};

/*
 * Set the screen mode
 */
static uint32_t bxnfbSetMode(uint32_t &code, uint32_t id, uint32_t flags, uint32_t data)
{
	int resX, resY;
	FbMode *mode;

	// Extract settings
	resX = data >> 16;
	resY = data & 0xffff;
	flags &= FB_FLAGS_MASK;

	// Reset to default text mode
	if(flags == 0 && data == 0){
		if(VESA_SetSVGAMode(0x03) == VESA_FAIL){
			error(EIO);
		}
		return 0;
	}

	// Find a compatible mode
	for(mode = modeList; mode->id != 0xffff; mode++){

		// Search for graphics modes
		if(flags & FB_GFX){
			if(resX == mode->gfxResX && resY == mode->gfxResY && flags == mode->flags){
				break;
			}

		// Search for text modes
		}else{
			if(resX == mode->textResX && resY == mode->textResY && flags == mode->flags){
				break;
			}
		}
	}

	// Mode wasn't found
	if(mode->id == 0xffff){
		error(EINVAL);
	}

	// Set mode
	if(VESA_SetSVGAMode(mode->id) == VESA_FAIL){
		error(EIO);
	}

	return 0;
Error:
	code = -errno;
	return -1;
}

/*
 * BoxOn frame buffer open
 */
static int32_t bxnfbOpen(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	if(flags != 0){
		error(EIO);
	}

	return 0;
Error:
	code = -errno;
	return -1;
}

/*
 * BoxOn frame buffer close
 */
static int32_t bxnfbClose(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	return 0;
}

/*
 * BoxOn frame buffer read
 */
static int32_t bxnfbRead(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	error(EIO)

	return 0;
Error:
	code = -errno;
	return -1;
}

/*
 * BoxOn frame buffer write
 */
static int32_t bxnfbWrite(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	error(EIO)

	return 0;
Error:
	code = -errno;
	return -1;
}

/*
 * BoxOn frame buffer ioctl
 */
static int32_t bxnfbIoctl(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data)
{
	switch(code){

	case BXN_SET_MODE:
		return bxnfbSetMode(code, id, flags, data);

	default:{
		error(ENOSYS)
	}};

	return 0;
Error:
	code = -errno;
	return -1;
}

static FbDevice bxnfbDev={
	bxnfbOpen,
	bxnfbClose,
	bxnfbRead,
	bxnfbWrite,
	bxnfbIoctl
};

/*
 * Initialize the BoxOn frame buffer
 */
FbDevice *bxnfbInit()
{
	// Only install if the S3 card is used
	if(svgaCard != SVGA_S3Trio){
		LOG_MSG("BoxOn frame buffer device requires the setting: machine=svga_s3");
		error(ENODEV);
	}

	return &bxnfbDev;
Error:
	return NULL;
}

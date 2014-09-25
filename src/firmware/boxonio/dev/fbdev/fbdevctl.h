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

#ifndef _FBDEV_CTL_H
#define _FBDEV_CTL_H

#include "boxonio/dev/fbdev.h"

#define FB_FLAGS_MASK	0x000001ff

#define	FB_INFO_BLOCK	0x00001fff

struct FbMode{
	uint32_t	id;
	uint32_t	gfxResX;
	uint32_t	gfxResY;
	uint32_t	textResX;
	uint32_t	textResY;
	uint32_t	flags;
};

struct FbDevice{
	int32_t	(*open)(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data);
	int32_t	(*close)(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data);
	int32_t	(*read)(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data);
	int32_t	(*write)(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data);
	int32_t (*ioctl)(uint32_t &code, uint32_t &id, uint32_t &flags, uint32_t &data);
};

#endif	// _FBDEV_CTL_H

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

// Frame buffer information blocks
#define	FB_INFO		0x00001000

#endif	// _FBDEV_H

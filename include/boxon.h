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

#ifndef _BOXON_H
#define _BOXON_H

#include <cstddef>
#include <cstdarg>
#include "stdint.h"

#include "config.h"
#include "cpu.h"
#include "inout.h"
#include "regs.h"

#define error(err) \
{ \
	errno = err; \
	goto Error; \
}

// Define a  multi-character constant
#define MC_CONST(a, b, c, d)	((d << 24) | (c << 16) | (b << 8) | (a))

#define boxReadB(src)	mem_readb((PhysPt)(src))
#define boxReadW(src)	mem_readw((PhysPt)(src))
#define boxReadD(src)	mem_readd((PhysPt)(src))

#define boxWriteB(dest, src)\
			mem_writeb((PhysPt)(dest), (Bit8u)(src))
#define boxWriteW(dest, src)\
			mem_writew((PhysPt)(dest), (Bit16u)(src))
#define boxWriteD(dest, src)\
			mem_writed((PhysPt)(dest), (Bit32u)(src))

#define format(str, size, fmt, ...)\
{\
	va_list vl;\
	va_start(vl, fmt);\
	vsnprintf(str, size, fmt, vl);\
	va_end(vl);\
}

void boxEnablePmode(DescriptorTable &gdt, DescriptorTable &idt);
int boxStrncpy(Bit32u dest, char *src, size_t cnt);
int boxStrncpy(char *dest, Bit32u src, size_t cnt);
Bit32u boxMemcpy(Bit32u dest, void *src, size_t cnt);
void *boxMemcpy(void *dest, Bit32u src, size_t cnt);

#endif		// _BOXON_H

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

#ifndef _BOXON_H
#define _BOXON_H

#include <cstddef>
#include <cstdarg>

#include "config.h"
#include "cpu.h"
#include "inout.h"
#include "regs.h"

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

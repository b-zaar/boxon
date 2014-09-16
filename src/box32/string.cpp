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

#include "boxon.h"

/*
 * Copy a range of memory to the box, limit to cnt
 */
Bit32u boxMemcpy(Bit32u dest, void *src, size_t cnt)
{
	char *s;
	Bit32u d;

	d = dest;
	s = (char *)src;
	for(; cnt >= 4; cnt -= 4, d += 4, s += 4){
		boxWriteD(d, *(Bit32u *)s);
	}
	for(; cnt; cnt--, d++, s++){
		boxWriteB(d, *s);
	}
	return dest;
}

/*
 * Copy a range of memory from the box, limit to cnt
 */
void *boxMemcpy(void *dest, Bit32u src, size_t cnt)
{
	char *d;

	d = (char *)dest;
	for(; cnt >= 4; cnt -= 4, d += 4, src += 4){
		*(Bit32u *)d = boxReadD(src);
	}
	for(; cnt; cnt--, d++, src++){
		*d = boxReadB(src);
	}
	return dest;
}

/*
 * Copy a string to the the box, limit to cnt
 */
int boxStrncpy(Bit32u dest, char *src, size_t cnt)
{
	int i, p;
	int d;

	// Copy string
	p = 0;
	d = dest;
	for(i = 0; i < cnt; i++, d++, src++){
		boxWriteB(d, *src);
		if(*src == '\0'){

			// Pad til end
			for(p = i; p < cnt; p++, d++){
				boxWriteB(d, '\0');
			}
			break;
		}
	}

	// No terminating '\0' found
	if(p == 0){
		// For safety
		boxWriteB(d + cnt - 1, '\0');
	}
	return i;
}

/*
 * Copy a string from the box, limit to cnt
 */
int boxStrncpy(char *dest, Bit32u src, size_t cnt)
{
	int i, p;
	char *c;

	// Copy string
	c = dest;
	for(i = 0; i < cnt; i++, c++){
		*c = boxReadB(src + i);
		if(*c == '\0'){

			// Pad til end
			for(p = i; p < cnt; p++, c++){
				*c = '\0';
			}
			break;
		}
	}

	// No terminating '\0' found
	if(*c != '\0'){
		// For safety
		*(dest + cnt - 1) = '\0';
	}
	return i;
}

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
#include "fbdevctl.h"

/*
 * Video modes supported by the BoxOn frame buffer
 */
struct FbModes modeList[] = {};

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
	error(ENOSYS)

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

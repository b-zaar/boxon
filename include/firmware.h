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

#ifndef	_FIRMWARE_H
#define	_FIRMWARE_H

#include "setup.h"

#define FIRMWARE_INFO_BASE	0x88000

#define	MSG_MAX_LEN		1024

// Firmware module
class FIRMWARE:public Module_base {
public:
	FIRMWARE(Section *sec): Module_base(sec) {};
	const char *propString(const char *prop);

private:
};

// Firmware System control
class FirmwareSystem {
public:
	FirmwareSystem(std::string name, void (*init)(const char *), void (*boot)(const char *)):
		Name(name), Init(init), Boot(boot) {};
	void init(const char *);
	void boot(const char *);
	std::string name();

private:
	std::string Name;
	void (*Boot)(const char *);
	void (*Init)(const char *);
};

#endif		// _FIRMWARE_H

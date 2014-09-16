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

#include <cstring>
#include <fstream>
#include "boxon.h"

#include "bios.h"
#include "multiboot.h"
#include "../ints/int10.h"

#define BUF_SZ	1024

#define MEM_SIZE	0x2000
#define MB_INFO_BASE	0x0008000
#define MOD_LIMIT	5

// Multiboot supported required options
#define MULTIBOOT_FLAGS_SUPPORTED	(MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO)	// | MULTIBOOT_VIDEO_MODE)
#define MULTIBOOT_FLAGS_REQUIRED	(0xffff)

bool loadMod(std::string &modName);

static int loadAddr;
static multiboot_header *mbHeader = NULL;
static multiboot_mod_list *modList[MOD_LIMIT];
static int modCnt = 0;

void createInfoblock();
void writeOut(const char *str, ...);

/*
 * Simple Multiboot boot loader
 */
void SMB_Boot(const char *rc)
{
	std::ifstream rcFile;
	std::string modName;

	INT10_SetVideoMode(0x03);
	writeOut("BoxOn Simple Boot Loader\n\n");

	// Load the rc file
	rcFile.open(rc);
	if(!rcFile.is_open()){
		writeOut("Error opening configuration file: %s\n", rc);
		return;
	}

	// Load each module
	loadAddr = 0x100000;
	modCnt = 0;
	while(getline(rcFile, modName) && modCnt < MOD_LIMIT){

		// Error loading module
		if(!loadMod(modName)){
			writeOut("Error loading module: %s\n", modName.c_str());
			break;
		}

		// Not a multiboot kernel
		if(modCnt == 0 && !mbHeader){
			writeOut("First module was not a supported Multiboot kernel\n");
			break;
		}

		// Show module info
		modCnt++;
		std::string name = modName.length() <= 35 ? modName :
			"..." + modName.substr(modName.length() - 32);
		writeOut("Start: 0x%08x  End: 0x%08x  Module: %-35s\n", modList[modCnt - 1]->mod_start,
			modList[modCnt - 1]->mod_end, name.c_str());
	}
	rcFile.close();

	// Setup a Multiboot information block
	if(mbHeader){
		createInfoblock();
		reg_eax = MULTIBOOT_BOOTLOADER_MAGIC;
		reg_ebx = MB_INFO_BASE;
		reg_esp = MB_INFO_BASE;
		reg_eip = mbHeader->entry_addr;
		writeOut("\nStarting kernel...\n\n");
	}
}

/*
 * Check for a multiboot header
 *
 * params:
 * 	mbImage	= The multiboot image to check
 *
 * return:
 *	loadAddr
 *		= Multiboot kernel load address
 *	int	= Start of the kernel image
 */
int checkMultiboot(char *mbImage)
{
	int imgOff;
	multiboot_header *mb;

	// Only check the first module
	if(modCnt){
		return 0;
	}

	// Search for Multiboot header
	for(imgOff = 0; imgOff < MULTIBOOT_SEARCH; imgOff += MULTIBOOT_HEADER_ALIGN){
		mb = (struct multiboot_header *)&mbImage[imgOff];

		// Magic found
		if(mb->magic == MULTIBOOT_HEADER_MAGIC){
			uint32_t chk = mb->magic + mb->flags + mb->checksum;

			// Valid checksum
			if(chk == 0){
				LOG_MSG("SMB_Boot found multiboot header at: 0x%08x", imgOff);
				break;
			}
		}
	}

	// No Multiboot header found
	if(mb->magic != MULTIBOOT_HEADER_MAGIC){
		return 0;
	}

	// Check for unsupported required flags
	if((mb->flags & MULTIBOOT_FLAGS_REQUIRED) & ~MULTIBOOT_FLAGS_SUPPORTED){
		writeOut("Some required Multiboot flags are not supported\n");
		return 0;
	}

	// Valid address fields
	int imgStart, imgSize, bssStart, bssSize;
	if(mb->flags & MULTIBOOT_AOUT_KLUDGE){
		loadAddr = mb->load_addr;
		imgStart = imgOff - (mb->header_addr - mb->load_addr);
		imgSize = mb->load_end_addr - mb->load_addr;
		bssStart = mb->load_end_addr;
		if(mb->bss_end_addr){
			bssSize = mb->bss_end_addr - mb->load_end_addr;
		}else{
			bssSize = 0;
		}

		LOG_MSG("  Load Address: 0x%08x", loadAddr);
		LOG_MSG("   Image Start: 0x%08x", imgStart);
		LOG_MSG("    Image Size: 0x%08x", imgSize);
		LOG_MSG("    Entry addr: 0x%08x", mb->entry_addr);
		LOG_MSG("     BSS Start: 0x%08x", bssStart);
		LOG_MSG("      BSS Size: 0x%08x", bssSize);


	// Use ELF address fields
	}else{
		writeOut("ELF header not supported\n");
		return 0;
	}

	// Clear bss
	for(int i = 0; i < bssSize; i++){
		mem_writeb(bssStart + i, 0);
	}

	// Save multiboot header
	mbHeader = new multiboot_header;
	memcpy(mbHeader, mb, sizeof(multiboot_header));

	return imgStart;
}

/*
 * Load a boot module
 */
bool loadMod(std::string &modName)
{
	int i;
	char *rdBuf;
	std::ifstream modFile;
	multiboot_mod_list *modBlock;

	// Open module
	modFile.open(modName.c_str(), std::ifstream::binary);
	if(!modFile.is_open()){
		return false;
	}

	// Check for multiboot struct
	rdBuf = new char[MEM_SIZE];
	modFile.readsome(rdBuf, MEM_SIZE);
	int imgStart = checkMultiboot(rdBuf);
	if(imgStart){
		// Reset image start
		modFile.seekg(imgStart);
		modFile.readsome(rdBuf, MEM_SIZE);
	}

	// Load module
	modBlock = new multiboot_mod_list();
	modList[modCnt] = modBlock;
	modBlock->mod_start = loadAddr;
	do{
		for(i = 0; i < modFile.gcount(); i++){
			mem_writeb(loadAddr, *(rdBuf + i));
			loadAddr++;
		}
		modFile.readsome(rdBuf, MEM_SIZE);
	}while(modFile.gcount());

	// Align load address to next page
	modBlock->mod_end = loadAddr;
	loadAddr = (loadAddr + 0xfff) & 0xfffff000;

	delete[] rdBuf;
	return true;
}

// BIOS custom service
Bit16u extendedMemSize();

/*
 * Create a Multiboot information block
 */
void createInfoblock()
{
	int i, freeMem;
	multiboot_info *mbInfo;

	// Create block and next free mem pointer
	mbInfo = new multiboot_info();
	freeMem = MB_INFO_BASE + sizeof(multiboot_info);

	// Set memory limits
	mbInfo->flags |= MULTIBOOT_INFO_MEMORY;
	mbInfo->mem_lower = mem_readw(BIOS_MEMORY_SIZE);
	mbInfo->mem_upper = extendedMemSize();

	// Set modules list
	if(modCnt > 1){
		mbInfo->flags |= MULTIBOOT_INFO_MODS;
		mbInfo->mods_count = modCnt - 1;
		mbInfo->mods_addr = freeMem;

		for(i = 1; i < modCnt; i++){
			mem_writed(freeMem, modList[i]->mod_start);
			mem_writed(freeMem + 4, modList[i]->mod_end);
			mem_writed(freeMem + 8, modList[i]->cmdline);
			mem_writed(freeMem + 12, 0);
			freeMem += sizeof(multiboot_mod_list);
		}
	}

	// Write info block to memory
	char *info = (char *)mbInfo;
	for(i = 0; i < sizeof(multiboot_info); i++){
		mem_writeb(MB_INFO_BASE + i, *(info + i));
	}
}

/*
 * Write a character in Teletype mode
 */
void putChar(const char c)
{
	INT10_TeletypeOutput(c, 0x07);
}

/*
 * Write a string with format information to the console
 */
void writeOut(const char *str, ...)
{
	char *c, *s;

	c = s = new char[BUF_SZ];
	format(s, BUF_SZ, str);
	for(; *c; c++){
		switch(*c){
		case '\n':
			putChar('\r');
			putChar('\n');
			break;
		default:
			putChar(*c);
		}
	}
	delete[] s;
}

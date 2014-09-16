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
#include "control.h"
#include "cross.h"
#include "firmware.h"

static FIRMWARE *firmware;

char **initFirmwareList();
FirmwareSystem *findFirmware(const char *name);
void bxnIOSysInit();

static void FIRMWARE_Config(Section *config);

/*
 * Initialize config file settings
 */
void FIRMWARE_Init(void)
{
	char **list;
	std::string config_path, rc_file;
	Section_prop *secprop;
	Section_line *secline;
	Prop_int *Pint;
	Prop_hex *Phex;
	Prop_string *Pstring;
	Prop_bool *Pbool;
	Prop_multival *Pmulti;
	Prop_multival_remain *Pmulti_remain;

	/* Configuration file directory */
	Cross::GetPlatformConfigDir(config_path);

	// Create list of supported firmware systems
	list = initFirmwareList();

	// Setup firmware config file options
	secprop=control->AddSection_prop("firmware", &FIRMWARE_Config);

	// System type to use
	Pstring = secprop->Add_string("system", Property::Changeable::OnlyAtStart, list[0]);
	Pstring->Set_values(list);
	Pstring->Set_help("Select the firmware to use.");

	// System configuration file
	rc_file = config_path + "sys.rc";
	Pstring = secprop->Add_path("sysrc", Property::Changeable::OnlyAtStart, rc_file.c_str());
	Pstring->Set_help("Path to the system configuration file.");

	// Boot rc file
	rc_file = config_path + "boot.rc";
	Pstring = secprop->Add_path("bootrc", Property::Changeable::OnlyAtStart, rc_file.c_str());
	Pstring->Set_help("Path to the boot configuration file.");
}

/*
 * Configure the firmware module
 */
static void FIRMWARE_Config(Section *config)
{
	firmware = new FIRMWARE(config);
}

/*
 * Boot the system
 */
void FIRMWARE_Boot()
{
	FirmwareSystem *fwSys;

	if((fwSys = findFirmware(firmware->propString("system"))) == NULL){
		E_Exit("Firmware: Could not locate firmware system: %s", firmware->propString("system"));
	}

	fwSys->init(firmware->propString("sysrc"));
	fwSys->boot(firmware->propString("bootrc"));
}

/*
 * Get the firmware name
 */
std::string FirmwareSystem::name(void)
{
	return Name;
}

/*
 * Run the boot loader
 */
void FirmwareSystem::boot(const char *bootrc)
{
	(*Boot)(bootrc);
}

/*
 * Initialize the firmware
 */
void FirmwareSystem::init(const char *sysrc)
{
	(*Init)(sysrc);
}

/*
 * Get a property string
 */
const char *FIRMWARE::propString(const char *prop)
{
	Section_prop *section = static_cast<Section_prop *>(m_configuration);

	return section->Get_string(prop);
}

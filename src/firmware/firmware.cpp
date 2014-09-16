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

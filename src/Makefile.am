AM_CPPFLAGS = -I$(top_srcdir)/include

SUBDIRS = cpu debug dos fpu gui hardware libs ints misc shell firmware box32 platform

bin_PROGRAMS = boxon

if HAVE_WINDRES
ico_stuff = winres.rc
endif

.rc.o:
	$(WINDRES) -o $@ $<

boxon_SOURCES = dosbox.cpp $(ico_stuff)
boxon_LDADD = cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui.a \
               ints/libints.a misc/libmisc.a shell/libshell.a hardware/serialport/libserial.a libs/gui_tk/libgui_tk.a \
	       firmware/libfirmware.a firmware/bionix/libfw_bionix.a \
	       box32/libbox32.a

EXTRA_DIST = winres.rc dosbox.ico



# SPDX-FileCopyrightText: 2025 Linblow <dev@linblow.com>
# SPDX-License-Identifier: BSD-3-Clause
# -----
# Availaible rules: all (default), clean, distclean, dist, install 
# -----

# Path to the actual PPSSPP memstick directory on the system.
# For quick deployment and testing via the 'install' rule.
ppsspp_memstick_dir = /mnt/c/Users/bro/Documents/PPSSPP

# List of game IDs this plugin will be enabled for (plugin.ini file)
APP_RUN_FOR   = UCES00543
APP_NAME      = bootFixFtb2Eu
TARGET        = $(APP_NAME)-Linblow
APP_VER_MAJOR = 1
APP_VER_MINOR = 1

# CURDIR = $(shell pwd)

# No need to change these.
memstickdir = dist/ms0
plugindir   = $(memstickdir)/PSP/PLUGINS/$(TARGET)
zipname     = $(APP_NAME)Plugin.zip

OBJS = printf/src/printf/printf.o \
 	   src/exports.c \
 	   src/vdprintf.o \
	   src/main.o 

INCDIR = printf/src src
LIBDIR =
LIBS = 
USE_KERNEL_LIBS = 1

DEFINES = -DAPP_NAME=\"$(APP_NAME)\" \
		  -DAPP_VER_MAJOR=$(APP_VER_MAJOR) \
		  -DAPP_VER_MINOR=$(APP_VER_MINOR) \
		  -DPRINTF_SUPPORT_LONG_LONG=0 \
		  -DPRINTF_SUPPORT_DECIMAL_SPECIFIERS=0 \
		  -DPRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS=0 \
		  -DPRINTF_ALIAS_STANDARD_FUNCTION_NAMES_HARD=1

CFLAGS = -Os -Wall -Wno-main -fno-builtin-printf $(DEFINES)
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

.PHONY: dist install force_remake

$(memstickdir) $(plugindir):
	mkdir -p "$@"

# Create the "plugin.ini" file:
version = $(shell printf '%d%02d' $(APP_VER_MAJOR) $(APP_VER_MINOR))
plugin_ini = $(plugindir)/plugin.ini
$(plugin_ini): $(plugindir) force_remake
	@echo "[options]" > $@
	@echo "version = $(version)" >> $@
	@echo "type = prx" >> $@
	@echo "filename = $(TARGET).prx" >> $@
	@echo "" >> $@
	@echo "[games]" >> $@
ifeq ($(strip $(APP_RUN_FOR)),)
	@echo "ALL = true" >> $@
else
	@$(foreach id,$(APP_RUN_FOR),echo "$(id) = true" >> $@;)
endif

dist: all $(memstickdir) $(plugindir) $(plugin_ini) README.md
	cp -f $(TARGET).prx README.md "$(plugindir)/"
	cd "$(memstickdir)" && zip -r ../$(zipname) .

distclean:
	rm -f dist/$(zipname)
	rm -rf "$(memstickdir)"

install: dist
ifeq ($(strip $(ppsspp_memstick_dir)),)
	@echo "Error: ppsspp_memstick_dir variable is not set or empty, cannot install"
	@false
else
	cp -rf $(memstickdir)/* "$(ppsspp_memstick_dir)/"
endif

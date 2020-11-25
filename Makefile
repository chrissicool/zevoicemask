#
# platform-io handles all dependencies for us.
# This makefile is only a shortcut so that common idioms like the
# following work:
#
#  $ make
#  $ make test
#  $ make install

export LC_ALL=C.UTF-8
export LANG=C.UTF-8

.PHONY: all build test install upload flash clean clean-pio3 clean-pio4 release
all: build

ifeq ($(V),1)
VERBOSE = --verbose
endif

ifeq ($(RELEASE),)
PLATFORMIO_BUILD_FLAGS= -DVOICEMASK_DEBUG
endif

PIO= PLATFORMIO_BUILD_FLAGS="$(PLATFORMIO_BUILD_FLAGS)" platformio

TARGET := nodemcuv2 

build:
	$(PIO) run $(addprefix -e ,$(TARGET)) $(VERBOSE)

test:
	$(PIO) test -e native $(VERBOSE)

release: version=$(shell git describe --tags --candidates=1 --dirty --broken)
release:
	$(MAKE) $(MAKE_OPTIONS) test
	$(MAKE) $(MAKE_OPTIONS) RELEASE=true build
	@for t in $(TARGET) ; do \
		cp .pio/build/$${t}/firmware.bin zevoicemask_$${t}_$(version).bin ; \
		tput setaf 2 ; \
		echo ">>> zevoicemask_$${t}_$(version).bin" ; \
		tput sgr0 ; \
	done

install upload flash:
	[ $(words $(TARGET)) -eq 1 ] || exit 1
	$(PIO) run $(addprefix -e ,$(TARGET)) --target upload $(VERBOSE)


clean-pio3:
	rm -rf .pioenvs/ .piolibdeps/

clean-pio4:
	rm -rf .pio/

clean: clean-pio3 clean-pio4

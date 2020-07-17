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

.PHONY: all build test install upload flash clean clean-pio3 clean-pio4
all: build

ifeq ($(V),1)
VERBOSE = --verbose
endif

ifeq ($(RELEASE),)
export PLATFORMIO_BUILD_FLAGS= -DVOICEMASK_DEBUG
endif

TARGET := nodemcuv2 

build:
	platformio run $(addprefix -e ,$(TARGET)) $(VERBOSE)

test: build
	platformio test $(addprefix -e ,$(TARGET)) $(VERBOSE)

install upload flash:
	[ $(words $(TARGET)) -eq 1 ] || exit 1
	platformio run $(addprefix -e ,$(TARGET)) --target upload $(VERBOSE)


clean-pio3:
	rm -rf .pioenvs/ .piolibdeps/

clean-pio4:
	rm -rf .pio/

clean: clean-pio3 clean-pio4

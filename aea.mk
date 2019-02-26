# Define project name here
PROJECT = ch

# Imported source files and paths.
CHIBIOS  := $(HOME)/ChibiOS
CONFDIR  := ./cfg
BUILDDIR := ./build
DEPDIR   := ./.dep

# Dynamically import all source files under ./include
SRCEXT := c
SRCDIR := $(CURDIR)/include
SRCS   := $(shell find $(SRCDIR) -name '*.$(SRCEXT)')
SRCINC := $(shell find $(SRCDIR) -type d)
ALLCSRC += $(SRCS)
ALLINC += $(SRCINC)

# Software level IO configuration
# ./io.c
ALLCSRC += io.c

DEPS_DB := $(CURDIR)/dependencies.txt

CHIBIOS_BRANCH := $(shell cd ${CHIBIOS} && git branch | grep \* | cut -d ' ' -f2)
CHIBIOS_COMMIT := $(shell cd ${CHIBIOS} && git rev-parse HEAD)

.DEFAULT_GOAL := all

PRE_MAKE_ALL_RULE_HOOK:
	@echo "ChibiOS/$(CHIBIOS_BRANCH) $(CHIBIOS_COMMIT)" > $(DEPS_DB)

POST_MAKE_ALL_RULE_HOOK:

CLEAN_RULE_HOOK:
	@echo "Cleanup hook..."
	@echo
	@find $(CURDIR) -iname '*.gch' -exec rm {} \;
	@rm $(CURDIR)/_breakpoints.txt 2> /dev/null || true
	@rm $(CURDIR)/core 2> /dev/null || true

include ./mcu-debug/main.mk

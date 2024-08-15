PROJECTS := hbloader lyrics
OBJS := modldgen.o
DEPS := $(OBJS:.o=.d)

vpath %.c $(dir $(MAKEFILE_LIST))

all: hbloader lyrics

lyrics: hbloader

TARGET_VERSION ?= S16
export TARGET_VERSION

.PHONY: hbloader lyrics
$(PROJECTS): modldgen
	@echo Building $@...
	@$(MAKE) -f $(dir $(MAKEFILE_LIST))$(basename $@)/GNUmakefile

$(addsuffix .clean,$(PROJECTS)):
	@echo Cleaning $(basename $@)...
	@$(MAKE) -f $(dir $(MAKEFILE_LIST))$(basename $@)/GNUmakefile clean

modldgen: $(OBJS)

.PHONY: clean
clean: $(addsuffix .clean,$(PROJECTS))
	$(RM) -rf modldgen $(OBJS) $(DEPS)

-include $(DEPS)

DIRS := $(sort $(wildcard 20*/*))

all: $(DIRS)

$(DIRS):
	make -C $@ $(MODE)


.PHONY: all $(DIRS)


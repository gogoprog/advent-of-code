DIRS := $(sort $(wildcard 20*/*))

all: $(DIRS)

$(DIRS):
	make -C $@ $(MODE)

clean:
	$(foreach dir,$(DIRS),make -C $(dir) clean || exit;)

run-debug:
	$(foreach dir,$(DIRS),make -C $(dir) run-debug || exit;)

run-release:
	$(foreach dir,$(DIRS),make -C $(dir) run-release || exit;)

run-retail:
	$(foreach dir,$(DIRS),make -C $(dir) run-retail || exit;)


.PHONY: all $(DIRS)


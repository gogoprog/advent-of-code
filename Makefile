DIRS := $(sort $(wildcard 20*/*))

list:
	$(foreach dir,$(DIRS),echo $(dir);)

build-debug:
	$(foreach dir,$(DIRS),make -C $(dir) || exit;)

build-release:
	$(foreach dir,$(DIRS),make -C $(dir) release || exit;)
	
build-retail:
	$(foreach dir,$(DIRS),make -C $(dir) retail || exit;)


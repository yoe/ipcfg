RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc
endif

ipcfg: edge.d node.d parser.d main.d path.d
	dmd $(RELEASE) $(DEBUG) -of$@ $^

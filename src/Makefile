RELEASE := -frelease
ifeq ($(RELEASE),)
DEBUG := -g
endif

ipcfg: edge.d node.d parser.d main.d path.d
	gdc $(RELEASE) $(DEBUG) -o $@ $^

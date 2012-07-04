RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w -unittest
endif

DC := gdmd

LIBS=$(shell pkg-config --libs libnl-3.0 libnl-route-3.0)

output/ipcfg: main.o ipcfg/debugout.o ipcfg/edge.o ipcfg/node.o ipcfg/parser.o ipcfg/path.o ipcfg/linux/mii.o ipcfg/linux/rtnetlink_common.o
	[ -d output ] || mkdir output
	$(DC) -of$@ $(addprefix -L,$(LIBS)) $^

ipcfg/linux/%.o: ipcfg/linux/Makefile ipcfg/linux/%.d
	$(MAKE) -C ipcfg/linux $(notdir $@)

ipcfg/%.o: ipcfg/Makefile ipcfg/%.d
	$(MAKE) -C ipcfg $(notdir $@)

%.o: %.d
	$(DC) $(RELEASE) $(DEBUG) -c -of$@ $^

clean: 
	$(MAKE) -C ipcfg/linux clean
	$(MAKE) -C ipcfg clean
	rm -f output/ipcfg *.o

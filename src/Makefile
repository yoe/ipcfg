RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w -unittest
endif

DC := gdmd

output/ipcfg: main.o ipcfg/edge.o ipcfg/node.o ipcfg/parser.o ipcfg/path.o linux/mii.o
	mkdir output
	$(DC) -of$@ -L`pkg-config --libs libnl-3.0` $^

linux/%.o: linux/Makefile linux/%.d
	$(MAKE) -C linux $(notdir $@)

ipcfg/%.o: ipcfg/Makefile ipcfg/%.d
	$(MAKE) -C ipcfg $(notdir $@)

%.o: %.d
	$(DC) $(RELEASE) $(DEBUG) -c -of$@ $^

clean: 
	$(MAKE) -C linux clean
	$(MAKE) -C ipcfg clean
	rm -f output/ipcfg *.o

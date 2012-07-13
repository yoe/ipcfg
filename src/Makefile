RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w -unittest
endif

DC := gdmd

LIBS=$(shell pkg-config --libs libnl-3.0 libnl-route-3.0)

output/ipcfg: ipcfg/debugout.o ipcfg/defaults.o ipcfg/edge.o ipcfg/event.o ipcfg/iface.o ipcfg/node.o ipcfg/graph.o ipcfg/parser.o ipcfg/templates.o ipcfg/linux/mii.o ipcfg/linux/rtnetlink_common.o ipcfg/linux/staticip.o main.o
	[ -d output ] || mkdir output
	$(DC) -of$@ $(addprefix -L,$(LIBS)) -L-L../../Pegged -L-lpegged $^

ipcfg/linux/%.o: ipcfg/linux/Makefile ipcfg/linux/%.d
	$(MAKE) -C ipcfg/linux $(notdir $@)

ipcfg/%.o: ipcfg/Makefile ipcfg/%.d
	$(MAKE) -C ipcfg $(notdir $@)

%.o: %.d
	$(DC) $(RELEASE) $(DEBUG) -I../../Pegged -c -of$@ $^

clean: 
	$(MAKE) -C ipcfg/linux clean
	$(MAKE) -C ipcfg clean
	rm -f output/ipcfg *.o

test: testdefaults.d test.d
	dmd -gc -w -oftest -I../../Pegged -L-L../../Pegged -L-lpegged test.d testdefaults.d

testdefaults.d: parser adddebug.pl
	./parser
	./adddebug.pl < testdefaults.d > f
	mv f testdefaults.d

parser: ipcfg/defaults.d
	dmd -ofparser -I../../Pegged -L-L../../Pegged -L-lpegged -I. ipcfg/defaults.d

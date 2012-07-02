RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w
endif

DC := gdmd

ipcfg: main.d edge.d node.d parser.d path.d
	$(DC) $(RELEASE) $(DEBUG) -of$@ $^

clean:
	rm ipcfg *.o

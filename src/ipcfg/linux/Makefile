RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w -unittest
endif

DC := gdmd

%.o: %.d
	$(DC) $(RELEASE) $(DEBUG) -I../.. -c -of$@ $^


clean:
	rm -f *.o

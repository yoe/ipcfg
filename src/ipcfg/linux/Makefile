RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w -unittest
endif

DC := gdmd

mii.o: mii.d
	$(DC) $(RELEASE) $(DEBUG) -I../.. -c -of$@ $^

clean:
	rm -f *.o

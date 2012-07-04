RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w -unittest
endif

DC := gdmd

all: debugout.o edge.o node.o parser.o path.o

%.o: %.d
	$(DC) $(RELEASE) $(DEBUG) -I.. -c -of$@ $^

clean:
	rm -f *.o

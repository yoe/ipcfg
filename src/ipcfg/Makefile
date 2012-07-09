RELEASE := -release
ifeq ($(RELEASE),)
DEBUG := -gc -w -unittest
endif

DC := gdmd

all: debugout.o edge.o node.o parser.o path.o event.o templates.o defaults.o

%.o: %.d
	$(DC) $(RELEASE) $(DEBUG) -I.. -I../../../Pegged -c -of$@ $^

clean:
	rm -f *.o

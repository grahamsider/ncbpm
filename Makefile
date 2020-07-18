CC=gcc
IDIR=src
ODIR=obj
CFLAGS=-I$(IDIR) -Wall -lncurses

_DEPS=ncbpm.h nchelpers.h ncdefines.h nctime.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=ncbpm.o nchelpers.o nctime.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(IDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ncbpm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f ncbpm $(ODIR)/*.o

CC=gcc
CFLAGS=-I. -Wall -lncurses
DEPS=ncbpm.h
ODIR=obj
_OBJ=ncbpm.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ncbpm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f ncbpm $(ODIR)/*.o

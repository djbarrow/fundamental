#CC=gcc33
CC=gcc
OBJFILES = \
fundamental.o \
utils.o \
print_sum.o \
fundtest.o \
simplify.o
#factor_test.o
#fundtest.o 

CFLAGS= -g -DLINUX 
#CFLAGS=-O2 -g
LINKFLAGS =
LIBS = -lm 



%.o: %.c fundamental.h fundamental_config.h fundamental_sanity.h Makefile
	$(CC) -c $(CFLAGS) -o $@ $<

fundamental: $(OBJFILES)
	$(CC) $(CFLAGS) $(LINKFLAGS) $(OBJFILES) $(LIBS) -o $@

clean:
	rm -f $(OBJFILES) *~ t1 *.i fundamental

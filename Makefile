#CC=gcc33
CC=gcc
OBJFILES = \
fundamental.o \
do_sum.o \
utils.o \
print_sum.o \
fundtest.o \
csv.o \
simplify.o
#fundtest.o 
#factor_test.o


CFLAGS= -g -DLINUX 
#CFLAGS=-O2 -g
LINKFLAGS =
LIBS = -lm 



%.o: %.c fundamental.h fundamental_config.h fundamental_sanity.h do_sum.h Makefile
	$(CC) -c $(CFLAGS) -o $@ $<

fundamental: $(OBJFILES)
	$(CC) $(CFLAGS) $(LINKFLAGS) $(OBJFILES) $(LIBS) -o $@

clean:
	rm -f $(OBJFILES) *~ t1 *.i fundamental

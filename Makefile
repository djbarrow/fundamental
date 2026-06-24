
#CC=gcc33
CC=gcc
#CC=nvcc
#CC=clang-19
OBJFILES = \
fundamental.o \
do_sum.o \
utils.o \
print_sum.o \
fundtest.o \
csv.o \
c-queue/queue.o
#fundtest.o 
#factor_test.o

CPATH=$(CPATH);.
CFLAGS= -g -DLINUX -I/usr/local/cuda/include

#CFLAGS=-O2 -g
LINKFLAGS =
LIBS = -lm #-L/usr/local/cuda/lib64 -lcuda -lcudart

%.o: %.c fundamental.h fundamental_config.h fundamental_sanity.h do_sum.h Makefile
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cu fundamental.h fundamental_config.h fundamental_sanity.h do_sum.h Makefile
	$(CC) -c $(CFLAGS) -o $@ $<


fundamental: $(OBJFILES)
	$(CC) $(CFLAGS) $(LINKFLAGS) $(OBJFILES) $(LIBS) -o $@

clean:
	rm -f $(OBJFILES) *~ t1 *.i fundamental

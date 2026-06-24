ringBuffer: ringBuffer_test.c ringBuffer.c
	gcc -o ringBuffer_test ringBuffer_test.c ringBuffer.c	-I.
clean:
	rm -f ringBuffer_test *.o





## or, for gcc...
#CC=gcc CFLAGS=-O0 -g -Wall

#LD=$(CC)
#LDFLAGS=-g

#test:	ringBuffer_test
#	./ringBuffer_test

##valgrind: ringBuffer_test
##	valgrind ./ringbuf-test

##help:
###	@echo "Targets:"
###	@echo
##	@echo "test  - build and run ringbuf unit tests."
###	@echo "valgrind - use valgrind to check for memory leaks."
##	@echo "clean - remove all targets."
###	@echo "help  - this message."

#ringBuffer_test: ringBuffer_test.o ringBuffer.o
#	$(LD) -o ringBuffer_test $(LDFLAGS) $^

#ringBuffer_test.o: ringBuffer.c ringBuffer_test.c ringBuffer.h
#	$(CC) $(CFLAGS) -c $< -o $@

##ringBuffer.o: ringBuffer.c ringBuffer.h
##	$(CC) $(CFLAGS) -c $< -o $@

#clean:
#	rm -f ringBuffer_test *.o

#.PHONY:	clean

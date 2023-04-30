GC = gcc
FLAGS = -Wall -g

all: cmp copy encode decode stshell

cmp.o: cmp.c
	$(GC) $(FLAGS) -c cmp.c

copy.o: copy.c
	$(GC) $(FLAGS) -c copy.c

encode.o: encode.c
	$(GC) $(FLAGS) -c encode.c -ldl

encode: encode.o
	$(GC) $(FLAGS) -o encode encode.o -ldl

decode.o: decode.c
	$(GC) $(FLAGS) -c decode.c -ldl

decode: decode.o
	$(GC) $(FLAGS) -o decode decode.o -ldl

codecA: codecA.c codecA.h
	$(GC) $(FLAGS) -shared -fPIC -o codecA codecA.c

codecB: codecB.c codecB.h
	$(GC) $(FLAGS) -shared -fPIC -o codecB codecB.c

stshell.o: stshell.c
	$(GC) $(FLAGS) -c stshell.c

stshell: stshell.o
	$(GC) $(FLAGS) -o stshell stshell.o

.PHONY: all clean

clean: 
	rm -f *.o *.so *.a cmp copy encode decode stshell
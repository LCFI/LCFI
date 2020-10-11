CC=clang
LINKER=/chpc/home/gst-bdshan-a/llvm-build/bin/llvm-link
OUTPUT=a.out
CFLAGS=-w -emit-llvm -fno-use-cxa-atexit -S
LINKER_FLAGS=-o $(OUTPUT) -S
SRCDIR_OBJS=demo.ll 

build:$(SRCDIR_OBJS)
	$(LINKER) $(LINKER_FLAGS) $(SRCDIR_OBJS)

demo.ll: demo.c $(PRIMARY_HEADERS)
	$(CC) $(CFLAGS) demo.c

clean:
	rm -rf *.ll *.bc llfi* a.out


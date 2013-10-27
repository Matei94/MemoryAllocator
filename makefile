.PHONY : build clean

build : tema4.c
	gcc -o allocator tema4.c
	
clean :
	rm -f allocator

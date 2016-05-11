all:
	mkdir bin
	gcc -o bin/tagfs src/tagfs.c src/comm.c src/database.c src/parser.c \
	-Iheader -Wall -Wextra -lfuse -D_FILE_OFFSET_BITS=64
	cp bin/tagfs tst-perf/tagfs

mount:
	./bin/tagfs . tst/mnt/

unmount:
	fusermount -uz tst/mnt

clean:
	rm -rf src/*.o bin/* tst-perf/tagfs
	rmdir --ignore-fail-on-non-empty bin

remount: unmount clean all mount

.PHONY: mount unmount clean remount


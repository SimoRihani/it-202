mount:
	./tagfs images mnt -o nonempty

unmount:
	fusermount -uz mnt

remount: unmount mount

mktest:
	gcc -o mnt/tst1 perfs_1tag_Nfichiers.c
	gcc -o mnt/tst2 perfs_Ntags_1fichier.c

test:
	@echo "\n----- BASE TESTS -----\n"
	./check_ls.sh
	./check_modifs.sh
	@echo "\n----- PERF TESTS -----\n"
	./tagfs images mnt -o nonempty
	(cd images ; ./tst1 ; ./tst2)

.PHONY: mount unmount remount test mktest

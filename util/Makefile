all: dumpnoobna nbna_getval test_nbna_getI3 nbna_stats

dumpnoobna: dumpnoobna.c ../noobna.h
	$(CC) dumpnoobna.c -o dumpnoobna

nbna_getval: nbna_getval.c ../noobna.h
	$(CC) nbna_getval.c -o nbna_getval

test_nbna_getI3: test_nbna_getI3.c ../noobna.h
	$(CC) test_nbna_getI3.c -o test_nbna_getI3

nbna_stats: test_nbna_getI3.c ../noobna.h
	$(CC) nbna_stats.c -o nbna_stats

.PHONY: clean all

clean:
	rm -fv dumpnoobna nbna_getval test_nbna_getI3 nbna_stats

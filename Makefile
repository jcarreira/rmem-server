.PHONY: clean

CFLAGS  := -Wall -g -fPIC -std=gnu11
LD      := gcc
LDFLAGS := ${LDFLAGS} -lrdmacm -libverbs -lpthread -std=gnu11
INCLUDE :=-I. -Idata -Iutils

FILES   := utils/log.h common.o rmem_table.o rmem-server.o rvm.o rmem.o data/hash.o data/list.o data/stack.o
APPS    := rmem-server rmem-client rmem-test
TESTS   := tests/rvm_test_normal tests/rvm_test_txn_commit tests/rvm_test_recovery tests/rvm_test_free tests/rvm_test_big_commit tests/rvm_test_size_alloc
#HEADERS := $(wildcard *.h)
SRCS    := $(wildcard *.c)

all: ${APPS} ${TESTS}

include .depend

rmem-server: ${FILES}
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

rmem-client: ${FILES}
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

rmem-test: ${FILES}
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

tests/dgemv_test: tests/dgemv_test.c rmem.o rvm.o rmem_table.o common.o data/hash.o data/list.o buddy_malloc.o
	${LD} -o $@ $^ -lblas ${CFLAGS} ${LDFLAGS} ${INCLUDE} 

tests/rvm_test_normal: tests/rvm_test_normal.c rmem.o rvm.o rmem_table.o common.o data/hash.o data/list.o buddy_malloc.o
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

tests/rvm_test_txn_commit: tests/rvm_test_txn_commit.c rmem.o rvm.o rmem_table.o common.o data/hash.o data/list.o
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

tests/rvm_test_recovery: tests/rvm_test_recovery.c rmem.o rvm.o rmem_table.o common.o data/hash.o data/list.o
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

tests/rvm_test_free: tests/rvm_test_free.c rmem.o rvm.o rmem_table.o common.o data/hash.o data/list.o
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

tests/rvm_test_big_commit: tests/rvm_test_big_commit.c rmem.o rvm.o rmem_table.o common.o data/hash.o data/list.o
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

tests/rvm_test_size_alloc: tests/rvm_test_size_alloc.c rmem.o rvm.o rmem_table.o common.o data/hash.o data/list.o
	${LD} -o $@ $^ ${CFLAGS} ${LDFLAGS} ${INCLUDE}

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ > ./.depend;

clean:
	rm -f *.o ${APPS} ${TESTS}


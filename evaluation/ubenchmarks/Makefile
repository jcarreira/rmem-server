INCLUDES := -I../.. -I../../backends
CFLAGS  := $(INCLUDES) -Wall -O3 -std=gnu11 -pg -g
RVM_LIB := -L../.. -lrvm
RAMC_LIBS := $(RVM_LIB) -L/nscratch/joao/ramcloud/obj.master \
	-lramcloud -lboost_system -lboost_program_options -lstdc++
RMEM_LIBS := $(RVM_LIB) -lrvm -lrdmacm -libverbs -lpthread
RAMC_OBJS := /nscratch/joao/ramcloud/obj.master/OptionParser.o
LDFLAGS := -pg -g
BENCHMARKS := commit-bm-rm recovery-bm-rm commit-bm-rc recovery-bm-rc blcr-bm
STATIC_LIB := ../../librvm.a

all: $(BENCHMARKS)

%-rm: %-rm.o $(STATIC_LIB)
	$(CC) $(LDFLAGS) $< -o $@ $(RMEM_LIBS)

%-rm.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%-rc: %-rc.o $(RAMC_OBJS) $(STATIC_LIB)
	$(CXX) $(LDFLAGS) $< $(RAMC_OBJS) -o $@ $(RAMC_LIBS)

%-rc.o: %.c
	$(CC) $(CFLAGS) -DRAMC -c -o $@ $<

blcr-bm.o: blcr-bm.c
	$(CC) $(CFLAGS) -c -o $@ $<

blcr-bm: blcr-bm.o
	$(CC) $(LDFLAGS) $< -o $@

clean:
	rm -f $(BENCHMARKS) *.o

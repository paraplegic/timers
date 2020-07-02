SRC=clock.c obcache.c timer.c
HDR=clock.h obcache.h timer.h
OBJ=clock.o obcache.o timer.o


SRC_DIR = ./src
TGT_DIR = ./usr
TEST_DIR = ./test

CC ?= gcc
DEST_DIR ?= $(TGT_DIR)

INC_DIR = $(DEST_DIR)/include
LIB_DIR = $(DEST_DIR)/lib
BIN_DIR = $(DEST_DIR)/bin

LDOPTS += -L $(LIB_DIR)
COPTS += -I $(INC_DIR)

all:	install $(BIN_DIR)/crash_srv
	
%.o: $(SRC_DIR)/%.c 
	$(CC) -c -o $@ $< $(OPTS)

$(LIB_DIR)/libEvent.so:	$(OBJ)
	ar rcs $@ $^
	rm $(OBJ)

init:
	mkdir -p $(LIB_DIR)
	mkdir -p $(INC_DIR)
	mkdir -p $(BIN_DIR)
	cp $(SRC_DIR)/*.h $(INC_DIR)

install:	init	$(LIB_DIR)/libEvent.so
	
clean:
	rm -rf *.o
	rm -rf core
	rm -rf backup.tgz
	rm -rf $(TEST_DIR)

realclean:	clean
	rm -rf $(LIB_DIR)
	rm -rf $(INC_DIR)
	rm -rf $(BIN_DIR)
	rm -rf $(DEST_DIR)

test_dir:	$(TEST_DIR)
	mkdir -p $(TEST_DIR)

test:	install test_dir $(TEST_DIR)/clock $(TEST_DIR)/obcache $(TEST_DIR)/timer
	$(TEST_DIR)/clock
	$(TEST_DIR)/obcache
	$(TEST_DIR)/timer

$(TEST_DIR)/obcache: $(SRC_DIR)/obcache.c $(SRC_DIR)/obcache.h
	$(CC) $(COPTS) -o $@ -DTEST $<

$(TEST_DIR)/clock:	$(SRC_DIR)/clock.c $(SRC_DIR)/clock.h
	$(CC) $(COPTS) -o $@ -DTEST $<

$(TEST_DIR)/timer:	$(SRC_DIR)/timer.c $(SRC_DIR)/timer.h
	$(CC) $(COPTS) -o $@ -DTEST $< -lrt -L $(LIB_DIR) -lEvent

backup:	realclean backup.tgz

backup.tgz: $(SRC_DIR) Makefile README.md main.c
	tar -zcvf $@ $^

$(BIN_DIR)/crash_srv: $(SRC_DIR)/main.c 
	$(CC) $(COPTS) -o $@ $< $(LDOPTS) -lEvent -lrt

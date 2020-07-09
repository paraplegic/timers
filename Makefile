SRC=clock.c obcache.c timer.c
HDR=clock.h obcache.h timer.h
OBJ=clock.o obcache.o timer.o

LIB_NAME = QEvent

SRC_DIR = ./src
TGT_DIR = ./usr
TEST_DIR = ./test

CC ?= gcc
DEST_DIR ?= $(TGT_DIR)

INC_DIR = $(DEST_DIR)/include/$(LIB_NAME)
LIB_DIR = $(DEST_DIR)/lib
BIN_DIR = $(DEST_DIR)/bin

LDOPTS += -L $(LIB_DIR)
COPTS += -I $(INC_DIR)


all:	$(DEST_DIR) $(BIN_DIR)/crash_srv
	
%.o: $(SRC_DIR)/%.c 
	$(CC) -c -o $@ $< $(OPTS)

$(LIB_DIR)/libQEvent.so:	$(OBJ)
	ar rcs $@ $^
	rm $(OBJ)

$(DEST_DIR):
	mkdir -p $(LIB_DIR)
	mkdir -p $(INC_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(TEST_DIR)
	cp $(SRC_DIR)/*.h $(INC_DIR)

install:	$(DEST_DIR)
ifeq ($(TGT_DIR), $(DEST_DIR))
	@echo "DEST_DIR=<install_path> make install"
else
	cp -r $(TGT_DIR)/* $(DEST_DIR)
endif
	
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


test:	all $(TEST_DIR)/clock $(TEST_DIR)/obcache $(TEST_DIR)/timer
	$(TEST_DIR)/clock
	$(TEST_DIR)/obcache
	$(TEST_DIR)/timer

$(TEST_DIR)/obcache: $(SRC_DIR)/obcache.c $(SRC_DIR)/obcache.h
	$(CC) $(COPTS) -o $@ -DTEST $<

$(TEST_DIR)/clock:	$(SRC_DIR)/clock.c $(SRC_DIR)/clock.h
	$(CC) $(COPTS) -o $@ -DTEST $<

$(TEST_DIR)/timer:	$(SRC_DIR)/timer.c $(SRC_DIR)/timer.h $(LIB_DIR)/libQEvent.so
	$(CC) $(COPTS) -o $@ -DTEST $< $(LDOPTS) -lQEvent -lrt

backup:	realclean backup.tgz

backup.tgz: $(SRC_DIR) Makefile README.md main.c
	tar -zcvf $@ $^

$(BIN_DIR)/crash_srv: $(SRC_DIR)/main.c $(LIB_DIR)/libQEvent.so
	$(CC) $(COPTS) -o $@ $< $(LDOPTS) -lQEvent -lrt

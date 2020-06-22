SRC=clock.c obcache.c timer.c
HDR=clock.h obcache.h timer.h
OBJ=clock.o obcache.o timer.o

CC=gcc
COPTS=-g
COPTS=-g

SRC_DIR=./src
INC_DIR=./include
LIB_DIR=./lib
BIN_DIR=./bin

all:	install crash_srv
	
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
	rm -rf crash_srv
	rm -rf backup.tgz

realclean:	clean
	rm -rf $(LIB_DIR)
	rm -rf $(INC_DIR)
	rm -rf $(BIN_DIR)

test:	install $(BIN_DIR)/clock $(BIN_DIR)/obcache $(BIN_DIR)/timer
	$(BIN_DIR)/clock
	$(BIN_DIR)/obcache
	$(BIN_DIR)/timer

$(BIN_DIR)/obcache: $(SRC_DIR)/obcache.c $(SRC_DIR)/obcache.h
	$(CC) $(COPTS) -o $@ -DTEST $<

$(BIN_DIR)/clock:	$(SRC_DIR)/clock.c $(SRC_DIR)/clock.h
	$(CC) $(COPTS) -o $@ -DTEST $<

$(BIN_DIR)/timer:	$(SRC_DIR)/timer.c $(SRC_DIR)/timer.h
	$(CC) $(COPTS) -o $@ -DTEST $< -lrt -L $(LIB_DIR) -lEvent

backup:	realclean backup.tgz

backup.tgz: $(SRC_DIR) Makefile README.txt main.c
	tar -zcvf $@ $^

crash_srv: main.c 
	$(CC) $(COPTS) -o $@ $< -I $(INC_DIR) -L $(LIB_DIR) -lEvent -lrt


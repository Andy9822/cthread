#
# Makefile para programas teste
#

CC=gcc
LIB_DIR=../lib
BIN_DIR=../bin
SRC_DIR=../src
BUILD_DIR=build
all: test-cidentify

test-cidentify:
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -o $(BUILD_DIR)/test-cidentify cthread.o test-cidentify.c -Wall

test-ccreate:
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -o $(BUILD_DIR)/test-ccreate cthread.o test-ccreate.c -Wall

clean:
	rm -rf *.o *~ build

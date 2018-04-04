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
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/mysupport.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -o $(BUILD_DIR)/test-cidentify LGA_logger.o cthread.o mysupport.o test-cidentify.c -Wall

test-ccreate:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-ccreate LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-ccreate.c -Wall

clean:
	rm -rf *.o *~ build

#
# Makefile para programas teste
#

CC=gcc
LIB_DIR=../lib
SRC_DIR=../src
BUILD_DIR=build
all: test-cidentify

test-cidentify:
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -o $(BUILD_DIR)/test-cidentify cthread.o test-cidentify.c -Wall

clean:
	rm -rf *.o *~ build

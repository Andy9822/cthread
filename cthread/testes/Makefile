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

test-cjoin:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cjoin LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cjoin.c -Wall

test-cjoin-critical:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cjoin-critical LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cjoin-critical.c -Wall

test-cjoin-critical2:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cjoin-critical2 LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cjoin-critical2.c -Wall

test-cyield:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cyield LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cyield.c -Wall

test-csuspend:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-csuspend LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-csuspend.c -Wall

test-cresume:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cresume LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cresume.c -Wall

test-global-suspension:
		rm -rf build
		mkdir $(BUILD_DIR)
		$(CC) -c $(SRC_DIR)/cthread.c
		$(CC) -c $(SRC_DIR)/LGA_logger.c
		$(CC) -c $(SRC_DIR)/LGA_support.c
		$(CC) -o $(BUILD_DIR)/test-global-suspension LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-global-suspension.c -Wall

test-mandel:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-mandel LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-mandel.c -Wall

test-series:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-series LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-series.c -Wall

test-cyield_ERROR:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cyield_ERROR LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cyield_ERROR.c -Wall

test-csuspend_ERROR:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-csuspend_ERROR LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-csuspend_ERROR.c -Wall

test-cresume_ERROR:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cresume_ERROR LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cresume_ERROR.c -Wall

test-cjoin_ERROR:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-cjoin_ERROR LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-cjoin_ERROR.c -Wall

test-semaforo:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-semaforo LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-semaforo.c -Wall


test-threaddepth:
	rm -rf build
	mkdir $(BUILD_DIR)
	$(CC) -c $(SRC_DIR)/cthread.c
	$(CC) -c $(SRC_DIR)/LGA_logger.c
	$(CC) -c $(SRC_DIR)/LGA_support.c
	$(CC) -o $(BUILD_DIR)/test-threaddepth LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-threaddepth.c -Wall
	$(CC) -o $(BUILD_DIR)/test-wait LGA_logger.o $(BIN_DIR)/support.o LGA_support.o cthread.o test-wait.c -Wall


clean:
	rm -rf *.o *~ build

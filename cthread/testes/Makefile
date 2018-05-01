#
# Makefile para programas teste
#

CC=gcc
LIB_DIR=../lib
BIN_DIR=../bin
SRC_DIR=../src
TST_DIR=../testes
BUILD_DIR=build
OBJ=$(wildcard $(TST_DIR)/*.c)

all: $(OBJ)
	$(foreach file, $(OBJ),$(CC) $(file) -L../lib -lcthread -Wall -o $(file:.c=).exe;)

test-cidentify:
	$(CC) -o test-cidentify test-cidentify.c -L../lib -lcthread -Wall

test-ccreate:
	$(CC) -o test-ccreate test-ccreate.c -L../lib -lcthread -Wall

test-cjoin:
	$(CC) -o test-cjoin test-cjoin.c -L../lib -lcthread -Wall

test-cjoin-critical:
	$(CC) -o test-critical test-critical.c -L../lib -lcthread -Wall

test-cjoin-critical2:
	$(CC) -o test-critical2 test-critical2.c -L../lib -lcthread -Wall

test-cyield:
	$(CC) -o test-cyield test-cyield.c -L../lib -lcthread -Wall

test-csuspend:
	$(CC) -o test-csuspend test-csuspend.c -L../lib -lcthread -Wall

test-cresume:
	$(CC) -o test-cresume test-cresume.c -L../lib -lcthread -Wall

test-global-suspension:
	$(CC) -o test-suspension test-suspension.c -L../lib -lcthread -Wall

test-mandel:
	$(CC) -o test-mandel test-mandel.c -L../lib -lcthread -Wall

test-series:
	$(CC) -o test-series test-series.c -L../lib -lcthread -Wall

test-cyield_ERROR:
	$(CC) -o test-cyield_ERROR test-cyield_ERROR.c -L../lib -lcthread -Wall

test-csuspend_ERROR:
	$(CC) -o test-csuspend_ERROR test-csuspend_ERROR.c -L../lib -lcthread -Wall

test-cresume_ERROR:
	$(CC) -o test-cresume_ERROR test-cresume_ERROR.c -L../lib -lcthread -Wall

test-cjoin_ERROR:
	$(CC) -o test-cjoin_ERROR test-cjoin_ERROR.c -L../lib -lcthread -Wall

test-semaforo:
	$(CC) -o test-semaforo test-semaforo.c -L../lib -lcthread -Wall

test-wait_suspens:
	$(CC) -o test-wait_suspens test-wait_suspens.c -L../lib -lcthread -Wall

test-wait_suspens_lib:
	$(CC) -o test-wait_suspens_lib test-wait_suspens_lib.c -L../lib -lcthread -Wall


clean:
	rm -rf *.o *~ build *.exe

#
# Makefile de EXEMPLO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# É NECESSARIO ADAPTAR ESSE ARQUIVO de makefile para suas necessidades.
#  1. Cuidado com a regra "clean" para não apagar o "support.o"
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "cthread"
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: cthread libcthread

cthread:
	$(CC) -c $(SRC_DIR)/cthread.c -o $(BIN_DIR)/cthread.o
	$(CC) -c $(SRC_DIR)/LGA_logger.c -o $(BIN_DIR)/LGA_logger.o
	$(CC) -c $(SRC_DIR)/LGA_support.c -o $(BIN_DIR)/LGA_support.o

libcthread:
	ar crs $(LIB_DIR)/libcthread.a $(BIN_DIR)/cthread.o $(BIN_DIR)/LGA_logger.o $(BIN_DIR)/LGA_support.o $(BIN_DIR)/support.o

clean:
	find $(BIN_DIR)/*.o ! -name 'support.o' -type f -exec rm -f {} +
	rm -rf $(LIB_DIR)/*.a $(SRC_DIR)/*~ $(INC_DIR)/*~ *~

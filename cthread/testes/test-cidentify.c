
/*
 *	Programa de Testes
 *
 * Teste da funcao cidentify
 */

#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

	char nome[100] = {0};

	cidentify(nome, 99);

	puts(nome);

	return 0;
}


/*
 *	Programa de Testes
 *
 * Teste da funcao cidentify
 */

#include "../include/support.h"
#include "../include/cthread.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

	char name[100] = {0}, name2[100] = {0};
	int response = 027;

	puts("Testing - cidentify");

	response = cidentify(name, 99);
	if (response == 0) {
		puts("First Case - SUCCEEDED");
	} else {
		puts("First Case - FAILED");
	}

	response = 027;

	response = cidentify(name2, 50);
	if (response == -1) {
		puts("Second Case - SUCCEEDED");
	} else {
		puts("Second Case - FAILED");
	}

	return 0;
}

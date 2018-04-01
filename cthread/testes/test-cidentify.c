/*
 *	Test Program - cidentify
 */

#include <stdio.h>

#include "../include/support.h"
#include "../include/cthread.h"
#include "../include/LGA_logger.h"

int main(int argc, char *argv[]) {

	char name[100] = {0}, name2[100] = {0};
	int response = 027;

	LGA_LOGGER_TEST("Testing cidentify");

	response = cidentify(name, 99);
	if (response == 0) {
		LGA_LOGGER_TEST("First Case - SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("First Case - FAILED");
	}

	response = 027;

	response = cidentify(name2, 50);
	if (response == -1) {
		LGA_LOGGER_TEST("Second Case - SUCCEEDED");
	} else {
		LGA_LOGGER_TEST("Second Case - FAILED");
	}

	return 0;
}

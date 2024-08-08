#include <stdio.h>
#include <stdlib.h>
#include "strutil.h"

int main (void)
{
	char name_list[3][10] = {"Emrah", "Vladislav", "Celil"};
	int res = 0;

	struct Str *names = str_init();
	if (names == NULL) {
		fprintf(stderr, "Memory allocation failed\n");
		return EXIT_FAILURE;
	}

	for (int i = 0; i < 3; i++) {
		res = str_add(names, name_list[i]);
		if (res) {
			str_free(names);
			return EXIT_FAILURE;
		}
		if (i != 2) {
			res = str_add(names, " ");
			if (res) {
				str_free(names);
				return EXIT_FAILURE;
			}
		}
	}

	printf("names: %s.\n", str_get_data(names));
	//Emrah Vladislav Celil.

	res = 0;
	res = str_rem_word(names, "islav");
	if (res) {
		fprintf(stderr, "str_rem_word failed\n");
		return EXIT_FAILURE;
	} else {
		printf("names: %s.\n", str_get_data(names));
		//Emrah Vlad Celil.
	}

	str_free(names);
	return 0;
}
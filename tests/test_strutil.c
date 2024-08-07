#include <stdio.h>
#include <string.h>
#include "strutil.h"


#define STR_PRINTERR() do {							\
	fprintf(stderr, "\033[1;31m%s failed, file: %s, line: %d\033[0m\n",	\
			__func__, __FILE__, (__LINE__ -1));\
} while (0)

#define STR_PRINTERR_CLEAR_AND_RETURN(s) do {	\
	str_clear(s);				\
	STR_PRINTERR();				\
	return;					\
} while (0)

#define FINISH_MSG(s, TEST_NAME) do {			\
	str_clear(s);					\
	printf("\033[1;32mTest %d %s passed\033[0m\n", 	\
		test_count++, #TEST_NAME);		\
	fflush(stdout);					\
} while (0)

unsigned int test_count = 1;


/*	FUNCTIONS	*/
struct Str *test_str_init()
{
	struct Str *s = str_init();
	if (s == NULL || s->is_dynamic != 1) {
		STR_PRINTERR();
		str_clear(s);
		return NULL;
	}

	return s;
}

void test_str_add(struct Str *s)
{
	if (s == NULL || s->data != NULL)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, "Hello"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strcmp(s->data, "Hello"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_add);
}

void test_str_input(struct Str *s)
{
	if (s == NULL || s->data != NULL) {
		STR_PRINTERR();
		return;
	}
	printf("test_str_input, write \"Hello\" and press enter: ");
	fflush(stdout);

	if(str_input(s))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strcmp(s->data, "Hello")) {
		STR_PRINTERR();
		fprintf(stderr, "\033[1;31mexpected: \"HELLO\", entered: %s\033[0m\n", s->data);
		str_clear(s);
		return;
	}
	
	FINISH_MSG(s, test_str_input);
}

void test_str_pop_back(struct Str *s)
{
	if (s == NULL || s->data != NULL)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, "Hello"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, " "))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, "World"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_pop_back(s, ' '))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strcmp(s->data, "Hello "))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_pop_back);
}

void test_str_get_size(struct Str *s)
{
	if (s == NULL || s->data != NULL)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, "Hello"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_get_size(s) != 5)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_get_size);
}

void test_str_rem_word(struct Str *s)
{
	if (s == NULL || s->data != NULL)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, "Hello World"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_rem_word(s, "World"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strcmp(s->data, "Hello "))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_rem_word);	
}

void test_str_swap_word(struct Str *s)
{
	if (s == NULL || s->data != NULL)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, "Hello World"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_swap_word(s, "Hello", "Hi"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strcmp(s->data, "Hi World"))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_swap_word);
}

void test_str_to_upper(struct Str *s)
{
	if (!s) {
		STR_PRINTERR();
		return;
	}

	const char test_msg_l[] = "Hello World!";
	const char test_msg_u[] = "HELLO WORLD!";

	if (str_add(s, test_msg_l))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_to_upper(s))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strcmp(s->data, test_msg_u))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_to_upper);
}

void test_str_to_lower(struct Str *s)
{
	if (!s)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	const char test_msg_u[] = "HELLO WORLD!";
	const char test_msg_l[] = "hello world!";

	if (str_add(s, test_msg_u))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_to_lower(s))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strcmp(s->data, test_msg_l))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_to_lower);
}

void test_str_reverse(struct Str *s)
{
	char msg[] = "Hello World!";
	char rev_msg[] = "!dlroW olleH";

	if (!s)
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_add(s, msg))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (str_reverse(s))
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	if (strncmp(s->data, rev_msg, strlen(rev_msg))) 
		STR_PRINTERR_CLEAR_AND_RETURN(s);

	FINISH_MSG(s, test_str_reverse);
}
 


#include "test_strutil.c"

int main()
{
	struct Str *s = test_str_init();
	if (!s)
		return EXIT_FAILURE;

	test_str_add(s);
	test_str_input(s);
	test_str_pop_back(s);
	test_str_get_size(s);
	test_str_rem_word(s);
	test_str_swap_word(s);
	test_str_to_upper(s);
	test_str_to_lower(s);
	test_str_reverse(s);
	
	str_free(s);
	return 0;
}
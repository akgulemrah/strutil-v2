/*
 * strutil.h - String Utility Functions
 *
 * This header file provides a set of functions for manipulating and managing
 * strings in a thread-safe manner. The `str` structure allows for dynamic
 * string management with support for various operations such as adding,
 * modifying, and querying string data.
 *
 * Features:
 * - Dynamic memory management for string data.
 * - Thread safety through mutex locking.
 * - Functions for common string operations like conversion, reversal, and
 *   manipulation.
 *
 * The functions provided in this header file include:
 * - `str_init()`: Initialize a new `Str` structure.
 * - `str_add()`: Append a string to the existing data.
 * - `str_input()`: Read a string from standard input.
 * - `str_add_input()`: Append input from standard input to existing data.
 * - `str_pop_back()`: Remove trailing data after a specified separator.
 * - `str_print()`: Print the string data to standard output.
 * - `str_get_size()`: Get the length of the string.
 * - `str_clear()`: Clear the string data.
 * - `str_free()`: Free the `Str` structure and its associated resources.
 * - `str_rem_word()`: Remove a specified word from the string.
 * - `str_swap_word()`: Swap occurrences of two words in the string.
 * - `str_to_upper()`: Convert the string to uppercase.
 * - `str_to_lower()`: Convert the string to lowercase.
 * - `str_to_title_case()`: Convert the string to title case.
 * - `str_reverse()`: Reverse the string.
 * - `str_is_empty()`: Check if the string is empty.
 * - `get_dyn_input()`: Helper function to read input dynamically.
 *
 * This file includes necessary headers for standard operations and thread safety,
 * and defines the `Str` structure along with associated function prototypes.
 *
 * Author: Emrah Akgül
 * Date: 27.07.2024
 *
 * License: unlicense
 */


#ifndef _STRUTIL_H_
#define _STRUTIL_H_


#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#if defined(__clang__) || defined(__GNUC__)
  #define STR_WARN_UNUSED_RESULT __attribute((warn_unused_result))
#else
  #define STR_WARN_UNUSED_RESULT 
#endif

extern const size_t MAX_STRING_SIZE;


struct Str {
	char	*data;
	unsigned char is_dynamic;
	pthread_mutex_t lock;
};

struct Pointer_counter {
	struct Str *str_ptr;
	struct Pointer_counter *next;
	size_t counter;
	pthread_mutex_t lock;
};

/*
 * str_init - Initialize a new Str structure
 *
 * This function allocates and initializes a new Str structure. It sets the
 * is_dynamic flag to 1 and initializes the mutex. The caller is responsible
 * for freeing the allocated memory using str_free().
 *
 * Return: Pointer to the newly allocated Str structure, or NULL if memory
 * allocation fails.
 */
struct Str *str_init(void);


/*
 * str_add - Add a string to the Str structure
 *
 * @self: Pointer to the Str structure
 * @_data: Pointer to the string to be added
 *
 * This function appends the given string @_data to the existing data in
 * the Str structure. If the data is already present, it reallocates
 * memory to accommodate the new string. Ensures thread safety with
 * mutex locks. Returns 0 on success or a negative error code on failure.
 */
int str_add(struct Str *self, const char *_data);


/*
 * str_input - Read a string from standard input into the Str structure.
 *
 * @self: Pointer to the Str structure where the input will be stored.
 *
 * This function reads a line of input from standard input (stdin) and
 * stores it in the Str structure. It allocates memory dynamically for
 * the input string. Thread safety is ensured by locking the mutex. If
 * there is already data present in the structure, it will return an error.
 * Returns 0 on success, or -1 if an error occurs.
 *
 * Return: 0 on success, or -1 on failure.
 */
int str_input(struct Str *self);


/*
 * str_print - Print the string data to standard output.
 *
 * @self: Pointer to the Str structure containing the data to print.
 *
 * This function prints the string data stored in the Str structure to
 * standard output (stdout). It acquires a mutex lock to ensure thread safety
 * while accessing the data. If the Str structure or its data is NULL, nothing
 * will be printed.
 */
void str_print(struct Str *self);


/*
 * str_free - Free the Str structure and its resources.
 *
 * @self: Pointer to the Str structure to be freed.
 *
 * This function releases all memory allocated for the Str structure,
 * including the string data and the structure itself if it was dynamically
 * allocated. Ensures thread safety by locking the mutex during the cleanup
 * process. If the Str structure or its data is NULL, nothing is done.
 */
void str_free(struct Str *self);


/*
 * str_pop_back - Remove the trailing portion of the string after the last
 * occurrence of a separator.
 *
 * @self: Pointer to the Str structure from which the data will be modified.
 * @sep: Separator character after which data will be removed.
 *
 * This function trims the string data in the Str structure by removing
 * the portion of the string following the last occurrence of the specified
 * separator character. Thread safety is ensured by using mutex locking.
 * Returns 0 on success, or a negative error code if an error occurs.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
int str_pop_back(struct Str *self, char sep);


/*
 * str_get_size - Get the length of the string in the Str structure.
 *
 * @self: Pointer to the Str structure whose string length is queried.
 *
 * This function returns the length of the string data stored in the Str
 * structure. If the structure or its data is NULL, it returns 0.
 *
 * Return: Length of the string, or 0 if the Str structure or data is NULL.
 */
size_t  str_get_size(const struct Str *self);


/*
 * str_clear - Clear the string data from the Str structure.
 *
 * @self: Pointer to the Str structure whose data will be cleared.
 *
 * This function deallocates the memory used for the string data in the
 * Str structure and sets the data pointer to NULL. Thread safety is ensured
 * through mutex locking. If the Str structure itself is NULL, no action is
 * taken.
 */
void str_clear(struct Str *self);


/*
 * str_rem_word - Remove all occurrences of a word from the string.
 *
 * @self: Pointer to the Str structure from which the word will be removed.
 * @needle: The word to remove from the string.
 *
 * This function removes all occurrences of the specified word from the
 * string data stored in the Str structure. Adjusts memory allocation as
 * necessary. Mutex locking is used to ensure thread safety. Returns 0 on
 * success, or a negative error code if an error occurs.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
int str_rem_word(struct Str *self, const char *needle);


/*
 * str_get_data - Retrieve the current string data from the Str structure.
 *
 * @self: Pointer to the Str structure from which to retrieve the data.
 *
 * This function returns a pointer to the string data stored within the
 * Str structure. If the Str structure or its data is NULL, the function
 * returns NULL. The caller should not modify the returned string directly
 * as it is managed by the Str structure. If modifications are needed,
 * use the appropriate functions provided by the Str API.
 *
 * Return: Pointer to the string data stored in the Str structure, or NULL
 *         if the Str structure or its data is NULL.
 */
const char *str_get_data(const struct Str *self);


/*
 * str_add_input - Append a line of input to existing string data.
 *
 * @self: Pointer to the Str structure where the input will be appended.
 *
 * This function reads a line of input from standard input (stdin) and appends
 * it to the existing string data within the Str structure. If there is no
 * existing data, it initializes the data with the new input. If there is
 * existing data, it reallocates memory to accommodate the new input and
 * appends it. Ensures thread safety with mutex locking. Returns 0 on success,
 * or a negative error code if any issue arises during input handling or memory
 * allocation.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
int str_add_input(struct Str *self);


/*
 * str_to_upper - Convert the entire string to uppercase.
 *
 * @self: Pointer to the Str structure containing the string to convert.
 *
 * This function transforms all lowercase characters in the string data
 * to uppercase. Thread safety is maintained through mutex locking. If
 * the Str structure or its data is NULL, returns -1.
 *
 * Return: 0 on success, or -1 if the Str structure or data is NULL.
 */
int str_to_upper(struct Str *self);


/*
 * str_to_lower - Convert the entire string to lowercase.
 *
 * @self: Pointer to the Str structure containing the string to convert.
 *
 * This function transforms all uppercase characters in the string data
 * to lowercase. Mutex locking is used to ensure thread safety. Returns 0
 * on success, or -1 if the Str structure or data is NULL.
 *
 * Return: 0 on success, or -1 if the Str structure or data is NULL.
 */
int str_to_lower(struct Str *self);


/*
 * get_dyn_input - Read a line of input from stdin with dynamic allocation.
 *
 * @max_str_size: The maximum size of the input string to read.
 *
 * This function reads a line of input from the standard input (stdin), dynamically
 * allocating memory as needed to accommodate the input. It grows the buffer in chunks
 * if necessary to handle larger inputs. The function ensures that the total size of
 * the buffer does not exceed the specified maximum size. After reading the input, it
 * copies the buffer to a new memory location and returns it, freeing the original
 * buffer. Returns a pointer to the newly allocated string or NULL if an error occurs.
 *
 * Return: Pointer to the dynamically allocated input string, or NULL on failure.
 */
char* get_dyn_input(size_t max_str_size);


/*
 * str_swap_word - Swap occurrences of two words in the string.
 *
 * @self: Pointer to the Str structure where the words will be swapped.
 * @word1: The word to be replaced.
 * @word2: The word to replace word1 with.
 *
 * This function replaces all occurrences of word1 in the string with word2.
 * Handles memory allocation and ensures thread safety with mutex locking.
 * Returns 0 on success, or a negative error code if an error occurs.
 *
 * Return: 0 on success, or a negative error code on failure.
 */
int str_swap_word(struct Str *self, const char *word1, const char *word2);


/*
 * str_to_title_case - Convert the string to title case.
 *
 * @self: Pointer to the Str structure containing the string to modify.
 *
 * This function converts the string data to title case, where the first
 * letter of each word is capitalized and the remaining letters are in
 * lowercase. Mutex locking ensures thread safety. Returns 0 on success,
 * or -1 if the Str structure or its data is NULL or empty.
 *
 * Return: 0 on success, or -1 if the Str structure or data is NULL or empty.
 */
int str_to_title_case(struct Str *self);


/*
 * str_reverse - Reverse the string in the Str structure.
 *
 * @self: Pointer to the Str structure containing the string to reverse.
 *
 * This function reverses the entire string data stored in the Str structure.
 * Ensures thread safety by locking the mutex. Returns 0 on success, or -1
 * if the Str structure or its data is NULL, or if the string is empty.
 *
 * Return: 0 on success, or -1 if the Str structure or data is NULL or empty.
 */
int str_reverse(struct Str *self);


/*
 * str_is_empty - Check if the Str structure's string is empty.
 *
 * @self: Pointer to the Str structure to check.
 *
 * This function checks whether the string data in the Str structure is
 * either NULL or has zero length. Returns true if the string is empty,
 * false otherwise.
 *
 * Return: true if the string is empty or NULL, false otherwise.
 */
bool str_is_empty(struct Str *self);


struct Pointer_counter *pointer_counter_create(void);
int pointer_counter_add(struct Pointer_counter **head, struct Str *_str_ptr);
int pointer_counter_free(struct Pointer_counter **head, struct Str *_str_ptr);
#endif
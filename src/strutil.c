#include "strutil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>

const size_t MAX_STRING_SIZE  = ((SIZE_MAX / 100) * 95);

struct Str *str_init(void)
{
	struct Str *tmp = (struct Str *)calloc(1, sizeof(struct Str));
	if (!tmp)
		return NULL;

	tmp->is_dynamic = 1;
	pthread_mutex_init(&tmp->lock, NULL);
	return tmp;
}


int str_add(struct Str *self, const char *_data)
{
	if (self == NULL || _data == NULL)
		return -EINVAL;

	pthread_mutex_lock(&self->lock);
	size_t size = strlen(_data);

	if (self->data != NULL) {
		size += strlen(self->data);

		char *p = (char *)realloc(self->data, (size + 1));
		if (p) {
			self->data = p;
		} else {
			pthread_mutex_unlock(&self->lock);
			return -ENOMEM;
		}
		strcat(self->data, _data);
	} else {
		self->data = (char *)malloc((size + 1) * sizeof(char));
		if (!self->data) {
			pthread_mutex_unlock(&self->lock);
			return -1;
		}
		strcpy(self->data, _data);
		if (strlen(self->data) != strlen(_data)) {
			pthread_mutex_unlock(&self->lock);
			return -1;
		}
	}
	pthread_mutex_unlock(&self->lock);
	return 0;
}


int str_input(struct Str *self)
{
	if (self == NULL) {
		return -1;
	} else if(self->data != NULL) {
		return -1;
	}
	
	pthread_mutex_lock(&self->lock);
	self->data = get_dyn_input(MAX_STRING_SIZE);
	pthread_mutex_unlock(&self->lock);
	return (self->data ? 0 : -1);
}


int str_add_input(struct Str *self)
{
	if (!self) {
		return -1;
	} 

	pthread_mutex_lock(&self->lock);

	if (!self->data) {
		self->data = get_dyn_input(MAX_STRING_SIZE);
		
		if (self->data == NULL) {
			pthread_mutex_unlock(&self->lock);
			return -2;
		}
		pthread_mutex_unlock(&self->lock);
		return 0;
	}

	char *self_data_ptr = self->data;
	size_t self_data_size = strlen(self_data_ptr);

	char *buf = get_dyn_input(MAX_STRING_SIZE - self_data_size);
	if (!buf) {
		pthread_mutex_unlock(&self->lock);
		return -1;
	}

	char *new_data = (char *)realloc(self_data_ptr, (self_data_size + strlen(buf) + 1));
	if (!new_data) {
		pthread_mutex_unlock(&self->lock);
		return -1;
	}

	self_data_ptr = new_data;
	strncat(self_data_ptr, buf, strlen(buf));

	free(buf);
	self->data = self_data_ptr;
	pthread_mutex_unlock(&self->lock);
	return 0;
}


int str_pop_back(struct Str *self, char sep)
{
	if (self == NULL) {
		return -1;
	} else if (self->data == NULL) {
		return -1;
	} else if (strlen(self->data) == 0) {
		return -1;
	}

	pthread_mutex_lock(&self->lock);

	char *p = strrchr(self->data, sep);
	if (!p) {
		pthread_mutex_unlock(&self->lock);
		return -EINVAL;
	}

	p++;
	*p = '\0';

	char *self_data_ptr = (char *)realloc(self->data, strlen(self->data) + 1); // Trim memory
	if (!self_data_ptr) {
		pthread_mutex_unlock(&self->lock);
		return -1;
	}
	
	self->data = self_data_ptr;
	pthread_mutex_unlock(&self->lock);
	return 0;
}


void str_print(struct Str *self)
{
	if (self) {
		pthread_mutex_lock(&self->lock);
		if (self->data) {
			printf("%s", self->data);
			fflush(stdout);
		}
		pthread_mutex_unlock(&self->lock);
	}
}



size_t str_get_size(const struct Str *self)
{
	if (self) {
    		return (self->data ? strlen(self->data) : 0);
	} else {
		return 0;
	}
}


const char *str_get_data(const struct Str *self)
{
	if (self) {
		if (self->data) {
    			return (const char *)self->data;
		} else {
			return NULL;
		}
	}
}


void str_clear(struct Str *self)
{
	if (self){
		pthread_mutex_lock(&self->lock);
		if (self->data) {
			free(self->data);
			self->data = NULL;
		}
		pthread_mutex_unlock(&self->lock);
	}
}


void str_free(struct Str *self)
{
	if (self) {
		if (self->data) {
			free(self->data);
			self->data = NULL;
		}
		if (self->is_dynamic) {
			pthread_mutex_destroy(&self->lock);
			free(self);
			self = NULL;
		}
	}
}


char* get_dyn_input(size_t max_str_size)
{
	const int CHUNK_SIZE = 10;
	char* buffer = (char *)calloc(CHUNK_SIZE,  sizeof(char));

	if (buffer == NULL) 
		return NULL;

	size_t current_size = CHUNK_SIZE; // Size of available memory.
	size_t length = 0; // Length of current string

	int c;
	while ((c = getchar()) != EOF && c != '\n') {
		if (length + 1 >= current_size) { // Expand memory
			current_size += CHUNK_SIZE;			
			char* tmp = realloc(buffer, current_size);

			if (tmp == NULL) {
				free(buffer);
				return NULL;
			}
			buffer = tmp;
		}

		if (current_size >= (max_str_size - 1)) {
			free(buffer);
			return NULL;
		}

		buffer[length++] = (char)c;
		buffer[length] = '\0'; // End the series
	}

	// Finally release the extra memory
	char *result = (char *)malloc((length + 1) * sizeof(char));
	if (result == NULL) {
		free(buffer);
		return NULL;
	}
	
	strncpy(result, buffer, strlen(buffer));
	result[strlen(buffer)] = '\0';

	free(buffer);
	return result;
}


int str_rem_word(struct Str *self, const char *needle)
{
	if (!self) {
		return -1;
	} else if (!self->data || !needle) {
		return -1;
	} 
        
	pthread_mutex_lock(&self->lock);
        size_t self_data_size = strlen(self->data);
        size_t needle_size = strlen(needle);
        
        if (needle_size > self_data_size) {
		pthread_mutex_unlock(&self->lock);
        	return -EINVAL;
	}
            
        char *L = NULL;
        L = strstr(self->data, needle);
        if(!L) {
		pthread_mutex_unlock(&self->lock);
        	return -EINVAL;
	}

        memmove(L, L + needle_size, self_data_size - (L - self->data) - needle_size + 1);
	self->data[self_data_size - needle_size] = '\0';
        
        char *buf = (char*)realloc(self->data, 
                ((self_data_size - needle_size)) +1);
        
	if (!buf) {	// realloc başarısız oldu; ancak kelime diziden kaldırıldı ve sonuna NULL eklendi
		pthread_mutex_unlock(&self->lock);
		return -1;
	}

        if (buf)
        	self->data = buf;

	pthread_mutex_unlock(&self->lock);
	return 0;
}


int str_swap_word(struct Str *self, const char *word1, const char *word2)
{
	if (!self) {
		return -1;
	} else if (!self->data || !self->is_dynamic || !word1 || !word2) {
		return -1;
	}

	pthread_mutex_lock(&self->lock);

	size_t self_data_size = strlen(self->data);
	size_t word1_size = strlen(word1);
	size_t word2_size = strlen(word2);

	char *L = strstr(self->data, word1);
	if (!L) {
		pthread_mutex_unlock(&self->lock);
		return -1;
	}

	size_t new_size = self_data_size - word1_size + word2_size;
	char *buf = (char *)malloc(sizeof(char) * (new_size + 1)); // +1 for null terminator
	if (!buf) {
		pthread_mutex_unlock(&self->lock);
		return -1;
	}

	// Copy everything before word1
	strncpy(buf, self->data, L - self->data);
	buf[L - self->data] = '\0'; // Null terminate the buffer

	// Copy word2
	strcat(buf, word2);

	// Copy everything after word1
	strcat(buf, L + word1_size);

	free(self->data);
	self->data = buf;

	pthread_mutex_unlock(&self->lock);
	return 0;
}


int str_to_upper(struct Str *self)
{
	if (self == NULL) {
		return -1;
	} else if (!self->data) {
		return -1;
	}
	pthread_mutex_lock(&self->lock);

	char *p = self->data;
	while (*p) {
		if ((*p >= 'a') && (*p <= 'z'))
			*p &= ~(1 << 5);
		p++;
	}
	pthread_mutex_unlock(&self->lock);
	return 0;
}


int str_to_lower(struct Str *self)
{
	if (!self) {
		return -1;
	} else if (!self->data) {
		return -1;
	}
	
	pthread_mutex_lock(&self->lock);
	char *p = self->data;
	while (*p) {
		if ((*p >= 'A') && (*p <= 'Z'))
			*p |= (1 << 5); // yes, faster than tolower :/
		p++;
	}

	pthread_mutex_unlock(&self->lock);
	return 0;
}


int str_to_title_case(struct Str *self)
{
	if (!self) {
		return -1;
	} else if (!self->data || !strlen(self->data)) {
		return -1;
	}
	
	pthread_mutex_lock(&self->lock);

	char *p = self->data;
	short flag = 1;

	while (*p) {
		if (flag && (*p >= 'a') && (*p <= 'z')) {
			*p &= ~(1 << 5);
			flag = 0;
		} else if (*p == ' ') {
			flag = 1;
		}
		p++;
	}

	pthread_mutex_unlock(&self->lock);
	return 0;
}

int str_reverse(struct Str *self)
{
	if (!self) {
		return -1;
	} else if (!self->data) {
		return -1;
	} else if (!strlen(self->data)) {
		return -1;
	}
    
	pthread_mutex_lock(&self->lock);
	char buf;
	char *data = self->data;
	size_t head = 0;
	size_t tail = strlen(self->data) - 1;

	while (head < tail) {
		buf = data[head];
		data[head] = data[tail];
		data[tail] = buf;
		head++;
		tail--;
	}

	pthread_mutex_unlock(&self->lock);
	return 0;
}

bool str_is_empty(struct Str *self)
{
	if (self){
		if (self->data == NULL){
			return true;
		} else if (strlen(self->data) == 0) {
			return true;
		} else {
			return false;
		}
	}
}


/*	POİNTER COUNTER FUNCTIONS	*/
struct Pointer_counter *pointer_counter_create(void)
{
	struct Pointer_counter *pc;
	pc = (struct Pointer_counter *)calloc(1, sizeof(struct Pointer_counter));
	if (!pc) {
		return NULL;
	}

	pc->str_ptr = (struct Str *)calloc(1, sizeof(struct Str));
	if (pc->str_ptr == NULL) {
		free(pc);
		return NULL;
	}

	if((pthread_mutex_init(&pc->lock, NULL)) != 0) {
		free(pc->str_ptr);
		free(pc);
		return NULL;
	}

	pc->counter = 0;
	return pc;
}


int pointer_counter_add(struct Pointer_counter **head,
			struct Str *_str_ptr)
{
	if (_str_ptr == NULL)
		return -1;
	
	struct Pointer_counter *pc_ptr = pointer_counter_create();
	if (pc_ptr == NULL)
		return -1;
	
	if (*head == NULL) {
		*head = pc_ptr;
		(*head)->counter++;
		(*head)->str_ptr = _str_ptr;
		(*head)->next = NULL;
		return 0;
	}
	
	struct Pointer_counter *head_ptr = *head;
	while(head_ptr->next) {
		head_ptr = head_ptr->next;
	}

	head_ptr->next = pc_ptr;
	head_ptr = head_ptr->next;
	head_ptr->counter++;
	head_ptr->next = NULL;
	head_ptr->str_ptr = _str_ptr;

	return 0;
}

int pointer_counter_free(struct Pointer_counter **head,
			struct Str *_str_ptr)
{
	if (*head == NULL || _str_ptr == NULL)
		return -1;
	
	if ((*head)->str_ptr == _str_ptr) {
		free(*head);
		return 0;
	}

	struct Pointer_counter *pc_iter = (*head)->next;
	struct Pointer_counter *pc_iter_back = *head;

	for (size_t i = 0; i < (*head)->counter; i++) {
		if (pc_iter == NULL) {
			return -1;
		} else if (pc_iter->str_ptr == _str_ptr) {
			pc_iter_back->next = pc_iter->next;
			free(pc_iter);
			return 0;
		}
		pc_iter = pc_iter->next;
		pc_iter_back = pc_iter_back->next;
	}
}
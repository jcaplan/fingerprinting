#include "galloc.h"
#include <string.h>

struct MEMORY_LIST {
	void *ptr;
	struct MEMORY_LIST *next;
};

struct MEMORY_LIST *memory_root = NULL;
struct MEMORY_LIST *memory_last = NULL;

void* galloc(size_t num, size_t size){
	void* ptr = calloc(num, size);
	if(!ptr){
		return NULL;
	}
	struct MEMORY_LIST *node = (struct MEMORY_LIST *)calloc(1, sizeof(struct MEMORY_LIST));
	node->ptr = ptr;
	if(memory_last){
		memory_last->next = node;
		memory_last = node;
	} else {
		memory_root = node;
		memory_last = node;
	}
	return ptr;
}

void cleanMemory(void){
	struct MEMORY_LIST *node = memory_root;
	while(node){
		free(node->ptr);
		memory_root = node->next;
		free(node);
		node = memory_root;
	}
	memory_root = NULL;
	memory_last = NULL;
}

void dealloc(void* ptr){
	struct MEMORY_LIST *node = memory_root;
	struct MEMORY_LIST *last = NULL;
	while(node){
		if(node->ptr == ptr){
			free(node->ptr);
			if(last){
				last->next = node->next;
			} else {
				memory_root = node->next;
			}
			if(!node->next){
				memory_last = last;
			}
			free(node);
			return;
		}
		last = node;
		node = node->next;
	}
}

//char* duplicate_string(const char* str){
//	char* new_string = (CHAR *)galloc(strlen(str) + 1, sizeof(char));
//	strcpy(new_string, str);
//	return new_string;
//}

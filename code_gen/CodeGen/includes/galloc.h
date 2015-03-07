#ifndef GALLOC_H
#define GALLOC_H

#include <stdlib.h>

void* galloc(size_t num, size_t size);
void cleanMemory(void);
void dealloc(void* ptr);
char* duplicate_string(const char* str);

#endif // MEMORY_H

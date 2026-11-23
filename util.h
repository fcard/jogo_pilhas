#ifndef JGP_UTIL
#define JGP_UTIL

#include "erro.h"
#include <stddef.h>

void* malloc_validado(size_t, Erro*);
void** malloc_matrix(size_t, size_t, size_t, Erro*);

#endif

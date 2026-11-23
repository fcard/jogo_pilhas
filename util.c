#include "util.h"
#include <stdlib.h>

void* malloc_validado(size_t sz, Erro* err) {
  void* m = malloc(sz);
  *err =  m == nullptr ? Erro_FaltaDeMemoria : Successo;
  return m;
}

void** malloc_matrix(size_t sz, size_t w, size_t h, Erro* err) {
  void** m = malloc_validado(sizeof(void*)*h, err);
  if (m != nullptr) {
    for (size_t i = 0; i < h; i++) {
      m[i] = malloc_validado(sz*w, err);
      if (m[i] == nullptr) {
        return nullptr;
      }
    }
    return m;
  } else {
    return nullptr;
  }
}


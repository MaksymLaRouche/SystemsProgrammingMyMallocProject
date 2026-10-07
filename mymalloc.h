//
// Created by maksym on 9/19/26.
//

#ifndef _MYMMALLOC_H
#define _MYMMALLOC_H

#include <stdlib.h>

#define malloc(X) mymalloc(X, __FILE__, __LINE__)
#define free(X) myfree(X, __FILE__, __LINE__)

void * mymalloc(size_t size, char *, int);
void myfree (void *, char *, int);

#endif
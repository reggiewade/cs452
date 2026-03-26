#ifndef WRAPPER_H
#define WRAPPER_H

#include "deq.h"

typedef struct Mtq *Mtq;

Mtq  mtq_new(int capacity);
void mtq_del(Mtq q);
void mtq_tail_put(Mtq q, Data d);
Data mtq_head_get(Mtq q);

#endif
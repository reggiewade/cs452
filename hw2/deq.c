#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

// indices and size of array of node pointers
typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];        // next/prev neighbors
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];                // head/tail nodes
  int len;
} *Rep;

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

/**
 * @brief Inserts a new element at the specified end of the deque.
 *
 * This function adds a new element to either the head or tail of the deque.
 *
 * @param r The deque representation.
 * @param e The end (Head or Tail) where the element is to be inserted.
 * @param d The data to be inserted.
 */

static void put(Rep r, End e, Data d) {
  if (r == NULL || d == NULL) ERROR("invalid representation or data");
  if (e != Head && e != Tail) ERROR("invalid End specified");
  Node n = (Node) malloc(sizeof(*n));
  if (!n) ERROR("malloc() failed");
  n->data = d;
  n->np[Head] = 0;
  n->np[Tail] = 0;

  if (r->len == 0) {
    r->ht[Head] = n;
    r->ht[Tail] = n;
  }
  else if (e == Head) {
    n->np[Tail] = r->ht[Head];
    r->ht[Head]->np[Head] = n;
    r->ht[Head] = n;
  }
  else {
    n->np[Head] = r->ht[Tail];
    r->ht[Tail]->np[Tail] = n;
    r->ht[Tail] = n;
  }

  r->len++;
}

/**
 * @brief Gets and removes an element from the specified end of the deque.
 *
 * The function removes and returns an element from either the head or tail of the deque.
 *
 * @param r The deque representation.
 * @param e The end (Head or Tail) where the element is to be returned and removed.
 * @return The data from the element at the specified end.
 */

static Data get(Rep r, End e) {
  if (r->len == 0) return 0;
  if (r == NULL) ERROR("invalid data");
  if (e != Head && e != Tail) ERROR("invalid End specified");
  Node n;
  if (e == Head) {
    n = r->ht[Head];
    r->ht[Head] = n->np[Tail];
    if (r->ht[Head]) r->ht[Head]->np[Head] = 0;
  }
  else {
    n = r->ht[Tail];
    r->ht[Tail] = n->np[Head];
    if (r->ht[Tail]) r->ht[Tail]->np[Tail] = 0;
  }

  Data d = n->data;
  free(n);
  r->len--;
  if (r->len == 0) {
    r->ht[Head] = 0;
    r->ht[Tail] = 0;
  }
  return d;
}

/**
 * @brief Gets and removes the ith element from the specified end of the deque.
 *
 * The function retrieves the ith element from either the head or tail of the deque.
 *
 * @param r The deque representation.
 * @param e The end (Head or Tail) where the element is to be returned and removed.
 * @param i The index of the element to retrieve.
 * @return The ith data from the specified end, or 0 if out of bounds.
 */

static Data ith(Rep r, End e, int i) {
  if (r == NULL) ERROR("invalid data");
  if (i<0 || i>=r->len) return 0;
  Node n;
  if (e==Head) {
    n = r->ht[Head];
    for (int j=0; j<i; j++)
      n = n->np[Tail];
  } else {
    n = r->ht[Tail];
    for (int j=0; j<i; j++)
      n = n->np[Head];
  }
  return n->data;
}

/**
 * @brief Gets and removes the element matching the given data from the specified end of the deque.
 *
 * The function retrieves and removes the element matching the given data from either the head or tail of the deque.
 *
 * @param r The deque representation.
 * @param e The end (Head or Tail) where the element is to be returned and removed.
 * @param d The data to be removed.
 * @return The data from the element matching the given data, or 0 if not found.
 */

static Data rem(Rep r, End e, Data d) {
  if (r == NULL || d == NULL) ERROR("invalid representation or data");
  if (e != Head && e != Tail) ERROR("invalid End specified");
  Node n;
  if (e==Head) {
    n = r->ht[Head];
    while (n && n->data != d)
      n = n->np[Tail];
  } else {
    n = r->ht[Tail];
    while (n && n->data != d)
      n = n->np[Head];
  }
  if (!n) return 0; // out of bounds
  // unlink node
  if (n->np[Head])
    n->np[Head]->np[Tail] = n->np[Tail];
  else
    r->ht[Head] = n->np[Tail];
  if (n->np[Tail])
    n->np[Tail]->np[Head] = n->np[Head];
  else
    r->ht[Tail] = n->np[Head];
  Data data = n->data;
  free(n);
  r->len--;
  return data;
}

extern Deq deq_new() {
  Rep r=(Rep)malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) {        put(rep(q),Head,d); }
extern Data deq_head_get(Deq q)         { return get(rep(q),Head);   }
extern Data deq_head_ith(Deq q, int i)  { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }

extern void deq_tail_put(Deq q, Data d) {        put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q)         { return get(rep(q),Tail);   }
extern Data deq_tail_ith(Deq q, int i)  { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) {
  if (f) deq_map(q,f);
  Node curr=rep(q)->ht[Head];
  while (curr) {
    Node next=curr->np[Tail];
    free(curr);
    curr=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}



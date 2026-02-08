#include <stdio.h>
#include <stdlib.h>

#include "deq.h"

int main() {
  // Create Queue
  Deq q=deq_new();

  printf("```Testing Head Functions```\n\n");

  // Test Empty Queue
  char *s = deq_str(q,0);
  printf("Empty Queue: %s\n",s);
  free(s);

  /** Test Head Functions with Empty Queue **/
  printf("\n``Initial Head Function Testing for Empty Queue``\n\n");

  // Test Head Put
  deq_head_put(q,NULL);
  s = deq_str(q,0);
  printf("Testing Head Put (Null Data): %s, Length: %d\n",s,deq_len(q));
  free(s);

  // Test Head ITh
  deq_head_ith(q,0);
  s = deq_str(q,0);
  printf("Testing Head iTh (Empty Queue): %s\n",s);
  free(s);

  // Test Head Get
  deq_head_get(q);
  s = deq_str(q,0);
  printf("Testing Head Get (Empty Queue): %s\n",s);
  free(s);

  // Test Head Rem
  deq_head_rem(q,"a");
  s = deq_str(q,0);
  printf("Testing Head Rem (Empty Queue): %s, Length: %d\n\n",s,deq_len(q));
  free(s);

  /** Test Normal Head Functions */
  printf("``Normal Head Testing``\n\n");

  // Test Head Put
  deq_head_put(q,"a");
  deq_head_put(q,"b");
  deq_head_put(q,"e");
  deq_head_put(q,"c");
  deq_head_put(q,"d");
  deq_head_put(q,"e");
  deq_head_put(q, "b");
  s = deq_str(q,0);
  printf("Testing Head Put: %s, Length: %d\n",s,deq_len(q));
  free(s);

  // Test Head iTh
  s = deq_head_ith(q, 3);
  printf("Testing Head iTh (Index: 3): %s\n",s);
  s = deq_head_ith(q, 5);
  printf("Testing Head iTh (Index: 5): %s\n",s);

  // Test Head iTh (Throws Warnings)
  s = deq_head_ith(q, -1);
  printf("Testing Head iTh (Index: -1): %s\n", s);
  s = deq_head_ith(q, deq_len(q));
  printf("Testing Head iTh (Index: %d): %s\n", deq_len(q),s);

  // Test Head Get
  s = deq_head_get(q);
  printf("Testing Head Get: %s\n",s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);
  s = deq_head_get(q);
  printf("Testing Head Get (2nd Time): %s\n",s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);

  // Test Head Rem
  s = deq_head_rem(q,"e");
  printf("Testing Head Rem (Data: e): %s\n", s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);

  // Test Head Rem (Throws Warnings)
  s = deq_head_rem(q, "z");
  printf("Testing Head Rem (Data: z): %s\n",s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);

  // Delete Queue
  deq_del(q,0);

  // Create New Queue
  q = deq_new();

  printf("\n\n```Testing Tail Functions```\n\n");

  // Test Empty Queue
  s = deq_str(q,0);
  printf("Empty Queue: %s\n",s);
  free(s);

  /** Test Tail Functions with Empty Queue **/
  printf("\n``Initial Tail Function Testing for Empty Queue``\n\n");
  
  // Test Tail Put
  deq_tail_put(q,NULL);
  s = deq_str(q,0);
  printf("Testing Tail Put (Null Data): %s, Length: %d\n",s,deq_len(q));
  free(s);

  // Test Tail ITh
  deq_tail_ith(q,0);
  s = deq_str(q,0);
  printf("Testing Tail iTh (Empty Queue): %s\n",s);
  free(s);

  // Test Tail Get
  deq_tail_get(q);
  s = deq_str(q,0);
  printf("Testing Tail Get (Empty Queue): %s\n",s);
  free(s);

  // Test Tail Rem
  deq_tail_rem(q,"a");
  s = deq_str(q,0);
  printf("Testing Tail Rem (Empty Queue): %s, Length: %d\n\n",s,deq_len(q));
  free(s);

  /** Test Normal Tail Functions */
  printf("``Normal Tail Testing``\n\n");

  // Test Tail Put
  deq_tail_put(q,"a");
  deq_tail_put(q,"b");
  deq_tail_put(q,"e");
  deq_tail_put(q,"c");
  deq_tail_put(q,"d");
  deq_tail_put(q,"e");
  deq_tail_put(q, "b");
  s = deq_str(q,0);
  printf("Testing Tail Put: %s, Length: %d\n",s,deq_len(q));
  free(s);

  // Test Tail iTh
  s = deq_tail_ith(q, 0);
  printf("Testing Tail iTh (Index: 0): %s\n",s);
  s = deq_tail_ith(q, 4);
  printf("Testing Tail iTh (Index: 4): %s\n",s);

  // Test Tail iTh (Throws Warnings)
  s = deq_tail_ith(q, -1);
  printf("Testing Tail iTh (Index: -1): %s\n", s);
  s = deq_tail_ith(q, deq_len(q));
  printf("Testing Tail iTh (Index: %d): %s\n", deq_len(q),s);

  // Test Tail Get
  s = deq_tail_get(q);
  printf("Testing Tail Get: %s\n",s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);
  s = deq_tail_get(q);
  printf("Testing Tail Get (2nd Time): %s\n",s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);

  // Test Tail Rem
  s = deq_tail_rem(q,"e");
  printf("Testing Tail Rem (Data: e): %s\n", s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);

  // Test Tail Rem (Throws Warnings)
  s = deq_tail_rem(q, "z");
  printf("Testing Tail Rem (Data: z): %s\n",s);
  s = deq_str(q,0);
  printf("Updated Queue: %s, Length: %d\n",s, deq_len(q));
  free(s);

  // Delete Queue
  deq_del(q,0);

  return 0;
}
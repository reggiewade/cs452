#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "deq.h"


char* str_printer(Data d) {
    return strdup((char*)d);
}

int main() {
    // Push and Pop (1)
    printf("Running Test: Push and Pop...\n");
    Deq q = deq_new();
    
    deq_head_put(q, "Middle");
    deq_head_put(q, "Front");
    deq_tail_put(q, "Back");
    
    assert(deq_len(q) == 3);
    assert(strcmp((char*)deq_head_get(q), "Front") == 0);
    assert(strcmp((char*)deq_tail_get(q), "Back") == 0);
    assert(deq_len(q) == 1);
    
    deq_del(q, NULL);
    printf("\tPass!\n");

    // Indexing (ith) (2)
    printf("Running Test: Indexing (ith)...\n");
    q = deq_new();
    deq_tail_put(q, "A");
    deq_tail_put(q, "B");
    deq_tail_put(q, "C");
    
    assert(strcmp((char*)deq_head_ith(q, 0), "A") == 0);
    assert(strcmp((char*)deq_head_ith(q, 2), "C") == 0);
    
    assert(strcmp((char*)deq_tail_ith(q, 0), "C") == 0);
    assert(strcmp((char*)deq_tail_ith(q, 2), "A") == 0);
    
    deq_del(q, NULL);
    printf("\tPass!\n");

    // Specific Removal (rem) (3)
    printf("Running Test: Specific Removal (rem)...\n");
    q = deq_new();
    char *target = "RemoveMe";
    deq_tail_put(q, "Keep1");
    deq_tail_put(q, target);
    deq_tail_put(q, "Keep2");
    
    Data removed = deq_head_rem(q, target);
    assert(removed == target);
    assert(deq_len(q) == 2);
    
    assert(strcmp((char*)deq_head_ith(q, 1), "Keep2") == 0);
    
    printf("\tPass!\n");

    // String Output (4)
    printf("Running Test: String Output...\n");
    char *str = deq_str(q, str_printer);
    printf("\tCurrent Deq: [%s]\n", str);
    free(str);

    deq_del(q, NULL);
/*
    // NULL push (5)
    printf("Running Test: NULL push...\n");
    q = deq_new();
    
    deq_head_put(q, NULL);
    
    deq_del(q, NULL);
    printf("\tPass!\n");
*/
    // Index (i) out of bounds (6)
    printf("Running Test: Index (i) out of bounds...\n");
    q = deq_new();

    deq_tail_put(q, "A");
    deq_tail_put(q, "B");
    deq_tail_put(q, "C");

    assert(deq_head_ith(q, 3) == 0);
    printf("\tPass!\n");

    deq_del(q, NULL);

    // Rem out of bounds (7)
    printf("Running Test: Rem out of bounds...\n");
    q = deq_new();

    deq_tail_put(q, "A");
    deq_tail_put(q, "B");
    deq_tail_put(q, "C");

    assert(deq_head_rem(q, "F") == 0);
    printf("\tPass!\n");

    deq_del(q, NULL);

    // Getting an empty deq (8)
    printf("Running Test: Getting an empty deq...\n");
    q = deq_new();

    assert(deq_head_get(q) == 0);
    printf("\tPass!\n");

    deq_del(q, NULL);

    // Specific Removal: Emptying the list (9)
    printf("Running Test: rem until empty...\n");
    q = deq_new();
    char *last = "Last";
    deq_head_put(q, last);
    
    Data r_data = deq_head_rem(q, last);
    assert(r_data == last);
    assert(deq_len(q) == 0);
    
    deq_head_put(q, "First");
    assert(deq_len(q) == 1);
    assert(strcmp((char*)deq_head_get(q), "First") == 0);
    
    deq_del(q, NULL);
    printf("\tPass!\n");

    printf("All tests passed successfully!\n");

    return 0;
}

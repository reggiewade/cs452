#error "untested: need armv8.1"

// ldaddal DST,SRCR,SRCM
//   DST <- [SRCM];
//   [SRCM] <- DST + SRCR;

extern int atomic_lock(int *lck) {
  int old=1;
  const int one=1;
  asm volatile("   ldadd w2,%[one],[%[lck]];"
               "   cbz   w2,1f;
               "   mov   w2,-1;"
               "   stadd w2,[%[lck]];"
               "   mov   %[old],0;"
               "1: dmb   sy;"
               : // OutputOperands
                 [old]"=m"(old) // m=memory
               : // InputOperands
                 [lck]"r"(lck), // r=register
                 [one]"r"(one)  // r=register
               : // Clobbers
               : "cc");    // flags
  return old;
}

extern void atomic_unlock(int *lck) {
  asm volatile("mov   w2,-1;"
               "stadd w2,[%[lck]];"
               : // OutputOperands
               : // InputOperands
                 [lck]"r"(lck) // r=register
               : // Clobbers
                 "cc");         // flags
}

extern int atomic_test(int *lck) { return *lck; }

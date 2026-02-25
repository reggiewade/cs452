#include "Jobs.h"
#include "deq.h"
#include "error.h"
#include <sys/wait.h>
#include <stdio.h>

extern Jobs newJobs() {
  return deq_new();
}

extern void addJobs(Jobs jobs, Pipeline pipeline) {
  deq_tail_put(jobs,pipeline);
}

extern int sizeJobs(Jobs jobs) {
  return deq_len(jobs);
}

extern void freeJobs(Jobs jobs) {
  deq_del(jobs,freePipeline);
}

extern void reapJobs(Jobs jobs) {
    int i = 0;
    while (i < deq_len(jobs)) {
        Pipeline pipeline = deq_head_ith(jobs, i);
        pid_t pid = getPipelinePid(pipeline);
        int status;
        
        pid_t result = waitpid(pid, &status, WNOHANG);
        
        if (result == pid) {
            deq_head_rem(jobs, pipeline);
            freePipeline(pipeline);
        } else {
            i++;
        }
    }
}
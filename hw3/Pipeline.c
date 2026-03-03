#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Pipeline.h"
#include "deq.h"
#include "error.h"

typedef struct {
  Deq processes;
  int fg;			// not "&"
  pid_t pid;			// for job control
} *PipelineRep;

extern Pipeline newPipeline(int fg) {
  PipelineRep r=(PipelineRep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->processes=deq_new();
  r->pid=0;
  r->fg=fg;
  return r;
}

/**
 * Add a command to the end of the pipeline.
 * @param pipeline the pipeline to add to
 * @param command the command to add
 */
extern void addPipeline(Pipeline pipeline, Command command) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_tail_put(r->processes,command);
}

extern int sizePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  return deq_len(r->processes);
}

/**
 * Execute a pipeline of commands.
 * 
 * This function executes each command in the pipeline in order.
 * If the pipeline is not in the foreground, this function will return
 * immediately after starting the first command. Otherwise, it will wait
 * for each command to finish before starting the next one.
 * 
 * @param pipeline the pipeline of commands to execute
 * @param jobs the jobs list to add the commands to
 * @param jobbed a pointer to an integer indicating whether the command is running in the background
 * @param eof a pointer to an integer indicating whether the shell should exit
 */
static void execute(Pipeline pipeline, Jobs jobs, int *jobbed, int *eof) {
  PipelineRep r = (PipelineRep)pipeline;
  int num_cmds = sizePipeline(r);
  
  int prev_fd = STDIN_FILENO;
  int fd[2];

  for (int i = 0; i < num_cmds && !*eof; i++) {
    int is_last = (i == num_cmds - 1);
    int out_fd = STDOUT_FILENO;

    if (!is_last) {
      if (pipe(fd) == -1) ERROR("pipe() failed");
      out_fd = fd[1];
    }

    execCommand(deq_head_ith(r->processes, i), pipeline, jobs, jobbed, eof, r->fg, prev_fd, out_fd);

    if (prev_fd != STDIN_FILENO) {
      close(prev_fd);
    }
    if (!is_last) {
      close(fd[1]);
      prev_fd = fd[0];
    }
  }

  if (r->fg) {
      for (int i = 0; i < num_cmds; i++) {
          wait(NULL); 
      }
  }
}

extern void execPipeline(Pipeline pipeline, Jobs jobs, int *eof) {
  int jobbed=0;
  execute(pipeline,jobs,&jobbed,eof);
  if (!jobbed)
    freePipeline(pipeline);	// for fg builtins, and such
}

extern void freePipeline(Pipeline pipeline) {
  PipelineRep r=(PipelineRep)pipeline;
  deq_del(r->processes,freeCommand);
  free(r);
}

extern void setPipelinePid(Pipeline pipeline, pid_t pid) {
  ((PipelineRep)pipeline)->pid=pid;
}

extern pid_t getPipelinePid(Pipeline pipeline) {
  return ((PipelineRep)pipeline)->pid;
}
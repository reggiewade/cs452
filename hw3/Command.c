#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/history.h>
#include <fcntl.h>

#include "Command.h"
#include "error.h"

typedef struct {
  char *file;
  char **argv;
  char *infile;
  char *outfile;
} *CommandRep;

#define BIARGS CommandRep r, int *eof, Jobs jobs
#define BINAME(name) bi_##name
#define BIDEFN(name) static void BINAME(name) (BIARGS)
#define BIENTRY(name) {#name,BINAME(name)}

static char *owd=0;
static char *cwd=0;

static void builtin_args(CommandRep r, int n) {
  char **argv=r->argv;
  for (n++; *argv++; n--);
  if (n)
    ERROR("wrong number of arguments to builtin command"); // warn
}

BIDEFN(exit) {
  builtin_args(r,0);
  *eof=1;
  freeJobs(jobs);
}

BIDEFN(pwd) {
  builtin_args(r,0);
  if (!cwd)
    cwd=getcwd(0,0);
  printf("%s\n",cwd);
}

BIDEFN(cd) {
  builtin_args(r,1);
  if (strcmp(r->argv[1],"-")==0) {
    char *twd=cwd;
    cwd=owd;
    owd=twd;
  } else {
    if (owd) free(owd);
    owd=cwd;
    cwd=strdup(r->argv[1]);
  }
  if (cwd && chdir(cwd) == 0) {
    free(cwd);
    cwd = getcwd(0,0);
  }
  else {
    WARN("The specified directory does not exist or is inaccessible"); // warn
  }
    
}

BIDEFN(history) {
  builtin_args(r,0);
  HIST_ENTRY **entries=history_list();
  if (entries)
    for (int i=0; entries[i]; i++)
      printf("%d: %s\n",i+history_base,entries[i]->line);
}

static int builtin(BIARGS) {
  typedef struct {
    char *s;
    void (*f)(BIARGS);
  } Builtin;
  static const Builtin builtins[]={
    BIENTRY(exit),
    BIENTRY(pwd),
    BIENTRY(cd),
    BIENTRY(history),
    {0,0}
  };
  int i;
  for (i=0; builtins[i].s; i++)
    if (!strcmp(r->file,builtins[i].s)) {
      builtins[i].f(r,eof,jobs);
      return 1;
    }
  return 0;
}

static char **getargs(T_words words) {
  int n=0;
  T_words p=words;
  while (p) {
    p=p->words;
    n++;
  }
  char **argv=(char **)malloc(sizeof(char *)*(n+1));
  if (!argv)
    ERROR("malloc() failed");
  p=words;
  int i=0;
  while (p) {
    argv[i++]=strdup(p->word->s);
    p=p->words;
  }
  argv[i]=0;
  return argv;
}

extern Command newCommand(T_words words, char* infile, char* outfile) {
  CommandRep r=(CommandRep)malloc(sizeof(*r));
  if (!r)
    ERROR("malloc() failed");
  r->argv=getargs(words);
  r->file=r->argv[0];
  r->infile = infile ? strdup(infile) : NULL;
  r->outfile = outfile ? strdup(outfile) : NULL;
  return r;
}

static void child(CommandRep r, int fg, int in_fd, int out_fd) {
  int eof=0;
  Jobs jobs=newJobs(); 
  
  // Handle Input Redirection overrides
  if (r->infile) {
    int fd = open(r->infile, O_RDONLY);
    if (fd < 0) { ERROR("Failed to open input file"); exit(1); }
    dup2(fd, STDIN_FILENO);
    close(fd);
  } else if (in_fd != STDIN_FILENO) {
    dup2(in_fd, STDIN_FILENO);
    close(in_fd);
  }
  
  // Handle Output Redirection overrides
  if (r->outfile) {
    // Open for writing, create if it doesn't exist, truncate if it does. Permissions 0644.
    int fd = open(r->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) { ERROR("Failed to open output file"); exit(1); }
    dup2(fd, STDOUT_FILENO);
    close(fd);
  } else if (out_fd != STDOUT_FILENO) {
    dup2(out_fd, STDOUT_FILENO);
    close(out_fd);
  }

  if (builtin(r,&eof,jobs)) {
    exit(0); 
  }
  
  execvp(r->argv[0],r->argv);
  
  ERROR("execvp() failed");
  exit(1);
}

extern void execCommand(Command command, Pipeline pipeline, Jobs jobs,
      int *jobbed, int *eof, int fg, int in_fd, int out_fd) {
      
  CommandRep r=command;
  
  if (fg && sizePipeline(pipeline) <= 1 && builtin(r,eof,jobs))
    return;
    
  if (!fg && !*jobbed) {
      *jobbed=1;
      addJobs(jobs,pipeline);
  }
  
  int pid=fork();
  if (pid==-1)
    ERROR("fork() failed");
    
  if (pid==0) {
    child(r, fg, in_fd, out_fd); 
  }
  else {
    setPipelinePid(pipeline,pid);
    if (!fg) {
      printf("[%d] %d\n", sizeJobs(jobs), pid);
    }
  }
}

extern void freeCommand(Command command) {
  CommandRep r=command;
  char **argv=r->argv;
  while (*argv)
    free(*argv++);
  free(r->argv);
  if (r->infile) free(r->infile);
  if (r->outfile) free(r->outfile);
  free(r);
}

extern void freestateCommand() {
  if (cwd) free(cwd);
  if (owd) free(owd);
}

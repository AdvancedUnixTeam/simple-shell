#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

/* ===================================================================================================
    Shell Stuff
   ===================================================================================================  */

pid_t shell_pgid;
struct termios shell_tmodes;
int shell_terminal;
int shell_is_interactive;

void init_shell();


void launch_process (process *p, 
                      pid_t pgid,
                      int infile, int outfile, int errfile,
                      int foreground);

void launch_job (job *j, int foreground);


/* ===================================================================================================
    Job Control Stuff
   ===================================================================================================  */

/* A process is a single process.  */
typedef struct process
{
  struct process *next;       /* next process in pipeline */
  char **argv;                /* for exec */
  pid_t pid;                  /* process ID */
  char completed;             /* true if process has completed */
  char stopped;               /* true if process has stopped */
  int status;                 /* reported status value */
} process;

/* A job is a pipeline of processes.  */
typedef struct job
{
  struct job *next;           /* next active job */
  char *command;              /* command line, used for messages */
  process *first_process;     /* list of processes in this job */
  pid_t pgid;                 /* process group ID */
  char notified;              /* true if user told about stopped job */
  struct termios tmodes;      /* saved terminal modes */
  int stdin, stdout, stderr;  /* standard i/o channels */
} job;

/* The active jobs are linked into a list.  This is its head.   */
job *first_job = NULL;

/* Find the active job with the indicated pgid.  */
job *find_job (pid_t pgid);

/* Return true if all processes in the job have stopped or completed.  */
int job_is_stopped (job *j);

/* Return true if all processes in the job have completed.  */
int job_is_completed (job *j);

/* Put job j in the foreground.  If cont is nonzero,
   restore the saved terminal modes and send the process group a
   SIGCONT signal to wake it up before we block.  */
void put_job_in_foreground (job *j, int cont);

/* Put a job in the background.  If the cont argument is true, send
   the process group a SIGCONT signal to wake it up.  */
void put_job_in_background (job *j, int cont);

/* Store the status of the process pid that was returned by waitpid.
   Return 0 if all went well, nonzero otherwise.  */
int mark_process_status (pid_t pid, int status);

/* Check for processes that have status information available,
   without blocking.  */
void update_status (void);

/* Check for processes that have status information available,
   blocking until all processes in the given job have reported.  */
void wait_for_job (job *j);

/* Format information about job status for the user to look at.  */
void format_job_info (job *j, const char *status);

/* Notify the user about stopped or terminated jobs.
   Delete terminated jobs from the active job list.  */
void do_job_notification (void);







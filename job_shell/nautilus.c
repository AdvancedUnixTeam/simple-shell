#include "nautilus.h"

/* Make sure the shell is running interactively as the foreground job
   before proceeding. */
void init_shell () {
	/* See if we are running interactively.  */
	shell_terminal = STDIN_FILENO;
	shell_is_interactive = isatty (shell_terminal);

	if (shell_is_interactive) {
		/* Loop until we are in the foreground.  */
		while (tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp ()))
		kill (- shell_pgid, SIGTTIN);

		/* Ignore interactive and job-control signals.  */
		signal (SIGINT, SIG_IGN);
		signal (SIGQUIT, SIG_IGN);
		signal (SIGTSTP, SIG_IGN);
		signal (SIGTTIN, SIG_IGN);
		signal (SIGTTOU, SIG_IGN);
		signal (SIGCHLD, SIG_IGN);

		/* Put ourselves in our own process group.  */
		shell_pgid = getpid ();
		if (setpgid (shell_pgid, shell_pgid) < 0) {
			perror ("Couldn't put the shell in its own process group");
			exit (1);
		}

		/* Grab control of the terminal.  */
		tcsetpgrp (shell_terminal, shell_pgid);

		/* Save default terminal attributes for shell.  */
		tcgetattr (shell_terminal, &shell_tmodes);
	}
}

void launch_process (process *p, 
						pid_t pgid,
                		int infile, int outfile, int errfile,
                		int foreground) {
	pid_t pid;

	if (shell_is_interactive) {
		/* Put the process into the process group and give the process group
		 the terminal, if appropriate.
		 This has to be done both by the shell and in the individual
		 child processes because of potential race conditions.  */
		pid = getpid ();

		if (pgid == 0) pgid = pid;
			setpgid (pid, pgid);
		if (foreground)
			tcsetpgrp (shell_terminal, pgid);

		/* Set the handling for job control signals back to the default.  */
		signal (SIGINT, SIG_DFL);
		signal (SIGQUIT, SIG_DFL);
		signal (SIGTSTP, SIG_DFL);
		signal (SIGTTIN, SIG_DFL);
		signal (SIGTTOU, SIG_DFL);
		signal (SIGCHLD, SIG_DFL);
	}

	/* Set the standard input/output channels of the new process.  */
	if (infile != STDIN_FILENO) {
		dup2 (infile, STDIN_FILENO);
		close (infile);
	}
	if (outfile != STDOUT_FILENO) {
		dup2 (outfile, STDOUT_FILENO);
		close (outfile);
	}
	if (errfile != STDERR_FILENO) {
		dup2 (errfile, STDERR_FILENO);
		close (errfile);
	}

	/* Exec the new process.  Make sure we exit.  */
	execvp (p->argv[0], p->argv);
	perror ("execvp");
	exit (1);
}

void launch_job (job *j, int foreground) {
	process *p;
	pid_t pid;
	int mypipe[2], infile, outfile;

	infile = j->stdin;
	for (p = j->first_process; p; p = p->next) {
		/* Set up pipes, if necessary.  */
		if (p->next) {
			if (pipe (mypipe) < 0) {
				perror ("pipe");
				exit (1);
			}
			outfile = mypipe[1];
		}
		else
			outfile = j->stdout;

		/* Fork the child processes.  */
		pid = fork ();
		if (pid == 0)
			/* This is the child process.  */
			launch_process (p, j->pgid, infile, outfile, j->stderr, foreground);
		else if (pid < 0) {
			/* The fork failed.  */
			perror ("fork");
			exit (1);
		}
		else {
			/* This is the parent process.  */
			p->pid = pid;
			if (shell_is_interactive) {
				if (!j->pgid)
				j->pgid = pid;
				setpgid (pid, j->pgid);
			}
		}

		/* Clean up after pipes.  */
		if (infile != j->stdin)
			close (infile);
		if (outfile != j->stdout)
			close (outfile);

		infile = mypipe[0];
	}

	format_job_info (j, "launched");

	if (!shell_is_interactive)
		wait_for_job (j);
	else if (foreground)
		put_job_in_foreground (j, 0);
	else
		put_job_in_background (j, 0);
}










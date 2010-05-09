/* vim: set cino= fo=croql sw=8 ts=8 sts=0 noet cin fdm=syntax : */

#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <pinktrace/pink.h>

int
main(void)
{
	int status;
	pid_t pid;
	pink_event_t event;

	/* Fork and start tracing. */
	if ((pid = fork()) < 0) {
		perror("fork");
		return EXIT_FAILURE;
	}
	else if (!pid) {
		/* Set up for tracing. */
		if (!pink_trace_me()) {
			perror("pink_trace_me");
			_exit(EXIT_FAILURE);
		}
		/* Stop, so that the parent has the chance to set up options
		 * for tracing. */
		kill(getpid(), SIGSTOP);

		/* Here's the point where you can do whatever the traced child
		 * should do. */
		_exit(EXIT_SUCCESS);
	}
	else {
		waitpid(pid, &status, 0);
		event = pink_event_decide(status);
		assert(event == PINK_EVENT_STOP);

		/* Set up the child for tracing */
		if (!pink_trace_setup(pid, PINK_TRACE_OPTION_SYSGOOD)) {
			perror("pink_trace_setup");
			pink_trace_kill(pid);
			return EXIT_FAILURE;
		}

		/* The child is ready for tracing, nothing interesting in this
		 * example, let the child continue its execution. */
		pink_trace_cont(pid, 0, (char *)1);
		return EXIT_SUCCESS;
	}
}
#include	<signal.h>

/* void (*signal(int sig, void (*func)(int)))(int); */

TrapSignal (action)

	int	(*action)();
{
   if (signal (SIGINT, SIG_IGN) != SIG_IGN)
	signal (SIGINT, action);
   if (signal (SIGQUIT, SIG_IGN) != SIG_IGN)
	signal (SIGQUIT, action);
}

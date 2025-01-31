#include "../../inc/minishell.h"

volatile int ms_signo = 0;

static void	sig_handler(int signo)
{
	ms_signo = signo;
}

void	set_sighandler(void)
{
	struct sigaction	sa;

	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

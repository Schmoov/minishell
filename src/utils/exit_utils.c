#include "../../inc/minishell.h"

int	exit_alloc(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}
int exit_exec(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}

int exit_pipe(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}
#include "../inc/minishell.h"

static bool	is_echo_flag(char *s)
{
	int	i;

	if (s[0] != '-')
		return (false);
	if (s[1] != 'n')
		return (false);
	i = 2;
	while (s[i] == 'n')
		i++;
	return (!s[i]);
}

int	blt_echo(char **argv)
{
	bool	nl;
	int		i;

	nl = true;
	i = 1;
	while (argv[i] && is_echo_flag(argv[i]))
		nl = false;
	while (argv[i])
	{
		write(1, argv[i], ft_strlen(argv[i]));
		i++;
		if (argv[i])
			write(1, " ", 1);
		else if (nl)
			write(1, "\n", 1);
	}

	return (0);
}

bool	path_resolve(t_ms *ms, char **solved, char *path);

int	blt_cd(t_ms *ms, char **argv)
{
	if (!argv[1] || argv[2])
	{
		write(2, "cd expects a single argument\n", 29);
		return (1);
	}
	export(
	

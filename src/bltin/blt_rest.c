#include "../../inc/minishell.h"

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

int	blt_cd(char **argv)
{
	int ret;

	if (!argv[1] || argv[2])
	{
		write(2, "cd expects a single argument\n", 29);
		return (1);
	}
	ret = chdir(argv[1]);
	if (ret)
		perror("cd");
	return (!!ret);
}

int	blt_pwd(void)
{
	char	buf[2048];
	char	*ret;

	ret = getcwd(buf, 2048);
	if (!ret)
	{
		perror("pwd");
		return (1);
	}
	write(1, buf, ft_strlen(buf));
	return (0);
}

int	blt_exit(t_ms *ms)
{
	ms_destroy(ms);
	exit(0);
}

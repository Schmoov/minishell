/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_rest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:16:27 by parden            #+#    #+#             */
/*   Updated: 2025/02/22 17:19:00 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	{
		nl = false;
		i++;
	}
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
	int	ret;

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
	ft_strlcat(buf, "\n", 2048);
	write(1, buf, ft_strlen(buf));
	return (0);
}

int	blt_exit(t_ms *ms)
{
	ms_destroy(ms);
	exit(0);
}

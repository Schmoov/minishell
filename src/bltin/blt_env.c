/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blt_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:16:15 by parden            #+#    #+#             */
/*   Updated: 2025/02/24 18:27:09 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	blt_export_unvalid_id(char *s)
{
	int	s_len;

	s_len = ft_strlen(s);
	write(2, "export: '", 9);
	write(2, s, s_len);
	write(2, "' not a valid identifier\n", 25);
	return (1);
}

int	blt_export_str(t_ms *ms, char *s)
{
	int	eq_pos;
	int	env_pos;

	if (!*s)
		return (0);
	if (!(ft_isalpha(*s) || *s == '_'))
		return (blt_export_unvalid_id(s));
	eq_pos = 1;
	while (s[eq_pos] && s[eq_pos] != '=')
	{
		if (!(ft_isalnum(s[eq_pos]) || s[eq_pos] == '_'))
			return (blt_export_unvalid_id(s));
		eq_pos++;
	}
	if (!s[eq_pos])
		return (0);
	env_pos = spl_find(ms->envp, s, eq_pos + 1);
	if (env_pos == -1)
		spl_append(&ms->envp, s);
	else
		spl_replace(ms->envp, s, env_pos);
	if (!(ms->envp[0]))
		exit_alloc(ms);
	return (0);
}

int	blt_export(t_ms *ms, char **argv)
{
	int		i;
	int		ret;
	bool	fail;

	if (!argv[1])
	{
		blt_env(ms, argv);
		return (1);
	}
	i = 1;
	fail = false;
	while (argv[i])
	{
		ret = blt_export_str(ms, argv[i]);
		if (ret)
			fail = true;
		i++;
	}
	return (fail);
}

int	blt_env(t_ms *ms, char **argv)
{
	int	i;

	if (argv[1])
	{
		write(2, "env doesn't expect arguments\n", 29);
		return (1);
	}
	i = 0;
	while (ms->envp[i])
	{
		write(1, ms->envp[i], ft_strlen(ms->envp[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

int	blt_unset(t_ms *ms, char **argv)
{
	int	i;
	int	env_pos;

	i = 1;
	while (argv[i])
	{
		env_pos = spl_find(ms->envp, argv[i], ft_strlen(argv[i]));
		if (env_pos != -1)
			spl_remove(ms->envp, env_pos);
		i++;
	}
	return (0);
}

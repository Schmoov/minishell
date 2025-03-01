/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:09:34 by lscheupl          #+#    #+#             */
/*   Updated: 2025/03/01 15:33:06 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_empty(char *cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int	redir_len_one(int *redir, char **args, int i)
{
	if (ft_strncmp(args[i], ">", 1) == 0)
	{
		if (args[i + 1] == NULL)
			return (-1);
		redir[1] = redir_out((args[i + 1]));
		if (redir[1] == -1)
			return (redir[1]);
		spl_remove(args, i + 1);
		spl_remove(args, i);
	}
	else if (ft_strncmp(args[i], "<", 1) == 0)
	{
		if (args[i + 1] == NULL)
			return (-1);
		redir[0] = redir_in(args[i + 1]);
		if (redir[0] == -1)
			return (redir[0]);
		spl_remove(args, i + 1);
		spl_remove(args, i);
	}
	return (0);
}

int	redir_len_two(int *redir, char **args, int i)
{
	if (ft_strncmp(args[i], ">>", 2) == 0)
	{
		if (args[i + 1] == NULL)
			return (-1);
		redir[1] = redir_app(args[i + 1]);
		if (redir[1] == -1)
			return (redir[1]);
		spl_remove(args, i + 1);
		spl_remove(args, i);
	}
	return (0);
}

int	redir_handler(int *redir, char **args)
{
	int	i;
	int	fd;

	i = 0;
	fd = -69;
	if (is_empty(args[i]))
		return (0);
	while (args[i] != NULL)
	{
		if ((args[i][0] == '>' || args[i][0] == '<') && ft_strlen(args[i]) == 1)
		{
			if (redir_len_one(redir, args, i) == -1)
				return (-1);
			i--;
		}
		else if (ft_strcmp(args[i], ">>") == 0 && ft_strlen(args[i]) == 2)
		{
			if (redir_len_two(redir, args, i) == -1)
				return (-1);
			i--;
		}
		i++;
	}
	return (0);
}

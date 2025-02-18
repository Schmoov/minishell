/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 17:09:34 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/18 20:31:12 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_builtin(t_node_cmd **node, t_ms *ms)
{
	(*node)->bltin = is_builtin((*node)->args[0]);

	if ((*node)->bltin == E_EMPTY)
		return (2);
	if ((*node)->bltin == E_EXIT)
		exit_exec(ms);
	else if ((*node)->bltin == E_CD)
		return (blt_cd((*node)->args));
	else if ((*node)->bltin == E_PWD)
		return (blt_pwd());
	else if ((*node)->bltin == E_ECHO)
		return (blt_echo((*node)->args));
	else if ((*node)->bltin == E_EXPORT)
		return (blt_export(ms, (*node)->args));
	else if ((*node)->bltin == E_UNSET)
		return (blt_unset(ms, (*node)->args));
	else if ((*node)->bltin == E_ENV)
		return (blt_env(ms, (*node)->args));
	return (0);
}

int is_empty(char *cmd)
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

int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (E_EMPTY);
	if (cmd[0] == '\0')
		return (E_EMPTY);
	if (is_empty(cmd))
		return (E_EMPTY);
	if (ft_strncmp(cmd, "exit", 4) == 0)
		return (E_EXIT);
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (E_CD);
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return (E_PWD);
	else if (ft_strncmp(cmd, "echo", 4) == 0)
		return (E_ECHO);
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (E_EXPORT);
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return (E_UNSET);
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return (E_ENV);
	return (E_NOTBLTIN);
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

int	redir_len_two(int *redir, char **args, int i, t_ms *ms)
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
	else if (ft_strncmp(args[i], "<<", 2) == 0)
	{
		if (args[i + 1] == NULL)
			return (0);
		redir[0] = redir_hd(args[i + 1], ms);
		if (redir[0] == -1)
			return (redir[0]);
		
		spl_remove(args, i + 1);
		spl_remove(args, i);
	}
	return (0);
}

int	redir_handler(int *redir, char **args, t_ms *ms)
{
	int	i;
	int	fd;

	i = 0;
	fd = -1;
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
		else if ((ft_strcmp(args[i],">>") == 0 || ft_strcmp(args[i],"<<") == 0) && ft_strlen(args[i]) == 2)
		{
			if (redir_len_two(redir, args, i, ms) == -1)
				return (-1);
			i--;
		}
		i++;
	}
	return (0);
}

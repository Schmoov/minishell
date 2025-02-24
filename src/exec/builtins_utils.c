/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:42:55 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/24 18:55:26 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	dup_ms(t_ms *ms)
{
	dup2(ms->fd[0], STDIN_FILENO);
	dup2(ms->fd[1], STDOUT_FILENO);
}

void	restore_fd(int *tmp, int *redir, t_ms *ms)
{
	dup2(tmp[0], STDIN_FILENO);
	dup2(tmp[1], STDOUT_FILENO);
	close(tmp[0]);
	close(tmp[1]);
	close_all(redir, ms);
}

int	exec_builtin(t_node_cmd *node, t_ms *ms)
{
	int	tmp[2];

	node->bltin = is_builtin(node->args[0]);
	if (node->bltin == E_EXIT)
		blt_exit(ms, node->args);
	tmp[0] = dup(STDIN_FILENO);
	tmp[1] = dup(STDOUT_FILENO);
	dup_ms(ms);
	redir_executions(node->redir);
	if (node->bltin == E_EMPTY)
		ms->status = 0;
	else if (node->bltin == E_CD)
		ms->status = blt_cd(node->args);
	else if (node->bltin == E_PWD)
		ms->status = blt_pwd();
	else if (node->bltin == E_ECHO)
		ms->status = blt_echo(node->args);
	else if (node->bltin == E_EXPORT)
		ms->status = blt_export(ms, node->args);
	else if (node->bltin == E_UNSET)
		ms->status = blt_unset(ms, node->args);
	else if (node->bltin == E_ENV)
		ms->status = blt_env(ms, node->args);
	restore_fd(tmp, node->redir, ms);
	return (ms->status);
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

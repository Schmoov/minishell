/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:42:55 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/26 18:13:42 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	restore_fd(int *tmp, int *redir)
{
	dup2(tmp[0], STDIN_FILENO);
	dup2(tmp[1], STDOUT_FILENO);
	close(tmp[0]);
	close(tmp[1]);
	if (redir[0] != -1)
		close(redir[0]);
	if (redir[1] != -1)
		close(redir[1]);
}

int	exec_builtin(t_node_cmd *node, t_ms *ms)
{
	int	tmp[2];

	node->bltin = is_builtin(node->args[0]);
	if (node->bltin == E_EXIT)
		blt_exit(ms, node->args);
	tmp[0] = dup(STDIN_FILENO);
	tmp[1] = dup(STDOUT_FILENO);
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
	restore_fd(tmp, node->redir);
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
	if (ft_strcmp(cmd, "exit") == 0)
		return (E_EXIT);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (E_CD);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (E_PWD);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (E_ECHO);
	else if (ft_strcmp(cmd, "export") == 0)
		return (E_EXPORT);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (E_UNSET);
	else if (ft_strcmp(cmd, "env") == 0)
		return (E_ENV);
	return (E_NOTBLTIN);
}

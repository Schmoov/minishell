/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:33:31 by leonel            #+#    #+#             */
/*   Updated: 2025/02/10 23:24:01 by leonel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_execve(char *path, char **tab_arg, char **envp)
{
	execve(path, tab_arg, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	redir_executions(int *redir, t_ms *ms)
{
	if (redir[0] != -1)
		dup2(redir[0], ms->fd[0]);
	if (redir[1] != -1)
		dup2(redir[1], ms->fd[1]);
}

int	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	t_node_cmd	*node;
	char		*path;
	pid_t		pid;

	node = &(root->cmd);
	node->args = expand_expand(ft_convert_pos_to_string(input, node->start,
				node->end), ms);
	node->redir[0] = -1;
	node->redir[1] = -1;
	redir_handler(node->redir, node->args);
	node->bltin = is_builtin(node->args[0]);
	if (node->bltin != E_NOTBLTIN)
	{
		ms_close_fd(ms);
		return (exec_builtin(&node, ms));
	}
	path = ft_find_path(ms, node->args);
	if (path == NULL)
	{
		fprintf(stderr, "command not found: %s\n", node->args[0]);
		exit_exec(ms);
		ms_close_fd(ms);
		return (ms->status = 127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (exit_fork(ms));
	}
	if (pid == 0)
	{
		redir_executions(node->redir, ms);
		ms_close_fd(ms);
		if (node->redir[0] != -1)
			close(node->redir[0]);
		if (node->redir[1] != -1)
			close(node->redir[1]);
		ft_execve(path, node->args, ms->envp);
		exit_exec(ms);
	}
	if (node->redir[0] != -1)
		close(node->redir[0]);
	if (node->redir[1] != -1)
		close(node->redir[1]);
	close(ms->fd[0]);
	close(ms->fd[1]);
	waitpid(pid, &ms->status, 0);
	free(path);
	if (WIFSIGNALED(ms->status))
		return (128 + WTERMSIG(ms->status));
	return (WEXITSTATUS(ms->status));
}

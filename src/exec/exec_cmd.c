/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:33:31 by leonel            #+#    #+#             */
/*   Updated: 2025/02/12 22:34:41 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_execve(char *path, t_node_cmd *node, t_ms *ms)
{
	redir_executions(node->redir, ms);
	close_all(node->redir, ms);
	execve(path, node->args, ms->envp);
	perror("execve");
	exit_exec(ms);
}

void	redir_executions(int *redir, t_ms *ms)
{
	if (redir[0] != -1)
		dup2(redir[0], STDIN_FILENO);
	if (redir[1] != -1)
		dup2(redir[1], STDOUT_FILENO);
}

void	close_all(int *redir, t_ms *ms)
{
	if (redir[0] != -1)
		close(redir[0]);
	if (redir[1] != -1)
		close(redir[1]);
	ms_close_fd(ms);
}

int	exec_cmd(char *input, t_ast *root, t_ms *ms)
{
	t_node_cmd	*node;
	char		*path;
	pid_t		pid;

	node = &(root->cmd);
	node->args = to_expansion(pos_to_string(input, node->start, node->end), ms);
	// if (redir_handler(node->redir, node->args, ms) != 0)
	// 	return (write(2, "redir failed\n", 13), close_all(node->redir, ms), ms->status = 1);
	if (node->args[0] == NULL)
		return (close_all(node->redir, ms), ms->status = 0);
	if (is_builtin(node->args[0]) != E_NOTBLTIN)
		return (close_all(node->redir, ms), exec_builtin(&node, ms));
	if (node->args[0] == NULL)
		return (0);
	path = ft_find_path(ms, node->args);
	if (path == NULL)
		return (fprintf(stderr, "command not found\n"),
			close_all(node->redir, ms), ms->status = 127);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), exit_fork(ms));
	if (pid == 0)
		ft_execve(path, node, ms);
	close_all(node->redir, ms);
	waitpid(pid, &ms->status, 0);
	free(path);
	if (WIFSIGNALED(ms->status))
		return (128 + WTERMSIG(ms->status));
	return (WEXITSTATUS(ms->status));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:00 by leonel            #+#    #+#             */
/*   Updated: 2025/02/11 20:37:00 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <stdio.h>

int init_pipe(t_node_pip *node, t_ms *ms)
{
	int i;

	i = 0;
	node->pip_redir = malloc((sizeof (int [2]) * node->pip_len));
	if (node->pip_redir == NULL)
		return (perror("malloc"), -1);
	while (i < node->pip_len - 1)
	{
		if (pipe(node->pip_redir[i]) == -1)
			return (perror("pipe"), -1);
		i++;
	}
	return (0);
}
// node->pip_redir = malloc((sizeof (int [2]) * node->pip_len));
// 	while (i < node->pip_len)
// 	{
// 		if (pipe(node->pip_redir[i]) == -1)
// 		{
// 			perror("pipe");
// 			return (exit_pipe(ms));
// 		}
// 		i++;
// 	}

void close_pip_redir(int *i, int *j)
{
	close(i[0]);
	close(i[1]);
	*j = *j + 1;
}

void pip_dup_handler(t_node_pip *node, int i, t_ms *ms)
{
	if (i != 0)
	{
		// dprintf(2, "before dup2 %d %d\n", node->pip_redir[i - 1][0], ms->fd[0]);
		dup2(node->pip_redir[i - 1][0], 0);
		// dprintf(2 ,"dup2 %d %d\n", node->pip_redir[i - 1][0], ms->fd[0]);
		close(node->pip_redir[i - 1][0]);
	}
	if (i != node->pip_len - 1)
	{
		// dprintf(2, "before 2 dup2 %d %d\n", node->pip_redir[i][1], ms->fd[1]);
		dup2(node->pip_redir[i][1], 1);
		// dprintf(2, "dup2 2 %d %d\n", node->pip_redir[i][1], ms->fd[1]);
		close(node->pip_redir[i][1]);
	}
}

void handle_child_process(t_node_pip *node, int i, char *input, t_ms *ms)
{
	int j;

	j = 0;
	pip_dup_handler(node, i, ms);
    while (j < node->pip_len)
        close_pip_redir(node->pip_redir[j], &j);
    free(node->pip_redir);
    ms->status = exec_general(input, node->piped[i], ms);
    exit_pipe(ms);
}

int	exec_pip(char *input, t_ast *root, t_ms *ms)
{
	int			i;
	t_node_pip	*node;
	pid_t		pid;

	i = 0;
	node = &(root->pip);
	// dprintf(2, "ms->fd[0] = %d\n", ms->fd[0]);
	// dprintf(2, "ms->fd[1] = %d\n", ms->fd[1]);
	if (init_pipe(node, ms) == -1)
		return (exit_pipe(ms));	
	while (i < node->pip_len)
	{
		dprintf(2, "i = %d\n", i);
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (exit_pipe(ms));
		}
		if (pid == 0)
			handle_child_process(node, i, input, ms);
		i++;
	}
	i = 0;
	while (i < node->pip_len)
		close_pip_redir(node->pip_redir[i], &i);
	i = 0;
	while (i < node->pip_len)
	{
		waitpid(pid, NULL, 0);
		i++;
	}
	ms_close_fd(ms);
	free(node->pip_redir);
	return (ms->status);
}

int	exec_grp(char *input, t_ast *root, t_ms *ms)
{
	t_node_grp	*node;

	node = &(root->grp);
	return (exec_general(input, node->next, ms));
}

int	exec_logic(char *input, t_ast *root, t_ms *ms)
{
	t_node_logic	*node;
	int				status;

	node = &(root->logic);
	status = exec_general(input, node->left, ms);
	if (node->is_and)
	{
		if (status == 0)
		{
			status = exec_general(input, node->right, ms);
		}
	}
	else
	{
		if (status != 0)
			status = exec_general(input, node->right, ms);
	}
	return (status);
}

int	exec_general(char *input, t_ast *root, t_ms *ms)
{
	t_node_type	node_type;

	node_type = root->type;
	if (node_type == E_CMD)
		return (ms->status = exec_cmd(input, root, ms));
	else if (node_type == E_LOGIC)
		return (ms->status = exec_logic(input, root, ms));
	else if (node_type == E_PIP)
		return (ms->status = exec_pip(input, root, ms));
	else
		return (ms->status = exec_grp(input, root, ms));
}

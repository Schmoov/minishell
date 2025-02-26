/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 17:41:00 by leonel            #+#    #+#             */
/*   Updated: 2025/02/26 15:43:57 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	end_pip(t_node_pip *node, pid_t *pid, t_ms *ms)
{
	int	i;

	i = 0;
	while (i < node->pip_len - 1)
		close_pip_redir(node->pip_redir[i++]);
	i = 0;
	while (i < node->pip_len)
		waitpid(pid[i++], &ms->status, 0);
	free(node->pip_redir);
	free(pid);
	ms_close_fd(ms);
}

int	exec_pip(char *input, t_ast *root, t_ms *ms)
{
	int			i;
	t_node_pip	*node;
	pid_t		*pid;

	i = 0;
	node = &(root->pip);
	if (init_pipe(node) == -1)
		return (exit_pipe(ms));
	pid = malloc(sizeof(pid_t) * node->pip_len);
	while (i < node->pip_len)
	{
		pid[i] = fork();
		if (pid[i] == -1)
			return (perror("fork"), exit_pipe(ms));
		if (pid[i] == 0)
			return (free(pid), handle_child_process(node, i, input, ms), 69);
		i++;
	}
	end_pip(node, pid, ms);
	if (WIFSIGNALED(ms->status))
		return (128 + WTERMSIG(ms->status));
	return (WEXITSTATUS(ms->status));
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

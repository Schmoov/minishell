/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:36:09 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/24 17:14:32 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	init_pipe(t_node_pip *node)
{
	int	i;

	i = 0;
	node->pip_redir = malloc((sizeof(int [2]) * node->pip_len - 1));
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

void	close_pip_redir(int *redir)
{
	close(redir[0]);
	close(redir[1]);
}

void	pip_dup_handler(t_node_pip *node, int i)
{
	if (i != 0)
	{
		dup2(node->pip_redir[i - 1][0], STDIN_FILENO);
		close(node->pip_redir[i - 1][0]);
	}
	if (i != node->pip_len - 1)
	{
		dup2(node->pip_redir[i][1], STDOUT_FILENO);
		close(node->pip_redir[i][1]);
	}
}

void	handle_child_process(t_node_pip *node, int i, char *input, t_ms *ms)
{
	int	j;

	j = 0;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	pip_dup_handler(node, i);
	while (j < node->pip_len - 1)
	{
		close(node->pip_redir[j][0]);
		close(node->pip_redir[j][1]);
		j++;
	}
	free(node->pip_redir);
	ms->status = exec_general(input, node->piped[i], ms);
	exit_pipe(ms);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:37:14 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/26 17:43:57 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	redir_executions(int *redir)
{
	if (redir[0] != -1)
	{
		dup2(redir[0], STDIN_FILENO);
		close(redir[0]);
	}
	if (redir[1] != -1)
	{
		dup2(redir[1], STDOUT_FILENO);
		close(redir[1]);
	}
}

void	close_all(int *redir)
{
	if (redir[0] != -1)
		close(redir[0]);
	if (redir[1] != -1)
		close(redir[1]);
	redir[0] = -1;
	redir[1] = -1;
}

int	is_redir_before(char *input, t_node_cmd *node)
{
	int	i;

	i = node->start;
	while (i < node->end && input[i] == ' ')
		i++;
	if (input[i] == '>' || input[i] == '<')
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

char	*get_next_word(char *input, int i, t_node_cmd *node)
{
	int	j;

	while (input[i] == ' ')
		i++;
	j = i;
	while (input[i] != ' ' && input[i] != '\0')
		i++;
	node->start = i;
	return (pos_to_string(input, j, node->start));
}

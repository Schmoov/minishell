/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_grp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:08:57 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/17 18:05:42 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int what_type(char *str, int i)
{
	if (str[i] == '>')
	{
		if (str[i + 1] == '>')
			return (2);
		else
			return (1);
	}
	if (str[i] == '<')
	{
		if (str[i + 1] == '<')
			return (3);
		else
			return (0);
	}
	return (0);
}

int redir_e(char *str, int start, int end, t_node_grp *node, t_ms *ms)
{
	int	i;

	i = start;
	
	while (i < end)
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if (str[i + 1] == str[i])
			{
				if (redir(get_next_word(str, i + 2, node), node->redir[1], ms, what_type(str, i)))
					return (EXIT_FAILURE);
				i++;
			}
			else
			{
				if (redir(get_next_word(str, i + 1, node), node->redir[1], ms, what_type(str, i)))
					return (EXIT_FAILURE);
			}
		}	
		i++;
	}
	return (EXIT_SUCCESS);
}

int	redir_grp(char *input, t_node_grp *node, t_ms *ms)
{
	int	i;

	i = node->start;
	while (i < node->end && input[i] != '(')
		i++;
	if (redir_e(input, node->start, i, node, ms) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = node->end;
	while (i > node->start && input[i] != ')')
		i--;
	if (redir_e(input, i, node->end, node, ms) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}


int	exec_grp(char *input, t_ast *root, t_ms *ms)
{
	t_node_grp	*node;
	
	node = &(root->grp);
	
	redir_grp(input, node, ms);
	redir_executions(node->redir, ms);
	return (exec_general(input, node->next, ms));
}
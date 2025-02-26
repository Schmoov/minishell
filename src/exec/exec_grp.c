/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_grp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 17:08:57 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/26 17:51:43 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	what_type(char *str, int i)
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

char	*get_next_word_grp(char *input, int i, t_node_grp *node)
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

int	redir_e(char *str, int *pos, t_node_grp *node, t_ms *ms)
{
	int		i;
	char	*word;

	i = pos[0];
	while (i < pos[1])
	{
		if (str[i] == '>' || str[i] == '<')
		{
			if (str[i + 1] == str[i])
			{
				word = get_next_word_grp(str, i + 2, node);
				if (redir(word, node->redir, ms, what_type(str, i)))
					return (free(word), EXIT_FAILURE);
				i++;
			}
			else
			{
				word = get_next_word_grp(str, i + 1, node);
				if (redir(word, node->redir, ms, what_type(str, i)))
					return (free(word), EXIT_FAILURE);
			}
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int	redir_grp(char *input, t_node_grp *node, t_ms *ms)
{
	int	i;
	int	pos[2];
	int	status;

	i = node->start;
	while (i < node->end && input[i] != '(')
		i++;
	pos[0] = node->start;
	pos[1] = i;
	status = redir_e(input, pos, node, ms);
	if (status == EXIT_FAILURE)
		return (EXIT_FAILURE);
	i = node->end;
	while (i > node->start && input[i] != ')')
		i--;
	pos[0] = i;
	pos[1] = node->end;
	status = redir_e(input, pos, node, ms);
	if (status == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	exec_grp(char *input, t_ast *root, t_ms *ms)
{
	t_node_grp	*node;
	int			tmp;
	int			tmp2;

	node = &(root->grp);
	tmp = dup(STDIN_FILENO);
	tmp2 = dup(STDOUT_FILENO);
	ms->status = redir_grp(input, node, ms);
	redir_executions(node->redir);
	dup2(tmp, STDIN_FILENO);
	close(tmp);
	dup2(tmp2, STDOUT_FILENO);
	close(tmp2);
	ms->status = exec_general(input, node->next, ms);
	return (ms->status);
}

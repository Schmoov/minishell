/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:02:49 by parden            #+#    #+#             */
/*   Updated: 2024/11/27 18:40:13 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ms_parse(t_ms *ms)
{
	ms->ast = parse_rec(ms->input, 0, ft_strlen(ms->input));
}

t_ast	*parse_rec(char *input, int start, int end)
{
	t_node_type	node_type;

	node_type = parse_node_type(input, start, end);
	if (node_type == E_CMD)
		return (parse_cmd(input, start, end));
	else if (node_type == E_LOGIC)
		return (parse_logic(input, start, end));
	else if (node_type == E_PIP)
		return (parse_pip(input, start, end));
	else
		return (parse_grp(input, start, end));
}

t_node_type	parse_node_type(char *input, int start, int end)
{
	t_node_type	res;
	int			i;

	res = E_CMD;
	i = start;
	while (i < end)
	{
		if (!ft_strncmp(input + i, "&&", 2)
			|| !ft_strncmp(input + i, "||", 2))
			return (E_LOGIC);
		else if (input[i] == '|')
			res = E_PIP;
		else if (input[i] == '(')
		{
			if (res == E_CMD)
				res = E_GRP;
			i = close_paren(input, i, end);
		}
		else if (input[i] == '\'' || input[i] == '"')
			i = close_quote(input, i, end);
		++i;
	}
	return (res);
}

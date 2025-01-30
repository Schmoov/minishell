/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:02:49 by parden            #+#    #+#             */
/*   Updated: 2024/11/27 18:38:33 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_ast	*parse_cmd(char *input, int start, int end)
{
	(void) input;
	return (ast_create_cmd(start, end));
}

t_ast	*parse_grp(char *input, int start, int end)
{
	t_ast	*res;
	t_ast	*next;

	res = ast_create_grp(start, end);
	if (!res)
		return (NULL);
	next = parse_rec(input,
			find_first_paren(input, start, end) + 1,
			find_last_paren(input, start, end));
	if (!next)
		return (ast_destroy(res), NULL);
	res->grp.next = next;
	return (res);
}

t_ast	*parse_logic(char *input, int start, int end)
{
	t_ast	*res;
	t_ast	*left;
	t_ast	*right;
	int		pos;

	pos = find_last_logic(input, start, end);
	res = ast_create_logic(start, end, input[pos] == '&');
	if (!res)
		return (NULL);
	left = parse_rec(input, start, pos);
	if (!left)
		return (ast_destroy(res), NULL);
	res->logic.left = left;
	right = parse_rec(input, pos + 2, end);
	if (!right)
		return (ast_destroy(res), NULL);
	res->logic.right = right;
	return (res);
}

t_ast	*parse_pip(char *input, int start, int end)
{
	t_ast	*res;
	int		pip_len;
	int		*pos;
	int		i;

	pip_len = count_pip(input, start, end);
	res = ast_create_pip(start, end, pip_len);
	if (!res)
		return (NULL);
	pos = malloc((pip_len + 1) * sizeof(int));
	if (!pos)
		return (ast_destroy(res), NULL);
	find_pip(input, start, end, pos);
	i = 0;
	while (i < pip_len)
	{
		res->pip.piped[i] = parse_rec(input, pos[i] + 1, pos[i + 1]);
		if (!res->pip.piped[i])
			return (free(pos), ast_destroy(res), NULL);
		i++;
	}
	free(pos);
	return (res);
}

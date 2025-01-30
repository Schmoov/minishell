/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:00:07 by parden            #+#    #+#             */
/*   Updated: 2024/11/27 18:37:36 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	find_first_paren(char *input, int start, int end)
{
	int	i;

	(void) end;
	i = start;
	while (input[i] != '(')
	{
		if (input[i] == '\'' || input[i] == '"')
			i = close_quote(input, i, end);
		i++;
	}
	return (i);
}

int	find_last_paren(char *input, int start, int end)
{
	int	i;

	i = end - 1;
	while (input[i] != ')')
	{
		if (input[i] == '\'' || input[i] == '"')
			i = rclose_quote(input, start, i + 1);
		i--;
	}
	return (i);
}

int	find_last_logic(char *input, int start, int end)
{
	int	i;

	i = end - 1;
	while (ft_strncmp(input + i, "&&", 2)
		&& ft_strncmp(input + i, "||", 2))
	{
		if (input[i] == '\'' || input[i] == '"')
			i = rclose_quote(input, start, i + 1);
		else if (input[i] == ')')
			i = rclose_paren(input, start, i + 1);
		i--;
	}
	return (i);
}

int	count_pip(char *input, int start, int end)
{
	int	i;
	int	res;

	i = start;
	res = 0;
	while (i < end)
	{
		if (input[i] == '\'' || input[i] == '"')
			i = close_quote(input, i, end);
		else if (input[i] == '(')
			i = close_paren(input, i, end);
		else if (!ft_strncmp(input + i, "||", 2))
			i++;
		else if (input[i] == '|')
			res++;
		i++;
	}
	return (res + 1);
}

void	find_pip(char *input, int start, int end, int *pos)
{
	int	i;
	int	i_pos;

	i_pos = 0;
	pos[i_pos] = start - 1;
	i_pos++;
	i = start;
	while (i < end)
	{
		if (input[i] == '\'' || input[i] == '"')
			i = close_quote(input, i, end);
		else if (input[i] == '(')
			i = close_paren(input, i, end);
		else if (!ft_strncmp(input + i, "||", 2))
			i++;
		else if (input[i] == '|')
			pos[i_pos++] = i;
		i++;
	}
	pos[i_pos] = end;
}

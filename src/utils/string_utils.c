/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:00:07 by parden            #+#    #+#             */
/*   Updated: 2024/11/27 18:37:36 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*
 *	close_X returns the position of the closing X
 *	rclose_X returns the position where X was opened
 *
 *	No protections for walking out of the string (i < end)
 *	It should never happen, if it does crashing will help
 *	us figure out where the actual bug is.
*/

int	close_quote(char *input, int start, int end)
{
	int	i;

	(void) end;
	i = start + 1;
	while (input[i] != input[start])
		i++;
	return (i);
}

int	rclose_quote(char *input, int start, int end)
{
	int	i;

	(void) start;
	i = end - 2;
	while (input[i] != input[end - 1])
		i--;
	return (i);
}

int	close_paren(char *input, int start, int end)
{
	int	i;
	int	nested;

	(void) end;
	i = start;
	nested = 1;
	while (nested)
	{
		i++;
		if (input[i] == '(')
			nested++;
		else if (input[i] == ')')
			nested--;
		else if (input[i] == '\'' || input[i] == '"')
			i = close_quote(input, i, end);
	}
	return (i);
}

int	rclose_paren(char *input, int start, int end)
{
	int	i;
	int	nested;

	i = end - 1;
	nested = 1;
	while (nested)
	{
		i--;
		if (input[i] == ')')
			nested++;
		else if (input[i] == '(')
			nested--;
		else if (input[i] == '\'' || input[i] == '"')
			i = rclose_quote(input, start, i + 1);
	}
	return (i);
}

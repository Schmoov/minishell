/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:31:17 by leonel            #+#    #+#             */
/*   Updated: 2025/02/11 18:36:18 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_clean_dollar(char *res)
{
	int	j;

	j = 1;
	while (res[j] != '\0')
	{
		res[j - 1] = res[j];
		j++;
	}
	res[j - 1] = '\0';
}

int	skip_single_quote(char *res, int i)
{
	i++;
	while (res[i] != '\'')
		i++;
	return (i);
}

int	skip_to(char *input, int i, char c)
{
	i++;
	while (input[i] != c)
		i++;
	return (i);
}

void	ft_clean_digit_dollar(char *res, int i)
{
	int	j;

	j = i + 2;
	while (res[j] != '\0')
	{
		res[j - 2] = res[j];
		j++;
	}
	res[j - 2] = '\0';
}

int	is_in_quote(char *res, int i)
{
	if (i == 0 && res[i + 1] == ' ')
		return (1);
	if (i > 0)
	{
		if (res[i - 1] == '\"' && res[i + 1] == '\"')
			return (1);
		if (res[i - 1] == ' ' && res[i + 1] == ' ')
			return (1);
	}
	return (0);
}

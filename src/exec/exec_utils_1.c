/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:02:12 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/21 17:05:08 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_clean_path(char **tab_path)
{
	int	i;
	int	j;

	i = 0;
	j = 5;
	while (tab_path[0][j] != '\0')
	{
		tab_path[0][i] = tab_path[0][j];
		i++;
		j++;
	}
	tab_path[0][i] = '\0';
}

char	*pos_to_string(char *input, int start, int end)
{
	char	*res;
	int		i;
	int		j;

	if (start >= end)
	{
		res = ft_calloc(1, sizeof(char));
		res[0] = '\0';
		return (res);
	}
	res = ft_calloc((end - start + 1), sizeof(char));
	i = 0;
	j = start;
	while (i < end - start)
	{
		res[i++] = input[j++];
	}
	res[end - start] = '\0';
	return (res);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = (char *)malloc(n + 1 * sizeof(char));
	if (!dup)
		return (NULL);
	dup = (char *)ft_memcpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
}

size_t	where_is_dollar(char *input, size_t i)
{
	while (input[i])
	{
		if (input[i] == '$')
			return (i);
		i++;
	}
	return (i);
}

size_t	where_dollar_end(char *input, size_t i)
{
	i++;
	if (input[i] == '?')
		return (i + 1);
	if (input[i] == '_' || ft_isalpha(input[i]))
		i++;
	else
		return (i);
	while (ft_isalnum(input[i]) || input[i] == '_')
		i++;
	return (i);
}

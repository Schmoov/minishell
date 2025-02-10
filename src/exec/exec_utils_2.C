/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_2.C                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 17:31:17 by leonel            #+#    #+#             */
/*   Updated: 2025/02/07 17:31:32 by leonel           ###   ########.fr       */
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

char	**ft_isolate_path(t_ms *ms)
{
	char	*path;
	char	**tab_path;
	int		i;

	i = 0;
	while (ms->envp[i])
	{
		if (ms->envp[i][0] == 'P' && ms->envp[i][1] == 'A'
			&& ms->envp[i][2] == 'T' && ms->envp[i][3] == 'H')
			path = ft_strdup(ms->envp[i]);
		i++;
	}
	tab_path = ft_split(path, ':');
	ft_clean_path(tab_path);
	free(path);
	return (tab_path);
}
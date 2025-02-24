/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 16:59:13 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/24 18:51:06 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	valid_star(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			i = skip_to(str, i, str[i]);
		if (str[i] == '*')
			return (true);
		i++;
	}
	return (false);
}

void	expanding_execution(char ***tmp, int *j, t_ms *ms)
{
	if (ft_strchr((*tmp)[(*j)], '$'))
		ft_expander(ms, tmp, j, (*tmp)[(*j)]);
	if (ft_strchr((*tmp)[(*j)], '*'))
		star_handler(tmp, j, (*tmp)[(*j)]);
}

void	end_expander(char **tab, int tab_index, char *to_be_expanded, char *tmp)
{
	spl_replace(tab, to_be_expanded, tab_index);
	if (ft_strcmp(to_be_expanded, tmp) != 0)
	{
		free(to_be_expanded);
	}
	free(tmp);
}

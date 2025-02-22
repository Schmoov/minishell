/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spl_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:03:41 by parden            #+#    #+#             */
/*   Updated: 2025/02/22 14:50:19 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**spldup(char **spl)
{
	int		i;
	char	**res;

	i = 0;
	while (spl[i])
		i++;
	res = ft_calloc(i + 1, sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	while (spl[i])
	{
		res[i] = ft_strdup(spl[i]);
		if (!res[i])
			return (ft_free_split(spl), NULL);
		i++;
	}
	return (res);
}

void	spl_append(char ***spl, char *new)
{
	int		i;
	char	**res;

	i = 0;
	while ((*spl)[i])
		i++;
	res = ft_calloc(i + 2, sizeof(char *));
	if (!res)
		return (ft_free_split(*spl));
	i = 0;
	while ((*spl)[i])
	{
		res[i] = ft_strdup((*spl)[i]);
		if (!res[i])
			return (ft_free_split(*spl), ft_free_split(res));
		i++;
	}
	res[i] = ft_strdup(new);
	if (!res[i])
		return (ft_free_split(*spl), ft_free_split(res));
	ft_free_split(*spl);
	*spl = res;
}

//Finds a match on the len first char
int	spl_find(char **spl, char *s, int len)
{
	int	i;

	i = 0;
	while (spl[i])
	{
		if (!ft_strncmp(spl[i], s, len))
			return (i);
		i++;
	}
	return (-1);
}

void	spl_replace(char **spl, char *new, int pos)
{
	char	*dup;

	dup = ft_strdup(new);
	if (!dup)
		ft_free_split(spl);
	free(spl[pos]);
	spl[pos] = dup;
}

void	spl_remove(char **spl, int pos)
{
	free(spl[pos]);
	while (spl[pos + 1])
	{
		spl[pos] = spl[pos + 1];
		pos++;
	}
	spl[pos] = NULL;
}

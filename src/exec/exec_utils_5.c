/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:35:28 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/22 16:50:21 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_star_good(char *input, int i)
{
	if (input[i] == '*' && (input[i + 1] == ' ' || input[i + 1] == '\0'))
	{
		if (i == 0)
			return (1);
		if (input[i - 1] == ' ')
			return (1);
	}
	return (0);
}

void	single_layer_quotes_remover(char *str)
{
	int	i;
	int	j;
	int	single_quote;
	int	double_quote;

	i = 0;
	j = 0;
	single_quote = 0;
	double_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && double_quote % 2 == 0)
			single_quote++;
		else if (str[i] == '\"' && single_quote % 2 == 0)
			double_quote++;
		else if (str[i] == '\'' && double_quote % 2 == 1)
			str[j++] = str[i];
		else if (str[i] == '\"' && single_quote % 2 == 1)
			str[j++] = str[i];
		else if (str[i] != '\'' && str[i] != '\"')
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

char	**ft_isolate_path(t_ms *ms)
{
	char	*path;
	char	**tab_path;
	int		i;

	i = 0;
	path = NULL;
	while (ms->envp[i])
	{
		if (ms->envp[i][0] == 'P' && ms->envp[i][1] == 'A'
			&& ms->envp[i][2] == 'T' && ms->envp[i][3] == 'H' 
			&& ms->envp[i][4] == '=')
			path = ft_strdup(ms->envp[i]);
		i++;
	}
	if (path != NULL)
	{
		tab_path = ft_split(path, ':');
		ft_clean_path(tab_path);
		free(path);
		return (tab_path);
	}
	return (NULL);
}

bool	is_valid(char *wildcard, char *name)
{
	if (*wildcard == '\0' && *name == '\0')
		return (true);
	if (*wildcard == '*')
		return (is_valid(wildcard + 1, name) || (*name && is_valid(wildcard, name + 1)));
	if (*wildcard == '?' || *wildcard == *name)
		return (is_valid(wildcard + 1, name + 1));
	return (false);
}

void	star_expander(char *wildcard, char ***tab, int *index)
{
	DIR				*dir;
	struct dirent	*dirent;
	int				i;

	dir = opendir(".");
	dirent = readdir(dir);
	if (is_valid(wildcard, dirent->d_name) && dirent->d_name[0] != '.')
		spl_replace((*tab), dirent->d_name, *index);
	while (dirent != NULL)
	{
		dirent = readdir(dir);
		if (dirent == NULL)
			break ;
		if (is_valid(wildcard, dirent->d_name) && dirent->d_name[0] != '.')
		{
			if (strcmp((*tab)[(*index)], wildcard) == 0)
				spl_replace((*tab), dirent->d_name, *index);
			else
			{
				spl_append(tab, dirent->d_name);
				(*index)++;
			}
		}
	}
	closedir(dir);
}

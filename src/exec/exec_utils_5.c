/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 18:35:28 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/11 18:37:00 by lscheupl         ###   ########.fr       */
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
			&& ms->envp[i][2] == 'T' && ms->envp[i][3] == 'H')
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

char	*star_expander(char *input, int *index)
{
	DIR				*dir;
	struct dirent	*dirent;
	char			*tmp2;

	input[*index] = '\0';
	tmp2 = NULL;
	dir = opendir(".");
	dirent = readdir(dir);
	while (dirent != NULL)
	{
		tmp2 = ft_strjoin(input, strcat(dirent->d_name, " "));
		free(input);
		input = ft_strdup(tmp2);
		free(tmp2);
		dirent = readdir(dir);
	}
	closedir(dir);
	input[ft_strlen(input) - 1] = '\0';
	return (input);
}

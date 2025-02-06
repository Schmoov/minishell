/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:02:12 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/06 16:35:27 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_clean_path(char **tab_path)
{
	int	i;
	int	j;

	i = 0;
	j = ft_strlen(tab_path[0]) - 5;
	while (i < j)
	{
		tab_path[0][i] = tab_path[0][i + 5];
		i++;
	}
}

char	*ft_convert_pos_to_string(char *input, int start, int end)
{
	char	*res;
	int		i;
	int		j;

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
char *ft_strndup(const char *s, size_t n)
{
	char	*dup;

	dup = (char *)malloc(n + 1 * sizeof(char));
	if (!dup)
		return (NULL);
	// ft_strlcpy(dup, s, n + 1);
	dup = (char *)ft_memcpy(dup, s, n);
	dup[n] = '\0';
	return (dup);
}


size_t where_is_dollar(char *input, size_t i)
{
	while (input[i])
	{
		if (input[i] == '$')
			return (i);
		i++;
	}
	return (i);
}

size_t where_dollar_end(char *input, size_t i)
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

void	ft_clean_dollar(char *res)
{
	int j;

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
	int j;

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

char	*ft_check_access(char **tab_path, char *cmd)
{
	char	test[255];
	char	*res;
	int		i;

	i = 0;
	res = NULL;
	if (access(cmd, X_OK) == 0)
		return (res = ft_strdup(cmd));
	while (tab_path[i])
	{
		ft_strlcpy(test, tab_path[i], 256);
		ft_strlcat(test, "/", 256);
		ft_strlcat(test, cmd, 256);
		if (access(test, X_OK) == 0)
			break ;
		ft_bzero(test, 256);
		i++;
	}
	if (access(test, X_OK) == 0)
	{
		res = malloc(sizeof(char) * (ft_strlen(test) + 1));
		ft_strlcpy(res, test, (ft_strlen(test) + 1));
	}
	return (res);
}

char	*ft_isolate_first_word(char *expanded)
{
	int		i;
	char	*cmd;

	i = 0;
	while (expanded[i] != ' ' && expanded[i] != '\0')
	{
		i++;
	}
	cmd = malloc(sizeof(char) * i + 2);
	i = 0;
	while (expanded[i] != ' ' && expanded[i] != '\0')
	{
		cmd[i] = expanded[i];
		i++;
	}
	cmd[i] = '\0';
	return (cmd);
}

char	*ft_find_path(char *expanded, t_ms *ms, char **args)
{
	char		**tab_path;
	char		*bin;

	bin = NULL;
	tab_path = ft_isolate_path(ms);
	bin = ft_check_access(tab_path, args[0]);
	ft_free_split(tab_path);
	return (bin);
}

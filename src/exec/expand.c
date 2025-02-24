/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:06:16 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/24 17:15:18 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <string.h>

void	if_redir_expand(char *input, char ***res, char *tmp, int *i)
{
	if (input[(*i)] == '>' | input[(*i)] == '<')
	{
		if (input[(*i) + 1] == input[(*i)])
		{
			spl_append(res, tmp = pos_to_string(input, (*i), (*i) + 2));
			(*i)++;
		}
		else
			spl_append(res, tmp = pos_to_string(input, (*i), (*i) + 1));
		free(tmp);
	}
}

void	while_words_array(int i, char ***res, char *input)
{
	int		j;
	char	*tmp;

	while (input[i])
	{
		while (ft_strchr("|&;() \n\t", input[i]) != NULL)
			i++;
		j = i;
		while (ft_strchr("|&;()<> \n\t", input[i]) == NULL && input[i])
		{
			if (input[i] == '\'' || input[i] == '\"')
				i = skip_to(input, i, input[i]);
			i++;
		}
		spl_append(res, tmp = pos_to_string(input, j, i));
		free(tmp);
		if (input[i] == '\0')
			break ;
		if_redir_expand(input, res, tmp, &i);
		if (input[i] != '\0')
			i++;
	}
}

char	**make_words_array(char *input)
{
	char	**res;
	int		i;

	i = 0;
	if (is_empty(input))
	{
		res = ft_calloc(2, sizeof(char *));
		res[0] = ft_strdup("");
		res[1] = NULL;
		return (res);
	}
	res = ft_calloc(1, sizeof(char *));
	res[0] = NULL;
	if (input == NULL || input[0] == '\0' || is_empty(input))
		return (res);
	while_words_array(i, &res, input);
	return (res);
}

int	expanding_while(char **res, t_ms *ms, char ***tmp, char *input)
{
	int		i;
	int		j;
	char	*tmp2;

	i = 0;
	j = 0;
	while (res[i])
	{
		if (valid_redir((*tmp)[j]))
		{
			if (valid_star(res[i + 1]))
			{
				tmp2 = count_star_out(res[i + 1]);
				if (tmp2 == NULL)
					return (free(input), ft_free_split(res), free(tmp2), 1);
			}
		}
		expanding_execution(tmp, &j, ms);
		j++;
		i++;
		if (!res[i])
			break ;
		spl_append(tmp, res[i]);
	}
	return (EXIT_SUCCESS);
}

char	**to_expansion(char *input, t_ms *ms)
{
	char	**res;
	char	**tmp;
	int		i;

	res = make_words_array(input);
	tmp = ft_calloc(1, sizeof(char *) * 2);
	i = 0;
	spl_replace(tmp, res[i], 0);
	if (is_empty(res[0]))
		return (free(input), res);
	if (expanding_while(res, ms, &tmp, input) == EXIT_FAILURE)
		return (tmp);
	while (tmp[i])
	{
		if (tmp[i][0] == '\0')
			spl_remove(tmp, i);
		else
			i++;
	}
	i = 0;
	free(input);
	ft_free_split(res);
	return (tmp);
}

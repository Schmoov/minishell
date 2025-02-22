/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:06:16 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/22 16:47:06 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <string.h>

char	*conversion_dollar(char *input, t_ms *ms)
{
	int	i;

	i = 0;
	if (ft_strncmp(input, "$?", 2) == 0)
		return (ft_strdup(ft_itoa(ms->status)));
	if (ft_strlen(input) == 1)
		return (ft_strdup(input));
	ft_clean_dollar(input);
	while (ms->envp[i])
	{
		if (ft_strncmp(ms->envp[i], input, ft_strlen(input)) == 0)
		{
			if (ms->envp[i][ft_strlen(input)] == '=')
			{
				return (pos_to_string(ms->envp[i], ft_strlen(input) + 1,
						ft_strlen(ms->envp[i])));
			}
		}
		i++;
	}
	return (ft_strdup(""));
}

char	*dollar_expander(char *input, t_ms *ms, int *index)
{
	int		i;
	char	*res;
	char	*tmp;
	char	*tmp2;

	res = NULL;
	tmp = NULL;
	tmp2 = NULL;
	dprintf(2, "input: %s\n", input);
	i = where_is_dollar(input, *index);
	if (i != 0)
		res = ft_strndup(input, i);
	else
		res = ft_strdup("");
	tmp = pos_to_string(input, i, where_dollar_end(input, i));
	tmp2 = conversion_dollar(tmp, ms);
	free(tmp);
	*index += ft_strlen(tmp2);
	tmp = ft_strjoin(res, tmp2);
	free(res);
	free(tmp2);
	res = ft_strjoin(tmp, input + where_dollar_end(input, i));
	free(tmp);
	free(input);
	dprintf(2, "res: %s\n", res);
	*index = *index - 1;
	return (res);
}

void	ft_expander(t_ms *ms, char ***tab, int *tab_index, char *to_be_expanded)
{
	int	i;
	int	d_quote;
	int tmp;

	tmp = *tab_index;
	i = 0;
	d_quote = 0;
	while (to_be_expanded[i])
	{
		if (to_be_expanded[i] == '\"')
			d_quote++;
		else if (to_be_expanded[i] == '\'' && d_quote % 2 == 0)
			i = skip_single_quote(to_be_expanded, i);
		else if (to_be_expanded[i] == '$')
		{
			if (ft_isdigit(to_be_expanded[i + 1]))
				ft_clean_digit_dollar(to_be_expanded, i);
			else if (is_in_quote(to_be_expanded, i) == 1)
				i++;
			else
				to_be_expanded = dollar_expander(to_be_expanded, ms, &i);
		}
	}
	spl_replace(*tab, to_be_expanded, tmp);
}

void	star_handler(t_ms *ms, char ***tab, int *tab_index, char *to_be_expanded)
{
	int	i;
	int	d_quote;
	char	*tmp2;

	i = 0;
	d_quote = 0;
	while (to_be_expanded[i])
	{
		if (to_be_expanded[i] == '\"')
			i = skip_to(to_be_expanded, i, '\"');
		else if (to_be_expanded[i] == '\'' && d_quote % 2 == 0)
			i = skip_single_quote(to_be_expanded, i);
		else if (to_be_expanded[i] == '*' && d_quote % 2 == 0)
		{
			tmp2 = ft_strdup(to_be_expanded);
			return (star_expander(tmp2, tab, tab_index), free(tmp2));
		}
		i++;
	}
}

char	**make_words_array(char *input)
{
	char	**res;
	char	*tmp;
	int		i;
	int		j;

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
		spl_append(&res, tmp = pos_to_string(input, j, i));
		free(tmp);
		if (input[i] == '\0')
			break;
		if (input[i] == '>' | input[i] == '<')
		{
			if (input[i + 1] == input[i])
			{
				spl_append(&res, tmp = pos_to_string(input, i, i + 2));
				i++;
			}
			else
				spl_append(&res, tmp = pos_to_string(input, i, i + 1));
			free(tmp);
		}
		if (input[i] != '\0')

			i++;
	}
	return (res);
}

char	**to_expansion(char *input, t_ms *ms)
{
	char	**res;
	char 	**tmp;
	int		i;
	int		j;

	res = make_words_array(input);
	tmp = ft_calloc(1, sizeof(char *) * 2);
	tmp[0] = NULL;
	i = 0;
	j = 0;
	spl_replace(tmp, res[i], 0);
	if (is_empty(res[0]))
		return(free(input), res);
	while (res[i])
	{
		dprintf(2, "res[%d]: %s\n", i, res[i]);
		if (ft_strchr(tmp[j], '$'))
			ft_expander(ms, &tmp, &j, tmp[j]);
		if (ft_strchr(tmp[j], '*'))
			star_handler(ms, &tmp, &j, tmp[j]);
		j++;
		i++;
		if (!res[i])
			break;
		spl_append(&tmp, res[i]);
	}
	i = 0;
	while (tmp[i])
	{
		if (tmp[i][0] == '\0')
			spl_remove(tmp, i);
		else
			i++;
	}
	i = 0;
	while (tmp[i])
		single_layer_quotes_remover(tmp[i++]);
	free(input);
	ft_free_split(res);
	return (tmp);
}

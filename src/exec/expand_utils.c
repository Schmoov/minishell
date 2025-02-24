/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 13:47:00 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/24 20:06:14 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	tmp2 = ft_strdup(input + where_dollar_end(input, i));
	res = ft_strjoin(tmp, tmp2);
	*index = *index - 1;
	return (free(tmp), free(tmp2), free(input), res);
}

void	ft_expander(t_ms *ms, char ***tab, int *tab_index, char *to_be_expanded)
{
	int		i;
	int		d_quote;
	char	*tmp;

	i = 0;
	d_quote = 0;
	tmp = ft_strdup(to_be_expanded);
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
		i++;
	}
	end_expander((*tab), (*tab_index), to_be_expanded, tmp);
}

void	star_handler(char ***tab, int *tab_index, char *to_be_expanded)
{
	int		i;
	int		d_quote;
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

bool	valid_redir(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			i = skip_to(str, i, str[i]);
		if (str[i] == '>' || str[i] == '<')
			return (true);
		i++;
	}
	return (false);
}

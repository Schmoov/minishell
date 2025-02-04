/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:06:16 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/04 14:22:46 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <string.h>

char	*conversion_dollar(char *input, t_ms *ms)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
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
				return (ft_convert_pos_to_string(ms->envp[i], ft_strlen(input)
						+ 1, ft_strlen(ms->envp[i])));
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
	tmp = ft_convert_pos_to_string(input, i, where_dollar_end(input, i));
	tmp2 = conversion_dollar(tmp, ms);
	free(tmp);
	*index += ft_strlen(tmp2);
	tmp = ft_strjoin(res, tmp2);
	free(res);
	free(tmp2);
	res = ft_strjoin(tmp, input + where_dollar_end(input, i));
	free(tmp);
	free(input);
	return (res);
}

char	*star_expander(char *input, t_ms *ms, int *index)
{
	DIR				*dir;
	struct dirent	*dirent;
	char			*tmp2;

	input[*index] = '\0';
	tmp2 = NULL;
	dir = opendir(".");
	while ((dirent = readdir(dir)) != NULL)
	{
		tmp2 = ft_strjoin(input, strcat(dirent->d_name, " "));
		free(input);
		input = ft_strdup(tmp2);
		free(tmp2);
	}
	closedir(dir);
	input[ft_strlen(input) - 1] = '\0';
	return (input);
}

char	*ft_expander(char *to_be_expanded, t_ms *ms)
{
	int		i;
	int		double_quote;

	i = 0;
	double_quote = 0;
	while (to_be_expanded[i])
	{
		if (to_be_expanded[i] == '\"')
			double_quote++;
		else if (to_be_expanded[i] == '\'' && double_quote % 2 == 0)
			i = skip_single_quote(to_be_expanded, i);
		else if (to_be_expanded[i] == '$')
		{
			if (ft_isdigit(to_be_expanded[i + 1]))
				ft_clean_digit_dollar(to_be_expanded, i);
			else if (is_in_quote(to_be_expanded, i) == 1)
				i++;
			else
			{
				to_be_expanded = dollar_expander(to_be_expanded, ms, &i);
				i--;
			}
		}
		else if (to_be_expanded[i] == '*' && double_quote % 2 == 0 && (to_be_expanded[i + 1] == ' '
				|| to_be_expanded[i + 1] == '\0') && (i == 0)) // metacharacter
			to_be_expanded = star_expander(to_be_expanded, ms, &i);
		i++;
	}
	return (to_be_expanded);
}

char **make_words_array(char *input, t_ms *ms)
{
	char	**res;
	char	*tmp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	res = ft_calloc(1, sizeof(char *));
	res[0] = NULL;
	while (input[i])
	{
		while (ft_strchr("|&;() \n\t", input[i]) != NULL)
			i++;
		j = i;
		while (ft_strchr("|&;() \n\t", input[i]) == NULL && input[i])
			i++;
		spl_append(&res, tmp = ft_convert_pos_to_string(input, j, i));
		free(tmp);
		if (input[i] != '\0')
			i++;
	}
	return (res);
}

void single_layer_quotes_remover(char *str)
{
	int i = 0;
	int j = 0;
	int single_quote = 0;
	int double_quote = 0;

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

char	**expand_expand(char *input, t_ms *ms)
{
	char **res;

	res = make_words_array(input, ms);
	int i = 0;
	while (res[i])
	{
		if (ft_strchr(res[i], '$') || ft_strchr(res[i], '*'))
			res[i] = ft_expander(res[i], ms);
		i++;
	}
	i = 0;
	while (res[i])
	{
		if (res[i][0] == '\0')
			spl_remove(res, i);
		else
			i++;
	}
	// i = 0;
	// while (res[i])
	// {
	// 	printf("res[%d]: %s\n", i, res[i]);
	// 	i++;
	// }
	i = 0;
	while (res[i])
	{
		single_layer_quotes_remover(res[i]);
		i++;
	}
	free(input);
	return (res);
}



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:06:16 by lscheupl          #+#    #+#             */
/*   Updated: 2025/01/31 19:15:30 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	res = ft_strndup(input, i);
	tmp = ft_convert_pos_to_string(input, i, where_dollar_end(input, i));
	tmp2 = conversion_dollar(tmp, ms);
	*index += ft_strlen(tmp2);
	tmp = ft_strjoin(res, tmp2);
	free(res);
	free(tmp2);
	res = ft_strjoin(tmp, input + where_dollar_end(input, i));
	free(tmp);
	return (res);
}

char	*star_expander(char *input, t_ms *ms, int *index)
{
	DIR				*dir;
	struct dirent	*dirent;
	char			*tmp;
	char			*tmp2;

	tmp = ft_convert_pos_to_string(input, *index + 1, ft_strlen(input));
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
	tmp2[ft_strlen(tmp2) - 1] = '\0';
	tmp2 = ft_strjoin(input, tmp);
	free(input);
	free(tmp);
	return (tmp2);
}

char	*ft_expander(char *to_be_expanded, t_ms *ms)
{
	char *res;
	int		i;
	int		double_quote;

	i = 0;
	double_quote = 0;
	while (to_be_expanded[i])
	{
		if (to_be_expanded[i] == '\"')
			double_quote++;
		if (to_be_expanded[i] == '\'' && double_quote % 2 == 0)
			i = skip_single_quote(res, i);
		if (to_be_expanded[i] == '$')
		{
			if (isdigit(to_be_expanded[i + 1]))
				ft_clean_digit_dollar(res, i);
			else if (is_in_quote(res, i) == 1)
				i++;
			else
			{
				res = dollar_expander(to_be_expanded, ms, &i);
				i--;
			}
		}
		if (to_be_expanded[i] == '*' && double_quote % 2 == 0 && (to_be_expanded[i + 1] == ' '
				|| to_be_expanded[i + 1] == '\0') && (to_be_expanded[i - 1] == ' ')) // metacharacter
			res = star_expander(to_be_expanded, ms, &i);
		i++;
	}
	return (res);
}

// int double_quote_handler(char *input, int i, char **res, t_ms *ms)
// {
// 	int	k;
// 	char *tmp;

// 	k = i + 1;
// 	while (input[k] == '"')
// 		k++;
// 	while (input[k] != '"') // metacharacter
// 		k++;
// 	input[k] = '\0';
// 	tmp = ft_convert_pos_to_string(input, i + 1, k);
// 	if (ft_strchr(tmp, '$') || ft_strchr(tmp, '*'))
// 		spl_append(&res, ft_expander(input, i + 1, k, ms));
// 	else
// 		spl_append(&res, tmp);
// 	free(tmp);
// 	return (k);
// }

// int single_quote_handler(char *input, int i, char **res, t_ms *ms)
// {
// 	int	k;
// 	char *tmp;
// 	int offset;

// 	k = i + 1;
// 	while (ft_strchr("|&;()<> \n\t", input[k]) != NULL)
// 	{
// 	while (input[k] == '\'')
// 		k++;
// 	while (input[k] != '\'')
// 		k++;
// 	if (ft_strchr("|&;()<> \n\t", input[k]) != NULL)
// 		input[k] = '\0';
// 	else
// 	{
// 		offset = k;
// 		while (input[k + 1] != '\0')
// 		{
// 			input[k] = input[k + 1];
// 			k++;
// 		}
// 	}

// 	}
// 	tmp = ft_convert_pos_to_string(input, i + 1, k);
// 	if (ft_strchr(tmp, '$') || ft_strchr(tmp, '*'))
// 		spl_append(&res, ft_expander(input, i + 1, k, ms));
// 	else
// 		spl_append(&res, tmp);
// 	free(tmp);
// 	return (k);
// }

// int unquoted_handler(char *input, int i, char **res, t_ms *ms)
// {
// 	int	k;
// 	char *tmp;

// 	k = i + 1;
// 	while (ft_strchr("|&;()<> \n\t", input[k]) != NULL)
// 		k++;
// 	while (ft_strchr("|&;()<> \n\t", input[k]) == NULL && input[k])
// 		k++;
// 	input[k] = '\0';
// 	tmp = ft_convert_pos_to_string(input, i + 1, k);
// 	if (ft_strchr(tmp, '$') || ft_strchr(tmp, '*'))
// 		spl_append(&res, ft_expander(input, i + 1, k, ms));
// 	else
// 		spl_append(&res, tmp);
// 	free(tmp);
// 	return (k);
// }

char **make_words_array(char *input, t_ms *ms)
{
	char	**res;
	int		i;
	int		j;

	i = 0;
	j = 0;
	res = ft_calloc(1, sizeof(char *));
	res[0] = NULL;
	while (input[i])
	{
		while (ft_strchr("|&;()<> \n\t", input[i]) != NULL)
			i++;
		j = i;
		while (ft_strchr("|&;()<> \n\t", input[i]) == NULL || input[i])
			i++;
		spl_append(&res, ft_convert_pos_to_string(input, j, i));
		if (input[i] != '\0')
			i++;
	}
	return (res);
}
// if (ft_strchr(res[i], '"') != NULL)
// 			i = double_quote_handler(res[i], ft_strchr(res[i], '"') - res[i], res, ms);
// 		else if (ft_strchr(res[i], '\'') != NULL)
// 			i = single_quote_handler(res[i], ft_strchr(res[i], '\'') - res[i], res, ms);
// 		else
// 			i = unquoted_handler(res[i], 0, res, ms);
// 		i++;

char	**expand_expand(char *input, t_ms *ms)
{
	char **res;

	res = make_words_array(input, ms);
	int i = 0;
	while (res[i])
	{
		i++;
	}
	return (res);
}



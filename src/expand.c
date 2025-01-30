/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:06:16 by lscheupl          #+#    #+#             */
/*   Updated: 2025/01/30 17:10:15 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char *conversion_dollar(char *input, t_ms *ms)
{
	int		i;
	int		j;
	char	*res;

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
				res = ft_convert_pos_to_string(ms->envp[i], ft_strlen(input) + 1, ft_strlen(ms->envp[i]));
				return (res);
			}
		}
		i++;
	}
	res = ft_strdup("");
	return (res);
}

char *dollar_expander(char *input, t_ms *ms, int *index)
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


char *star_expander(char *input, t_ms *ms, int *index)
{
    DIR *dir;
    struct dirent *dirent;
	char *tmp;
	char *tmp2;

	tmp = ft_convert_pos_to_string(input, *index + 1, ft_strlen(input));
	input[*index] = '\0';
	tmp2 = NULL;
    dir = opendir(".");
    while((dirent = readdir(dir)) != NULL)
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

char	*ft_expander(char *input, int start, int end, t_ms *ms)
{
	char	*res;
	int i;
	int double_quote;

	i = 0;
	double_quote = 0;
	res = ft_convert_pos_to_string(input, start, end);
	while (res[i])
	{
		if (res[i] == '\"')
			double_quote++;
		if (res[i] == '\'' && double_quote%2 == 0)
			i = skip_single_quote(res, i);
		if (res[i] == '$')
		{
			if (isdigit(res[i + 1]))
				ft_clean_digit_dollar(res, i);
			else if (is_in_quote(res, i) == 1)
				i++;
			else
			{
			res = dollar_expander(res, ms, &i);
			i--;
			}
		}
		if (res[i] == '*' && double_quote%2 == 0 && (res[i + 1] == ' ' || res[i + 1] == '\0') && (res[i - 1] == ' '))
			res = star_expander(res, ms, &i);
		i++;
	}
	return (res);
}

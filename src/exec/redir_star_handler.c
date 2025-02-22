/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_star_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 17:08:03 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/22 17:14:34 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*redir_star_expander(char *wildcard)
{
	DIR				*dir;
	struct dirent	*dirent;
	char            *str;
    int             i;

	dir = opendir(".");
    i = 0;
	dirent = readdir(dir);
	if (is_valid(wildcard, dirent->d_name) && dirent->d_name[0] != '.')
    {
		str = dirent->d_name;
        i++;
    }
	while (dirent != NULL)
	{
		dirent = readdir(dir);
		if (dirent == NULL)
			break ;
		if (is_valid(wildcard, dirent->d_name) && dirent->d_name[0] != '.')
		{
			str = dirent->d_name;
            i++;
		}
	}
    closedir(dir);
    if (i == 1)
        return (str);
    else
        return (NULL);
}

char	*count_star_out(char *to_be_expanded)
{
	int	i;
	int	d_quote;

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
			return (redir_star_expander(to_be_expanded));
		}
		i++;
	}
}
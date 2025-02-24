/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 14:51:31 by lscheupl          #+#    #+#             */
/*   Updated: 2025/02/24 14:51:40 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*redir_expander(char *to_be_expanded, t_ms *ms)
{
	int	i;
	int	d_quote;

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
		i++;
	}
	return (to_be_expanded);
}

int	redir(char *word, int fd[2], t_ms *ms, int type)
{
	word = redir_expander(word, ms);
	if (ft_strchr(word, '*'))
		word = count_star_out(word);
	if (word == NULL)
		return (write(2, "ambiguous redirect:", 20), EXIT_FAILURE);
	single_layer_quotes_remover(word);
	if (type == 0)
		fd[0] = redir_in(word);
	else if (type == 1)
		fd[1] = redir_out(word);
	else if (type == 2)
		fd[1] = redir_app(word);
	free(word);
	if (fd[0] == -1 && fd[1] == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

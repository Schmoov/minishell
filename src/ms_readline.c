/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:56:31 by parden            #+#    #+#             */
/*   Updated: 2024/11/27 18:37:36 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ms_readline_error(t_ms *ms, int idx)
{
	int	i;

	i = 0;
	while (i++ < idx + PROMPT_LEN)
		write(2, "~", 1);
	write(2, "^\nParse error : ", 16);
	if (ms->input[idx] == '\'' || ms->input[idx] == '"')
		write(2, "Quote was never closed !\n\n", 26);
	else if (ms->input[idx] == '(')
		write(2, "Parenthesis was never closed !\n\n", 32);
	else if (ms->input[idx] == ')')
		write(2, "Unexpected closed parenthesis !\n\n", 33);
	free(ms->input);
	ms->input = NULL;
}

void	ms_readline_check_quote(t_ms *ms)
{
	int	i;
	int	quote;

	quote = -1;
	i = 0;
	while (ms->input[i])
	{
		if (ms->input[i] == '"' || ms->input[i] == '\'')
		{
			if (quote == -1)
				quote = i;
			else if (ms->input[i] == ms->input[quote])
				quote = -1;
		}
		i++;
	}
	if (quote != -1)
		ms_readline_error(ms, quote);
}

//"nest += 2 * (ms->input[i] == '(') - 1"
//Smarty pants "++ if '(', -- if ')'
void	ms_readline_check_paren(t_ms *ms)
{
	int		i;
	int		paren;
	int		nest;

	nest = 0;
	paren = -1;
	i = 0;
	while (ms->input[i])
	{
		if (ms->input[i] == '"' || ms->input[i] == '\'')
			i = close_quote(ms->input, i, ft_strlen(ms->input));
		else if (ms->input[i] == '(' || ms->input[i] == ')')
		{
			if (nest == 0 && ms->input[i] == '(')
				paren = i;
			else if (!nest && ms->input[i] == ')')
				return ((void)ms_readline_error(ms, i));
			nest += 2 * (ms->input[i] == '(') - 1;
		}
		i++;
	}
	if (nest)
		ms_readline_error(ms, paren);
}

void	ms_readline(t_ms *ms)
{
	ms->input = readline("kksh>");
	add_history(ms->input);
	ms_readline_check_quote(ms);
	if (ms->input)
		ms_readline_check_paren(ms);
}

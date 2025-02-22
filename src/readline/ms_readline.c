/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:56:31 by parden            #+#    #+#             */
/*   Updated: 2025/02/22 17:15:41 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <readline/readline.h>

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

void	heredoc_process(t_ms *ms, int *pos)
{
	int		l;
	int		r;
	int		idx;
	char	*tmp;

	r = *pos + 2;
	heredoc_process_helper(ms, &l, &r);
	tmp = ft_substr(ms->input, l, r - l);
	idx = heredoc_parse(tmp);
	free(tmp);
	tmp = calloc(ft_strlen(ms->input) + 16, 1);
	if (!tmp)
		return ((void)(free(ms->input), ms->input = NULL));
	strncat(tmp, ms->input, *pos + 1);
	strcat(tmp, "/tmp/mshd_");
	tmp[ft_strlen(tmp)] = idx / 10 + '0';
	tmp[ft_strlen(tmp)] = idx % 10 + '0';
	strcat(tmp, ms->input + r);
	*pos += 13;
	free(ms->input);
	ms->input = tmp;
}

void	ms_readline(t_ms *ms)
{
	int	i;

	set_sighandler(true);
	ms->input = readline("Sch_sh>");
	set_sighandler(false);
	if (!ms->input)
		blt_exit(ms);
	add_history(ms->input);
	ms_readline_check_quote(ms);
	if (ms->input)
		ms_readline_check_paren(ms);
	i = 0;
	while (ms->input && ms->input[i])
	{
		if (!ft_strncmp(ms->input + i, "<<", 2))
			heredoc_process(ms, &i);
		else if (ms->input[i] == '\'' || ms->input[i] == '"')
			i = close_quote(ms->input, i, ft_strlen(ms->input));
		else
			i++;
	}
}

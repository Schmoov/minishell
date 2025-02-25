/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_parse.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:09:08 by parden            #+#    #+#             */
/*   Updated: 2025/02/22 17:17:50 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_PARSE_H
# define MS_PARSE_H

# include "ast.h"

typedef struct s_minishell	t_ms;

int			close_quote(char *input, int start, int end);
int			rclose_quote(char *input, int start, int end);
int			close_paren(char *input, int start, int end);
int			rclose_paren(char *input, int start, int end);
int			find_first_paren(char *input, int start, int end);
int			find_last_paren(char *input, int start, int end);
int			find_last_logic(char *input, int start, int end);
int			count_pip(char *input, int start, int end);
void		find_pip(char *input, int start, int end, int *pos);

void		set_sighandler(bool rl);
void		ms_readline(t_ms *ms);
void		heredoc_process(t_ms *ms, int *pos);
void		heredoc_process_helper(t_ms *ms, int *l, int *r);
int			heredoc_parse(char *delim);

void		ms_parse(t_ms *ms);
t_ast		*parse_rec(char *input, int start, int end);
t_node_type	parse_node_type(char *input, int start, int end);
t_ast		*parse_cmd(char *input, int start, int end);
t_ast		*parse_grp(char *input, int start, int end);
t_ast		*parse_logic(char *input, int start, int end);
t_ast		*parse_pip(char *input, int start, int end);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: linux <linux@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:02:04 by linux             #+#    #+#             */
/*   Updated: 2025/01/31 18:12:52 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "ms_utils.h"
# include "ms_parse.h"
# include "bltin.h"
# include "ms_signal.h"

# define PROMPT_LEN 7

typedef struct s_minishell {
	int		envp_len;
	char	**envp;

	char	*input;
	t_ast	*ast;
}			t_ms;

void	ms_create(t_ms *ms, char **envp);
void	ms_loop(t_ms *ms);
void	ms_exec(t_ms *ms);
void	ms_destroy(t_ms *ms);

void	ms_readline_error(t_ms *ms, int idx);
void	ms_readline_check_quote(t_ms *ms);
void	ms_readline_check_paren(t_ms *ms);
void	ms_readline(t_ms *ms);

#endif

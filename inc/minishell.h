/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:02:04 by linux             #+#    #+#             */
/*   Updated: 2025/01/15 18:36:54 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "ms_parse.h"
# include "sys/wait.h"

# define PROMPT_LEN 5

typedef struct s_minishell {
	int		envp_len;
	char	**envp;

	char	*input;
	t_ast	*ast;
	int		status;
}			t_ms;

void	ms_create(t_ms *ms, char **envp);
void	ms_loop(t_ms *ms);
void	ms_exec(t_ms *ms);
void	ms_destroy(t_ms *ms);

// test ///
void	exec_cmd(char *input, t_ast *root, t_ms *ms);
void    exec_general(char *input, t_ast *root, t_ms *ms);
char *strndup(const char *s, size_t n);

#endif

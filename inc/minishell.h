/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:02:04 by linux             #+#    #+#             */
/*   Updated: 2025/02/03 16:21:23 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "ms_utils.h"
# include "ms_parse.h"
# include "sys/wait.h"
#include <dirent.h>
#include <sys/types.h>
# include "bltin.h"

# define PROMPT_LEN 7

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
int	exec_cmd(char *input, t_ast *root, t_ms *ms);
int    exec_general(char *input, t_ast *root, t_ms *ms);
char *ft_strndup(const char *s, size_t n);
void    ft_exec_builtin(t_node_cmd *node,t_ms *ms);
char	*ft_find_path(char *expanded, t_ms *ms, char **args);
char	*ft_expander(char *to_be_expanded, t_ms *ms);
char	*ft_isolate_first_word(char *expanded);
char	*ft_check_access(char **tab_path, char *cmd);
char	**ft_isolate_path(t_ms *ms);
int	is_in_quote(char *res, int i);
int	skip_single_quote(char *res, int i);
void	ft_clean_dollar(char *res);
size_t where_dollar_end(char *input, size_t i);
size_t where_is_dollar(char *input, size_t i);
char	*ft_convert_pos_to_string(char *input, int start, int end);
void	ft_clean_path(char **tab_path);
void	ft_clean_digit_dollar(char *res, int i);



void	ms_readline_error(t_ms *ms, int idx);
void	ms_readline_check_quote(t_ms *ms);
void	ms_readline_check_paren(t_ms *ms);
void	ms_readline(t_ms *ms);

#endif

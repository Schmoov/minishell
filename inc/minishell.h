/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 11:02:04 by linux             #+#    #+#             */
/*   Updated: 2025/02/24 18:51:29 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/libft.h"
# include "bltin.h"
# include "ms_parse.h"
# include "ms_utils.h"
# include "sys/wait.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/types.h>

# define PROMPT_LEN 7

typedef struct s_minishell
{
	int		envp_len;
	char	**envp;

	char	*input;
	t_ast	*ast;
	int		status;
	int		fd[2];
}			t_ms;

void		ms_create(t_ms *ms, char **envp);
void		ms_loop(t_ms *ms);
void		ms_exec(t_ms *ms);
void		ms_destroy(t_ms *ms);

// test ///
int			exec_cmd(char *input, t_ast *root, t_ms *ms);
int			exec_general(char *input, t_ast *root, t_ms *ms);
char		*ft_strndup(const char *s, size_t n);
void		ft_exec_builtin(t_node_cmd *node, t_ms *ms);
void		ft_expander(t_ms *ms, char ***tab, int *index,
				char *to_be_expanded);
char		*ft_isolate_first_word(char *expanded);
char		*ft_check_access(char **tab_path, char *cmd);
char		**ft_isolate_path(t_ms *ms);
int			is_in_quote(char *res, int i);
int			skip_single_quote(char *res, int i);
void		ft_clean_dollar(char *res);
size_t		where_dollar_end(char *input, size_t i);
size_t		where_is_dollar(char *input, size_t i);
char		*pos_to_string(char *input, int start, int end);
void		ft_clean_path(char **tab_path);
void		ft_clean_digit_dollar(char *res, int i);
char		**to_expansion(char *input, t_ms *ms);
int			redir_in(char *path);
int			redir_out(char *path);
int			redir_app(char *path);
int			redir_hd(char *delim, t_ms *ms);
char		**ft_isolate_path(t_ms *ms);
void		ft_clean_dollar(char *res);
int			skip_single_quote(char *res, int i);
void		ft_clean_digit_dollar(char *res, int i);
int			is_in_quote(char *res, int i);
char		*ft_find_path(t_ms *ms, char **args);
int			redir_handler(int *redir, char **args);
int			is_builtin(char *cmd);
void		ms_close_fd(t_ms *ms);
int			exec_builtin(t_node_cmd *node, t_ms *ms);
void		ms_fd(t_ms *ms);
int			skip_to(char *input, int i, char c);
void		redir_executions(int *redir);
void		close_all(int *redir, t_ms *ms);
void		single_layer_quotes_remover(char *str);
int			redir(char *word, int fd[2], t_ms *ms, int type);
char		*get_next_word(char *input, int i, t_node_cmd *node);
int			exec_grp(char *input, t_ast *root, t_ms *ms);
void		star_expander(char *input, char ***tab, int *index);
char		*count_star_out(char *to_be_expanded);
char		*dollar_expander(char *input, t_ms *ms, int *index);
void		dup_fd(int **tmp, t_ms *ms);
int			is_empty(char *cmd);
void		close_pip_redir(int *redir);
int			init_pipe(t_node_pip *node);
void		handle_child_process(t_node_pip *node, int i, char *input,
				t_ms *ms);
int			is_redir_before(char *input, t_node_cmd *node);
bool		valid_redir(char *str);
bool		valid_star(char *str);
void		expanding_execution(char ***tmp, int *j, t_ms *ms);
void		star_handler(char ***tab, int *tab_index, char *to_be_expanded);
bool		is_valid(char *wildcard, char *name);
void		end_expander(char **tab, int tab_index, char *to_be_expanded,
				char *tmp);

void		ms_readline_error(t_ms *ms, int idx);
void		ms_readline_check_quote(t_ms *ms);
void		ms_readline_check_paren(t_ms *ms);
void		ms_readline(t_ms *ms);

#endif

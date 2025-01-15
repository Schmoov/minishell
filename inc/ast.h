/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 21:09:08 by parden            #+#    #+#             */
/*   Updated: 2024/11/27 18:39:23 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

typedef struct u_ast	t_ast;

typedef enum e_bltin {
	E_NOTBLTIN,
	E_ECHO,
	E_CD,
	E_PWD,
	E_EXPORT,
	E_UNSET,
	E_ENV,
	E_EXIT,
	NB_BLTIN
}	t_bltin;

typedef enum e_node {
	E_CMD,
	E_LOGIC,
	E_PIP,
	E_GRP,
	NB_NODE
}	t_node_type;

typedef struct s_node_cmd {
	int				start;
	int				end;

	enum e_bltin	bltin;
	char			*expanded;
	char			**args;
	int				redir[2];
}				t_node_cmd;

typedef struct s_node_logic {
	int			start;
	int			end;

	bool		is_and;
	t_ast		*left;
	t_ast		*right;
}				t_node_logic;

typedef struct s_node_pip {
	int			start;
	int			end;

	int			pip_len;
	t_ast		**piped;
	int			redir[2];
	int			(*pip_redir)[2];
}				t_node_pip;

typedef struct s_node_grp {
	int			start;
	int			end;

	int			redir[2];
	t_ast		*next;
}				t_node_grp;

//TAGGED UNION, THE POOR MAN'S POLYMORPHISM
typedef struct u_ast {
	enum e_node		type;
	union {
		t_node_cmd		cmd;
		t_node_logic	logic;
		t_node_pip		pip;
		t_node_grp	grp;
	};
}					t_ast;

t_ast		*ast_create_cmd(int start, int end);
t_ast		*ast_create_logic(int start, int end, bool is_and);
t_ast		*ast_create_pip(int start, int end, int pip_len);
t_ast		*ast_create_grp(int start, int end);
void		ast_destroy_cmd(t_ast *root);
void		ast_destroy_logic(t_ast *root);
void		ast_destroy_pip(t_ast *root);
void		ast_destroy_grp(t_ast *root);
void		ast_destroy(t_ast *root);

void		ast_print_cmd(char *input, t_ast *root, int nest);
void		ast_print_grp(char *input, t_ast *root, int nest);
void		ast_print_logic(char *input, t_ast *root, int nest);
void		ast_print_pip(char *input, t_ast *root, int nest);
void		ast_print(char *input, t_ast *root, int nest);

#endif

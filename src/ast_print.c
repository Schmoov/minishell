/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 18:19:27 by parden            #+#    #+#             */
/*   Updated: 2024/11/27 18:38:33 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ast_print_cmd(char *input, t_ast *root, int nest)
{
	int			i;
	t_node_cmd	*node;

	node = &(root->cmd);
	i = 0;
	while (i < nest)
	{
		printf("|\t");
		i++;
	}
	printf("|____CMD\t\t\"%.*s\"\n",
		node->end - node->start, input + node->start);
}

void	ast_print_grp(char *input, t_ast *root, int nest)
{
	int				i;
	t_node_grp	*node;

	i = 0;
	node = &(root->grp);
	while (i < nest)
	{
		printf("|\t");
		i++;
	}
	printf("|____grp\t\t\"%.*s\"\n",
		node->end - node->start, input + node->start);
	i = 0;
	while (i < nest + 1)
	{
		printf("|\t");
		i++;
	}
	printf("|\n");
	ast_print(input, node->next, nest + 1);
}

void	ast_print_logic(char *input, t_ast *root, int nest)
{
	int				i;
	t_node_logic	*node;

	i = 0;
	node = &(root->logic);
	while (i++ < nest)
		printf("|\t");
	if (node->is_and)
		printf("|______LOGIC &&\t\t");
	else
		printf("|______LOGIC ||\t\t");
	printf("\"%.*s\"\n", node->end - node->start, input + node->start);
	i = 0;
	while (i++ < nest + 1)
		printf("|\t");
	printf("|\n");
	ast_print(input, node->left, nest + 1);
	ast_print(input, node->right, nest + 1);
	i = 0;
	while (i++ < nest)
		printf("|\t");
	printf("|\n");
}

void	ast_print_pip(char *input, t_ast *root, int nest)
{
	int			i;
	t_node_pip	*node;

	i = 0;
	node = &(root->pip);
	while (i < nest)
	{
		printf("|\t");
		i++;
	}
	printf("|____PIP%d\t\t", node->pip_len);
	printf("\"%.*s\"\n", node->end - node->start, input + node->start);
	i = 0;
	while (i < node->pip_len)
		ast_print(input, node->piped[i++], nest + 1);
}

void	ast_print(char *input, t_ast *root, int nest)
{
	t_node_type	node_type;

	node_type = root->type;
	if (node_type == E_CMD)
		ast_print_cmd(input, root, nest);
	else if (node_type == E_LOGIC)
		ast_print_logic(input, root, nest);
	else if (node_type == E_PIP)
		ast_print_pip(input, root, nest);
	else
		ast_print_grp(input, root, nest);
}

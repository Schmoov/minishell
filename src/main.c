/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 14:38:52 by parden            #+#    #+#             */
/*   Updated: 2025/01/31 18:13:34 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//THIS IS JUST PLACEHOLDER CODE SO WE CAN TEST
//NOT REAL DESIGN
int	main(int argc, char **argv, char **envp)
{
	t_ms	ms;

	(void) argc;
	(void) argv;
	set_sighandler();
	ms_create(&ms, envp);
	ms_loop(&ms);
	ms_destroy(&ms);
}

void	ms_create(t_ms *ms, char **envp)
{
	int	i;

	ft_memset(ms, 0, sizeof(t_ms));
	i = 0;
	while (envp[i])
	{
		i++;
	}
	ms->envp_len = i;
	ms->envp = ft_calloc(ms->envp_len, sizeof(char *));
	i = 0;
	while (envp[i])
	{
		ms->envp[i] = ft_strdup(envp[i]);
		i++;
	}
}

void	ms_loop(t_ms *ms)
{
	while (1)
	{
		ms_readline(ms);
		if (!ms->input)
			continue ;
		if (!ft_strcmp(ms->input, "exit"))
			break ;
		ms_parse(ms);
		ms_exec(ms);
	}
}

void	ms_destroy(t_ms *ms)
{
	int	i;

	i = 0;
	while (i < ms->envp_len)
		free(ms->envp[i++]);
	free(ms->envp);
	free(ms->input);
	ast_destroy(ms->ast);
	rl_clear_history();
}

void	ms_exec(t_ms *ms)
{
	ast_print(ms->input, ms->ast, 0);
	ast_destroy(ms->ast);
	ms->ast = NULL;
	free(ms->input);
	ms->input = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:18:48 by parden            #+#    #+#             */
/*   Updated: 2025/02/20 17:36:38 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	sig_handler_rl(int signo, siginfo_t *info, void *context)
{
	(void) signo;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

static void	sig_handler_norl(int signo, siginfo_t *info, void *context)
{
	(void) signo;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	set_sighandler(bool rl)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	if (rl)
		sa.sa_sigaction = sig_handler_rl;
	else
		sa.sa_sigaction = sig_handler_norl;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

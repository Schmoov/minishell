/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 20:19:41 by parden            #+#    #+#             */
/*   Updated: 2025/02/06 20:20:02 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exit_alloc(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}

int	exit_exec(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}

int	exit_pipe(t_ms *ms)
{
	ms_destroy(ms);
	exit(1);
}

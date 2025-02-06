/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_utils.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lscheupl <lscheupl@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:12:27 by parden            #+#    #+#             */
/*   Updated: 2025/01/31 18:59:44 by lscheupl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MS_UTILS_H
# define MS_UTILS_H

typedef struct s_minishell t_ms;

int	exit_alloc(t_ms *ms);
int exit_fork(t_ms *ms);
int exit_pipe(t_ms *ms);
int exit_exec(t_ms *ms);
char	**spldup(char **spl);
void	spl_append(char ***spl, char *new);
int	spl_find(char **spl, char *s, int len);
void	spl_replace(char **spl, char *new, int pos);
void	spl_remove(char **spl, int pos);

#endif

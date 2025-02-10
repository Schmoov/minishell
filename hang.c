/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hang.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leonel <leonel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 23:33:25 by leonel            #+#    #+#             */
/*   Updated: 2025/02/10 23:33:26 by leonel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	while (1)
	{
		sleep(1);
		printf("%d\n", getpid());
	}
}

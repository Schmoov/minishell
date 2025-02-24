/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parden <parden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 10:55:11 by parden            #+#    #+#             */
/*   Updated: 2025/02/24 17:55:05 by parden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

char	*ft_strncat(char *dst, const char *s, size_t n)
{
	char	*a;

	a = dst;
	dst += ft_strlen(dst);
	while (n && *s)
	{
		n--;
		*dst++ = *s++;
	}
	*dst++ = 0;
	return (a);
}

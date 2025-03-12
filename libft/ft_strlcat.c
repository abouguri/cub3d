/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 01:50:13 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 05:52:39 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static char	*ft_strncpy(char *dest, const char *src, size_t n)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < n && src[i] != '\0')
// 	{
// 		dest[i] = src[i];
// 		i++;
// 	}
// 	while (i < n)
// 	{
// 		dest[i] = '\0';
// 		i++;
// 	}
// 	return (dest);
// }

size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t dstsize)
{
	size_t	dest_len;
	size_t	src_len;
	size_t	space_left;
	size_t	chars_to_copy;

	if (!dst && dstsize == 0)
		return (ft_strlen(src));
	dest_len = ft_strlen(dst);
	src_len = ft_strlen(src);
	space_left = dstsize - dest_len - 1;
	if (dstsize <= dest_len)
		return (dstsize + src_len);
	if (src_len < space_left)
		chars_to_copy = src_len;
	else
		chars_to_copy = space_left;
	if (chars_to_copy > 0)
	{
		ft_strncpy(dst + dest_len, src, chars_to_copy);
		dst[dest_len + chars_to_copy] = '\0';
	}
	return (dest_len + src_len);
}

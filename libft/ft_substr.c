/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 16:14:13 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 05:53:56 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static char	*ft_strncpy(char *dst, const char *src, size_t len)
// {
// 	size_t	i;

// 	i = 0;
// 	while (src[i] && i < len)
// 	{
// 		dst[i] = src[i];
// 		i++;
// 	}
// 	if (i < len)
// 	{
// 		while (i < len)
// 		{
// 			dst[i] = '\0';
// 			i++;
// 		}
// 	}
// 	return (dst);
// }

char	*ft_substr(char const *s, unsigned int start,
size_t len)
{
	size_t	s_len;
	size_t	sb_len;
	char	*sb;
	int		i;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (ft_strdup(""));
	if (len < (s_len - start))
		sb_len = len;
	else
		sb_len = s_len - start;
	sb = malloc(sb_len + 1);
	if (sb == NULL)
		return (NULL);
	i = 0;
	while (s[i])
		i++;
	ft_strncpy(sb, s + start, sb_len);
	sb[sb_len] = '\0';
	return (sb);
}

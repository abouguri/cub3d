/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 19:19:24 by abouguri          #+#    #+#             */
/*   Updated: 2023/11/23 03:23:07 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	needle_len;
	size_t	i;
	size_t	j;

	needle_len = 0;
	i = 0;
	j = 0;
	if (*needle == '\0' || (haystack == NULL && len == 0))
		return ((char *)haystack);
	while (needle[needle_len] != '\0')
		needle_len++;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (i + j < len && haystack[i + j] == needle[j]
			&& haystack[i + j] != '\0')
		{
			j++;
			if (j == needle_len)
				return ((char *)(haystack + i));
		}
		i++;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>
// int main(void) {
// 	printf("%s\n", ft_strnstr(NULL, "hello", 0));
// }
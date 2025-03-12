/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:31:10 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 04:17:57 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*create_line(const char *buffer, int length)
{
	char	*line;

	line = malloc(length + 1);
	if (!line)
		return (NULL);
	strncpy(line, buffer, length);
	line[length] = '\0';
	return (line);
}

char	*get_remaining_content(const char *buffer, int index)
{
	char	*remaining;

	remaining = strdup(&buffer[index + 1]);
	return (remaining);
}

int	extract_line(char **buffer, char **line)
{
	int		i;
	char	*remaining;

	i = 0;
	while ((*buffer)[i] != '\n' && (*buffer)[i] != '\0')
		i++;
	*line = create_line(*buffer, i);
	if (!*line)
		return (-1);
	if ((*buffer)[i] == '\0')
		clear_buffer(buffer);
	else
	{
		remaining = get_remaining_content(*buffer, i);
		if (!remaining)
		{
			free(*line);
			*line = NULL;
			return (-1);
		}
		clear_buffer(buffer);
		*buffer = remaining;
	}
	return (1);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:45:45 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/10 23:49:12 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_next_line(int fd, char **line)
{
	static char	*buffers[FOPEN_MAX];
	int			bytes_read;

	bytes_read = BUFFER_SIZE;
	if (validate_inputs(fd, line) == -1)
		return (-1);
	if (initialize_buffer(&buffers[fd]) == -1)
		return (-1);
	if (read_to_buffer(&buffers[fd], fd, &bytes_read) == -1)
		return (-1);
	if (contains_newline(buffers[fd]))
		return (extract_line(&buffers[fd], line));
	else
	{
		*line = strdup(buffers[fd]);
		if (!*line)
			return (-1);
		free(buffers[fd]);
		buffers[fd] = NULL;
		return (0);
	}
}

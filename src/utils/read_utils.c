/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:33:52 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/10 23:49:40 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	read_and_append_to_buffer(char **buffer, int fd, int *bytes_read)
{
	char	*temp;
	char	*combined_content;

	temp = malloc(BUFFER_SIZE + 1);
	if (!temp)
		return (-1);
	*bytes_read = read(fd, temp, BUFFER_SIZE);
	if (*bytes_read < 0)
	{
		free(temp);
		return (-1);
	}
	temp[*bytes_read] = '\0';
	combined_content = malloc(strlen(*buffer) + strlen(temp) + 1);
	if (!combined_content)
	{
		free(temp);
		return (-1);
	}
	return (append_to_buffer(buffer, combined_content, &temp));
}

int	read_to_buffer(char **buffer, int fd, int *bytes_read)
{
	while (!contains_newline(*buffer) && *bytes_read == BUFFER_SIZE)
	{
		if (read_and_append_to_buffer(buffer, fd, bytes_read) == -1)
			return (-1);
	}
	return (0);
}

int	validate_inputs(int fd, char **line)
{
	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FOPEN_MAX || !line)
		return (-1);
	return (0);
}

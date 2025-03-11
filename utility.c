/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:39:11 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:47:20 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_cub	*get_cub_data(void)
{
	static t_cub	data;

	return (&data);
}

void	error_exit(const char *message)
{
	write(2, message, strlen(message));
	exit(EXIT_FAILURE);
}

void	error_exit_fd_cleanup(const char *message, int fd)
{
	t_cub	*data;

	if (fd >= 0)
		close(fd);
	data = get_cub_data();
	free_array(&data->textures);
	free_array(&data->colors);
	free_array(&data->map);
	error_exit(message);
}

void	error_exit_cleanup(const char *message)
{
	t_cub	*data;

	data = get_cub_data();
	free_array(&data->textures);
	free_array(&data->colors);
	free_array(&data->map);
	error_exit(message);
}

void	free_array(char ***array)
{
	int	i;

	if (!*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		i++;
	}
	free(*array);
	*array = NULL;
}

void	free_resources(t_cub *data)
{
	if (data->textures)
		free_array(&data->textures);
	if (data->colors)
		free_array(&data->colors);
	if (data->map)
		free_array(&data->map);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*trim_line(const char *line)
{
	const char	*start = line;
	const char	*end;
	int			length;
	char		*trimmed;

	end = start + strlen(start) - 1;
	while (end > start && is_whitespace(*end))
		end--;
	length = end - start + 1;
	trimmed = malloc(length + 1);
	if (!trimmed)
		return (NULL);
	strncpy(trimmed, start, length);
	trimmed[length] = '\0';
	return (trimmed);
}

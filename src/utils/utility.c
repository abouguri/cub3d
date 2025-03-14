/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:39:11 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/13 21:46:15 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	error_exit(const char *message)
{
	write(2, message, ft_strlen(message));
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

	if (!array || !*array)
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

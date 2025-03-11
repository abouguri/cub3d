/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_utilsc                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:28:17 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/10 23:34:28 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	append_to_buffer(char **buffer, char *new_content, char **temp)
{
	int	i;

	i = 0;
	while ((*buffer)[i])
	{
		new_content[i] = (*buffer)[i];
		i++;
	}
	i = 0;
	while ((*temp)[i])
	{
		new_content[i + strlen(*buffer)] = (*temp)[i];
		i++;
	}
	new_content[i + strlen(*buffer)] = '\0';
	free(*temp);
	free(*buffer);
	*buffer = new_content;
	return (1);
}

void	clear_buffer(char **buffer)
{
	free(*buffer);
	*buffer = NULL;
}

int	contains_newline(char *buffer)
{
	int	i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	return (buffer[i] == '\n');
}

int	initialize_buffer(char **buffer)
{
	if (!*buffer)
	{
		*buffer = malloc(1);
		if (!*buffer)
			return (-1);
		(*buffer)[0] = '\0';
	}
	return (0);
}


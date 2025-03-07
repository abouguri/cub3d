/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 19:33:54 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/03 23:25:31 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	**reallocate_map_memory(char **pointer, int size)
{
	int		i;
	char	**new_pointer;

	i = 0;
	new_pointer = malloc(sizeof(char *) * size);
	if (!new_pointer)
		return (NULL);
	while (pointer && pointer[i])
	{
		new_pointer[i] = pointer[i];
		i++;
	}
	new_pointer[i] = NULL;
	free(pointer);
	return (new_pointer);
}

int	initialize_map(t_cub *data, char *line)
{
	data->map = malloc(sizeof(char *) * 2);
	if (!data->map)
		return (1);
	data->map[0] = strdup(line);
	data->map[1] = NULL;
	return (0);
}

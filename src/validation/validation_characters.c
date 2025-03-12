/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_characters.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:49:32 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 04:55:12 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_characters(void)
{
	t_index	z;
	int		count;
	t_cub	*data;

	data = get_cub_data();
	z.i = 0;
	count = 0;
	while (data->map[z.i])
	{
		z.j = 0;
		while (data->map[z.i][z.j])
		{
			if (!strchr(VALID_MAP_SYMBOLS, data->map[z.i][z.j]))
				return (0);
			if (data->map[z.i][z.j] == NORTH || data->map[z.i][z.j] == SOUTH
				|| data->map[z.i][z.j] == EAST || data->map[z.i][z.j] == WEST)
			{
				init_player(z.j, z.i);
				count++;
			}
			z.j++;
		}
		z.i++;
	}
	return (count);
}

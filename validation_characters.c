/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_characters.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:49:32 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:50:24 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_characters(void)
{
	int		i;
	int		j;
	int		count;
	t_cub	*data;

	data = get_cub_data();
	i = 0;
	count = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (!strchr(VALID_MAP_SYMBOLS, data->map[i][j]))
				return (0);
			if (data->map[i][j] == NORTH || data->map[i][j] == SOUTH
				|| data->map[i][j] == EAST || data->map[i][j] == WEST)
			{
				init_player(j, i);
				count++;
			}
			j++;
		}
		i++;
	}
	return (count);
}

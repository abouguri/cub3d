/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_cells.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:51:48 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:52:25 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_cells_and_players(t_cub *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->map[i])
	{
		if (validate_trailing_lines(i, data) == 1)
			return (1);
		j = 0;
		while (data->map[i][j])
		{
			if (check_cell_enclosure(i, j) == 1)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:38:13 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:39:00 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_boundary_cell(int i, int j, char **map)
{
	return (i == 0 || j == 0 || i >= ft_array_length(map) - 1
		|| j >= (int)strlen(map[i]) - 1);
}

int	print_boundary_error(int i, int j)
{
	printf(BOUNDARY_CELL_NOT_ENCLOSED, i, j);
	return (1);
}

int	has_adjacent_space(int i, int j, char **map)
{
	return ((i > 0 && j < (int)strlen(map[i - 1]) && map[i - 1][j] == ' ')
		||                        // Above
			(i + 1 < ft_array_length(map) && j < (int)strlen(map[i + 1])
				&& map[i + 1][j] == ' ') || // Below
			(j > 0 && map[i][j - 1] == ' ')
				||                                                       // Left
			(j + 1 < (int)strlen(map[i]) && map[i][j
				+ 1] == ' '));                                  // Right
}

int	print_adjacent_space_error(int i, int j)
{
	printf(CELL_NOT_ENCLOSED, i, j);
	return (1);
}

int	is_adjacent_to_incomplete_row(int i, int j, char **map)
{
	return ((i > 0 && j >= (int)strlen(map[i - 1])) ||                      
		// Current cell exceeds previous row length
			(i + 1 < ft_array_length(map) && j >= (int)strlen(map[i + 1])));
				// Current cell exceeds next row length
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:39:31 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:40:48 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	print_incomplete_row_error(int i, int j)
{
	printf(ERR_ADJ_INCOMPLETE_ROW, i, j);
	return (1);
}

int	check_cell_enclosure(int i, int j)
{
	t_cub	*data;
	char	**map;

	data = get_cub_data();
	map = data->map;
	if (map[i][j] == '0' || (map[i][j] != '1' && map[i][j] != ' '))
	{
		if (is_boundary_cell(i, j, map))
			return (print_boundary_error(i, j));
		if (has_adjacent_space(i, j, map))
			return (print_adjacent_space_error(i, j));
		if (is_adjacent_to_incomplete_row(i, j, map))
			return (print_incomplete_row_error(i, j));
	}
	return (0);
}

void	print_str(char *s)
{
	while (*s)
		write(1, s++, 1);
}
void	map_info_error(char **tokens, char *s)
{
	print_str(s);
	print_str(tokens[0]);
	print_str("\n");
	free_array(&tokens);
}

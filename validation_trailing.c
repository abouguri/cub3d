/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_trailing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:50:52 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:51:34 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	check_trailing_map_lines(int index)
{
	int		i;
	t_cub	*data;

	data = get_cub_data();
	i = index;
	i++;
	while (data->map[i])
	{
		if (strlen(data->map[i]) > 0)
			return (1);
		i++;
	}
	data->map[i] = NULL;
	return (0);
}

int	validate_trailing_lines(int i, t_cub *data)
{
	if (strlen(data->map[i]) == 0)
	{
		if (check_trailing_map_lines(i) == 1)
		{
			printf(TRAILING_EMPTY_LINES);
			return (1);
		}
	}
	return (0);
}

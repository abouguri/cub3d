/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_full_map.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:52:41 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:53:39 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	validate_full_map(void)
{
	t_cub	*data;
	int		player_count;

	data = get_cub_data();
	player_count = validate_characters();
	if (player_count != 1)
	{
		printf(SINGLE_PLAYER_NEEDED, player_count);
		return (1);
	}
	if (ft_array_length(data->map) < 3)
	{
		printf(SMALL_MAP);
		return (1);
	}
	if (validate_cells_and_players(data) == 1)
		return (1);
	return (0);
}

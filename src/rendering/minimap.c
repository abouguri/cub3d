/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:38:07 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 04:39:12 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_2d_map(t_game_state *game)
{
	t_cub			*data;
	t_data			*img;
	t_map_params	p;

	data = game->data;
	img = game->img;
	p.tile_size = 12;
	p.radius = 80;
	p.center_x = 100;
	p.center_y = 100;
	p.view_width = 15;
	p.view_height = 15;
	p.start_x = (int)data->var.position_x - p.view_width / 2;
	p.start_y = (int)data->var.position_y - p.view_height / 2;
	p.start_x = (p.start_x < 0) ? 0 : p.start_x;
	p.start_y = (p.start_y < 0) ? 0 : p.start_y;
	draw_circular_map_border(img, p.center_x, p.center_y, p.radius);
	draw_map_tiles_circular_zoomed(data, img, &p);
	draw_player_on_circular_map(data, img, &p);
	draw_enemies_on_circular_map(game, img, &p);
}

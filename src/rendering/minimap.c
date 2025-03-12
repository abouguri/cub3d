/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:38:07 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 03:28:21 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_enemies_on_circular_map(t_game_state *game, t_data *img,
		t_map_params *p)
{
	int			i;
	double		distance;
	t_enemy		*enemy;
	t_position	draw;

	if (!game->enemy_manager.enemies || game->enemy_manager.enemy_count <= 0)
		return ;
	i = 0;
	while (i < game->enemy_manager.enemy_count)
	{
		enemy = &game->enemy_manager.enemies[i];
		draw.x = p->center_x + ((int)enemy->pos_x - p->start_x - p->view_width
				/ 2) * p->tile_size;
		draw.y = p->center_y + ((int)enemy->pos_y - p->start_y - p->view_height
				/ 2) * p->tile_size;
		distance = sqrt(pow(draw.x - p->center_x, 2) + pow(draw.y - p->center_y,
					2));
		if (distance <= p->radius)
			fill_circle(img, draw, 3, RED);
		i++;
	}
}

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
	if (p.start_x < 0)
		p.start_x = 0;
	if (p.start_y < 0)
		p.start_y = 0;
	draw_circular_map_border(img, p.center_x, p.center_y, p.radius);
	draw_map_tiles_circular_zoomed(data, img, &p);
	draw_player_on_circular_map(data, img, &p);
	draw_enemies_on_circular_map(game, img, &p);
}

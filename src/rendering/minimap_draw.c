/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:39:29 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 05:48:00 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	draw_tile_if_in_radius(t_data *img, t_position draw_pos,
		char tile_type, t_map_params *p)
{
	double	distance;

	distance = sqrt(pow(draw_pos.x - p->center_x, 2) + pow(draw_pos.y
				- p->center_y, 2));
	if (distance > p->radius)
		return ;
	if (tile_type == '1')
		fill_circle(img, draw_pos, 3, CYAN);
	else if (tile_type == '0')
		fill_circle(img, draw_pos, 4, COOL_DARKGREY);
}

void	draw_map_tiles_circular_zoomed(t_cub *data, t_data *img,
		t_map_params *p)
{
	t_position	map_pos;
	t_position	draw_pos;
	char		tile_type;

	map_pos.y = -1;
	while (++map_pos.y < p->view_height)
	{
		if (p->start_y + map_pos.y >= get_map_height(data->map))
			break ;
		map_pos.x = -1;
		while (++map_pos.x < p->view_width)
		{
			if (p->start_x + map_pos.x >= (int)ft_strlen(data->map[p->start_y
						+ map_pos.y]))
				break ;
			draw_pos.x = p->center_x + (map_pos.x - p->view_width / 2)
				* p->tile_size;
			draw_pos.y = p->center_y + (map_pos.y - p->view_height / 2)
				* p->tile_size;
			tile_type = data->map[p->start_y + map_pos.y][p->start_x
				+ map_pos.x];
			draw_tile_if_in_radius(img, draw_pos, tile_type, p);
		}
	}
}

static void	init_line_params(t_line_params *line, t_position player,
		t_cub *data)
{
	line->start_x = player.x;
	line->start_y = player.y;
	line->end_x = player.x + (int)(10 * data->var.direction_x);
	line->end_y = player.y + (int)(10 * data->var.direction_y);
	line->dx = abs(line->end_x - line->start_x);
	if (line->start_x < line->end_x)
		line->sx = 1;
	else
		line->sx = -1;
	line->dy = -abs(line->end_y - line->start_y);
	if (line->start_y < line->end_y)
		line->sy = 1;
	else
		line->sy = -1;
	line->err = line->dx + line->dy;
}

void	draw_direction_line_circular(t_data *img, t_map_params *p, t_cub *data,
		t_position player)
{
	t_line_params	line;
	int				e2;
	double			distance;

	init_line_params(&line, player, data);
	while (1)
	{
		distance = sqrt(pow(line.start_x - p->center_x, 2) + pow(line.start_y
					- p->center_y, 2));
		if (distance <= p->radius)
			my_mlx_pixel_put(img, line.start_x, line.start_y, WHITE);
		if (line.start_x == line.end_x && line.start_y == line.end_y)
			break ;
		e2 = 2 * line.err;
		if (e2 >= line.dy)
		{
			line.err += line.dy;
			line.start_x += line.sx;
		}
		if (e2 <= line.dx)
		{
			line.err += line.dx;
			line.start_y += line.sy;
		}
	}
}

void	draw_player_on_circular_map(t_cub *data, t_data *img, t_map_params *p)
{
	t_position	player;
	double		distance;

	player.x = p->center_x + (int)((data->var.position_x - p->start_x
				- p->view_width / 2) * p->tile_size);
	player.y = p->center_y + (int)((data->var.position_y - p->start_y
				- p->view_height / 2) * p->tile_size);
	distance = sqrt(pow(player.x - p->center_x, 2) + pow(player.y - p->center_y,
				2));
	if (distance > p->radius)
		return ;
	fill_circle(img, player, 5, YELLOW);
	draw_direction_line_circular(img, p, data, player);
}

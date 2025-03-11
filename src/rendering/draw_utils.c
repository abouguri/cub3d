/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:15:56 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 04:16:47 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_floor_ceiling(t_data *img, int x, t_render *render, t_cub *data)
{
	int	y;

	y = 0;
	while (y < render->draw_start)
	{
		my_mlx_pixel_put(img, x, y, data->ceilling);
		y++;
	}
	y = render->draw_end;
	while (y < SCREEN_HEIGHT)
	{
		my_mlx_pixel_put(img, x, y, data->floor);
		y++;
	}
}

void	draw_textured_line(t_data *img, int x, t_render *render, t_cub *data)
{
	int		y;
	unsigned int		color;
	int		tex_y;
	double	step;
	double	tex_pos;
	int		pitch;

	step = 1.0 * TEXTURE_HEIGHT / render->line_height;
	pitch = -CAMERA_HEIGHT_OFFSET;
	tex_pos = (render->draw_start - (SCREEN_HEIGHT / 2 + pitch)
			+ render->line_height / 2) * step;
	y = render->draw_start;
	while (y < render->draw_end)
	{
		tex_y = (int)tex_pos & (TEXTURE_HEIGHT - 1);
		tex_pos += step;
		color = data->texture[render->tex_num][TEXTURE_HEIGHT * tex_y
			+ render->tex_x];
		t_rgb rgb = to_rgb(color);
		applyShading(&rgb, render->perp_wall_dist);
		color = to_int(rgb);
		// if (render->side == 1)
		// 	color = (color >> 1) & 8355711;
		my_mlx_pixel_put(img, x, y, color);
		y++;
	}
}

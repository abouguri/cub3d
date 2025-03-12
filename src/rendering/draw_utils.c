/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:15:56 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 03:10:37 by abouguri         ###   ########.fr       */
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
	int			y;
	int			pitch;
	t_tex_data	tex;

	tex.step = 1.0 * TEXTURE_HEIGHT / render->line_height;
	pitch = -CAMERA_HEIGHT_OFFSET;
	tex.tex_pos = (render->draw_start - (SCREEN_HEIGHT / 2 + pitch)
			+ render->line_height / 2) * tex.step;
	y = render->draw_start;
	while (y < render->draw_end)
	{
		tex.tex_y = (int)tex.tex_pos & (TEXTURE_HEIGHT - 1);
		tex.tex_pos += tex.step;
		tex.color = data->texture[render->tex_num][TEXTURE_HEIGHT * tex.tex_y
			+ render->tex_x];
		tex.rgb = to_rgb(tex.color);
		applyshading(&tex.rgb, render->perp_wall_dist);
		tex.color = to_int(tex.rgb);
		my_mlx_pixel_put(img, x, y, tex.color);
		y++;
	}
}

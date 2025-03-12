/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:12:06 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 03:11:46 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
	{
		return ;
	}
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

t_rgb	to_rgb(unsigned int clr)
{
	t_rgb	res;

	res.r = clr >> 16 & 0xFF;
	res.g = clr >> 8 & 0xFF;
	res.b = clr & 0xFF;
	return (res);
}

unsigned int	to_int(t_rgb rgb)
{
	return ((rgb.r << 16) | (rgb.g << 8) | rgb.b);
}

void	applyshading(t_rgb *rgb, double d)
{
	double	shading;

	shading = 1.0 - (d / MAX_DIST);
	if (shading < 0.4)
		shading = 0.4;
	rgb->r = (unsigned int)(rgb->r * shading + 0.5);
	rgb->g = (unsigned int)(rgb->g * shading + 0.5);
	rgb->b = (unsigned int)(rgb->b * shading + 0.5);
}

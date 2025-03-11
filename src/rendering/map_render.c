/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:20:25 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 06:01:15 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_circular_map_border(t_data *img, int center_x, int center_y, int radius)
{
	int	x;
	int	y;
	int	border_thick;
	int	outer_radius;
	int	inner_radius;

	border_thick = 3;
	outer_radius = radius + border_thick;
	inner_radius = radius - border_thick;
	y = -outer_radius;
	while (y <= outer_radius)
	{
		x = -outer_radius;
		while (x <= outer_radius)
		{
			if (is_in_circle_border(x, y, center_x, center_y, outer_radius,
					inner_radius))
			{
				my_mlx_pixel_put(img, center_x + x, center_y + y, PURPLE);
			}
			x++;
		}
		y++;
	}
}

void	fill_circle(t_data *img, int center_x, int center_y, int radius, int color)
{
	int		x;
	int		y;
	double	distance;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			distance = sqrt(x * x + y * y);
			if (distance <= radius)
				my_mlx_pixel_put(img, center_x + x, center_y + y, color);
			x++;
		}
		y++;
	}
}


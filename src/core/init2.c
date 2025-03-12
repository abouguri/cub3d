/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 03:34:39 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 03:34:44 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_camera_plane(double direction_x, double direction_y, double pla_x,
		double pla_y)
{
	t_cub	*data;

	data = get_cub_data();
	data->var.direction_x = direction_x;
	data->var.direction_y = direction_y;
	data->var.plane_x = pla_x;
	data->var.plane_y = pla_y;
}

void	init_player(int x, int y)
{
	t_cub	*data;

	data = get_cub_data();
	data->var.position_x = x + 0.5;
	data->var.position_y = y + 0.5;
	if (data->map[y][x] == NORTH)
		init_camera_plane(0, -1, 0.66, 0);
	else if (data->map[y][x] == SOUTH)
		init_camera_plane(0, 1, -0.66, 0);
	else if (data->map[y][x] == EAST)
		init_camera_plane(1, 0, 0, 0.66);
	else if (data->map[y][x] == WEST)
		init_camera_plane(-1, 0, 0, -0.66);
}

void	init_window(t_cub *data, t_data *img, int win_width, int win_height)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		error_exit_cleanup(ERR_INIT_MLX);
	data->win = mlx_new_window(data->mlx, win_width, win_height, "cub3D");
	if (!data->win)
		error_exit_cleanup(ERR_CREATE_WINDOW);
	img->img = mlx_new_image(data->mlx, win_width, win_height);
	if (!img->img)
		error_exit_cleanup(ERR_CREATE_IMAGE);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	if (!img->addr)
		error_exit_cleanup(ERR_GET_IMAGE_ADDR);
}

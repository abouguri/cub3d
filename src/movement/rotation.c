/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:55:38 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/10 23:58:41 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate_view(t_cub *data, double rot_speed, int direction)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = data->var.direction_x;
	data->var.direction_x = data->var.direction_x * cos(direction * rot_speed)
		- data->var.direction_y * sin(direction * rot_speed);
	data->var.direction_y = old_dir_x * sin(direction * rot_speed)
		+ data->var.direction_y * cos(direction * rot_speed);
	old_plane_x = data->var.plane_x;
	data->var.plane_x = data->var.plane_x * cos(direction * rot_speed)
		- data->var.plane_y * sin(direction * rot_speed);
	data->var.plane_y = old_plane_x * sin(direction * rot_speed)
		+ data->var.plane_y * cos(direction * rot_speed);
}

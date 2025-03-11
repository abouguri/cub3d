/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:06:54 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 00:10:30 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_valid_position(char **map, double pos_x, double pos_y)
{	
 	if (map[(int)pos_y][(int)pos_x] == '0' || map[(int)pos_y][(int)pos_x] == 'N'
	|| map[(int)pos_y][(int)pos_x] == 'S'
	|| map[(int)pos_y][(int)pos_x] == 'E'
	|| map[(int)pos_y][(int)pos_x] == 'W')
		return (1);
	return (0);
} 

void	move_strafe(t_cub *data, double move_speed, int direction)
{
	double	new_pos_x;
	double	new_pos_y;

	new_pos_x = data->var.position_x + direction * data->var.direction_y
		* move_speed;
	new_pos_y = data->var.position_y - direction * data->var.direction_x
		* move_speed;
	if (is_valid_position(data->map, new_pos_x, data->var.position_y))
		data->var.position_x = new_pos_x;
	if (is_valid_position(data->map, data->var.position_x, new_pos_y))
		data->var.position_y = new_pos_y;
}

void	move_forward_backward(t_cub *data, double move_speed, int direction)
{
	double	new_pos_x;
	double	new_pos_y;

	new_pos_x = data->var.position_x + direction * data->var.direction_x
		* move_speed;
	new_pos_y = data->var.position_y + direction * data->var.direction_y
		* move_speed;
	if (is_valid_position(data->map, new_pos_x, data->var.position_y))
		data->var.position_x = new_pos_x;
	if (is_valid_position(data->map, data->var.position_x, new_pos_y))
		data->var.position_y = new_pos_y;
}

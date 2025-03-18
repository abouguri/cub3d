/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:19:27 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/16 03:49:56 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	perform_dda(t_dda *dda, char **map)
{
	while (dda->hit == 0)
	{
		if (dda->side_dist_x < dda->side_dist_y)
		{
			dda->side_dist_x += dda->delta_dist_x;
			dda->map_x += dda->step_x;
			dda->side = 0;
		}
		else
		{
			dda->side_dist_y += dda->delta_dist_y;
			dda->map_y += dda->step_y;
			dda->side = 1;
		}
		if (map[dda->map_y][dda->map_x] == '1')
			dda->hit = 1;
	}
}

static void	set_step_and_side_dist(t_step_calc *calc)
{
	if (calc->ray_dir < 0)
	{
		*(calc->step) = -1;
		*(calc->side_dist) = (calc->pos - calc->map_pos) * calc->delta_dist;
	}
	else
	{
		*(calc->step) = 1;
		*(calc->side_dist) = (calc->map_pos + 1.0 - calc->pos)
			* calc->delta_dist;
	}
}

void	set_dda_params(t_dda *dda, t_var *var, double ray_dir_x,
		double ray_dir_y)
{
	t_step_calc	calc;

	dda->map_x = (int)var->position_x;
	dda->map_y = (int)var->position_y;
	dda->delta_dist_x = fabs(1 / ray_dir_x);
	dda->delta_dist_y = fabs(1 / ray_dir_y);
	dda->hit = 0;
	calc.step = &dda->step_x;
	calc.side_dist = &dda->side_dist_x;
	calc.ray_dir = ray_dir_x;
	calc.pos = var->position_x;
	calc.map_pos = dda->map_x;
	calc.delta_dist = dda->delta_dist_x;
	set_step_and_side_dist(&calc);
	calc.step = &dda->step_y;
	calc.side_dist = &dda->side_dist_y;
	calc.ray_dir = ray_dir_y;
	calc.pos = var->position_y;
	calc.map_pos = dda->map_y;
	calc.delta_dist = dda->delta_dist_y;
	set_step_and_side_dist(&calc);
}

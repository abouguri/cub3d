/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:25:00 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 02:33:11 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	calc_ray_pos_dir(t_var *var, int x, double *ray_dir_x,
		double *ray_dir_y)
{
	double	camera_x;

	camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
	*ray_dir_x = var->direction_x + var->plane_x * camera_x;
	*ray_dir_y = var->direction_y + var->plane_y * camera_x;
}

void	calc_wall_params(t_dda *dda, t_var *var, t_vector ray_dir,
		t_render *render)
{
	int	pitch;

	if (dda->side == 0)
		render->perp_wall_dist = (dda->map_x - var->position_x + (1
					- dda->step_x) / 2) / ray_dir.x;
	else
		render->perp_wall_dist = (dda->map_y - var->position_y + (1
					- dda->step_y) / 2) / ray_dir.y;
	render->line_height = (int)(SCREEN_HEIGHT / render->perp_wall_dist);
	pitch = -CAMERA_HEIGHT_OFFSET;
	render->draw_start = -render->line_height / 2 + SCREEN_HEIGHT / 2 + pitch;
	if (render->draw_start < 0)
		render->draw_start = 0;
	render->draw_end = render->line_height / 2 + SCREEN_HEIGHT / 2 + pitch;
	if (render->draw_end >= SCREEN_HEIGHT)
		render->draw_end = SCREEN_HEIGHT - 1;
	render->side = dda->side;
}

void	calc_texture_coords(t_dda *dda, t_var *var, t_render *render,
		t_vector ray_dir)
{
	double	wall_x;

	if (dda->side == 0)
		wall_x = var->position_y + (dda->map_x - var->position_x + (1
					- dda->step_x) / 2) / ray_dir.x * ray_dir.y;
	else
		wall_x = var->position_x + (dda->map_y - var->position_y + (1
					- dda->step_y) / 2) / ray_dir.y * ray_dir.x;
	wall_x -= floor(wall_x);
	render->tex_x = (int)(wall_x * (double)TEXTURE_WIDTH);
	if (dda->side == 0 && ray_dir.x > 0)
		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
	if (dda->side == 1 && ray_dir.y < 0)
		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
	render->tex_num = 0;
	if (dda->side == 0 && ray_dir.x < 0)
		render->tex_num = 0;
	if (dda->side == 0 && ray_dir.x > 0)
		render->tex_num = 1;
	if (dda->side == 1 && ray_dir.y < 0)
		render->tex_num = 2;
	if (dda->side == 1 && ray_dir.y > 0)
		render->tex_num = 3;
}

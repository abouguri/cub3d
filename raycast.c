/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:59:25 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/10 00:06:59 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	// Make sure we're not writing outside the image
    if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
	{
        return;
	}
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

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

#define CAMERA_HEIGHT_OFFSET 0
#define MAX_DIST 10.0
typedef struct s_rgb {
	unsigned int r;
	unsigned int g;
	unsigned int b;
} t_rgb;

t_rgb to_rgb(unsigned int clr) {
	t_rgb res;
	res.r = clr >> 16 & 0xFF;
	res.g = clr >> 8 & 0xFF;
	res.b = clr & 0xFF;
	return res;
}
void applyShading(t_rgb *rgb, double d)
{
    double shading = 1.0 - (d / MAX_DIST);
    if (shading < 0.4)
        shading = 0.4;
    
    // Apply shading and round properly
    rgb->r = (unsigned int)(rgb->r * shading + 0.5);
    rgb->g = (unsigned int)(rgb->g * shading + 0.5);
    rgb->b = (unsigned int)(rgb->b * shading + 0.5);
}

unsigned int to_int(t_rgb rgb) {
    return (rgb.r << 16) | (rgb.g << 8) | rgb.b;
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

void	calc_texture_coords(t_dda *dda, t_var *var, t_render *render,
		double ray_dir_x, double ray_dir_y)
{
	double	wall_x;

	if (dda->side == 0)
		wall_x = var->position_y + (dda->map_x - var->position_x + (1
					- dda->step_x) / 2) / ray_dir_x * ray_dir_y;
	else
		wall_x = var->position_x + (dda->map_y - var->position_y + (1
					- dda->step_y) / 2) / ray_dir_y * ray_dir_x;
	wall_x -= floor(wall_x);
	render->tex_x = (int)(wall_x * (double)TEXTURE_WIDTH);
	if (dda->side == 0 && ray_dir_x > 0)
		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
	if (dda->side == 1 && ray_dir_y < 0)
		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
	render->tex_num = 0;
	if (dda->side == 0 && ray_dir_x < 0)
		render->tex_num = 0;
	if (dda->side == 0 && ray_dir_x > 0)
		render->tex_num = 1;
	if (dda->side == 1 && ray_dir_y < 0)
		render->tex_num = 2;
	if (dda->side == 1 && ray_dir_y > 0)
		render->tex_num = 3;
}

void	calc_wall_params(t_dda *dda, t_var *var, double ray_dir_x,
		double ray_dir_y, t_render *render)
{
	int	pitch;

	if (dda->side == 0)
		render->perp_wall_dist = (dda->map_x - var->position_x + (1
					- dda->step_x) / 2) / ray_dir_x;
	else
		render->perp_wall_dist = (dda->map_y - var->position_y + (1
					- dda->step_y) / 2) / ray_dir_y;
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

void	set_dda_params(t_dda *dda, t_var *var, double ray_dir_x,
		double ray_dir_y)
{
	dda->map_x = (int)var->position_x;
	dda->map_y = (int)var->position_y;
	dda->delta_dist_x = fabs(1 / ray_dir_x);
	dda->delta_dist_y = fabs(1 / ray_dir_y);
	dda->hit = 0;
	if (ray_dir_x < 0)
	{
		dda->step_x = -1;
		dda->side_dist_x = (var->position_x - dda->map_x) * dda->delta_dist_x;
	}
	else
	{
		dda->step_x = 1;
		dda->side_dist_x = (dda->map_x + 1.0 - var->position_x)
			* dda->delta_dist_x;
	}
	if (ray_dir_y < 0)
	{
		dda->step_y = -1;
		dda->side_dist_y = (var->position_y - dda->map_y) * dda->delta_dist_y;
	}
	else
	{
		dda->step_y = 1;
		dda->side_dist_y = (dda->map_y + 1.0 - var->position_y)
			* dda->delta_dist_y;
	}
}

void	calc_ray_pos_dir(t_var *var, int x, double *ray_dir_x,
		double *ray_dir_y)
{
	double	camera_x;

	camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
	*ray_dir_x = var->direction_x + var->plane_x * camera_x;
	*ray_dir_y = var->direction_y + var->plane_y * camera_x;
}

int	get_map_height(char **map)
{
	int	height;

	height = 0;
	while (map[height])
		height++;
	return (height);
}

typedef struct s_map_params
{
	int	tile_size;
	int	offset_x;
	int	offset_y;
	int	radius;
	int	center_x;
	int	center_y;
	int	start_x;
	int	start_y;
	int	view_width;
	int	view_height;
	double rotation_angle; // Add this field
}		t_map_params;

void	rotate_point(int *x, int *y, int center_x, int center_y, double angle)
{
	int	original_x;
	int	original_y;

	original_x = *x - center_x;
	original_y = *y - center_y;
	// Apply rotation
	*x = center_x + (int)(original_x * cos(angle) - original_y * sin(angle));
	*y = center_y + (int)(original_x * sin(angle) + original_y * cos(angle));
}

int	is_in_circle_border(int x, int y, int center_x, int center_y,
		int outer_radius, int inner_radius)
{
	double	distance;

	(void)center_x;
	(void)center_y;
	distance = sqrt(x * x + y * y);
	return (distance <= outer_radius && distance >= inner_radius);
}

void	draw_circular_map_border(t_data *img, int center_x, int center_y,
		int radius)
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

void	fill_circle(t_data *img, int center_x, int center_y, int radius,
		int color)
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

void	draw_map_tiles_circular_zoomed(t_cub *data, t_data *img,
		t_map_params *p)
{
	int		x;
	int		y;
	int		draw_x;
	int		draw_y;
	double	distance;

	// int	color;
	y = -1;
	while (++y < p->view_height)
	{
		if (p->start_y + y >= get_map_height(data->map))
			break ;
		x = -1;
		while (++x < p->view_width)
		{
			if (p->start_x + x >= (int)strlen(data->map[p->start_y + y]))
				break ;
			// Center the view around the player
			draw_x = p->center_x + (x - p->view_width / 2) * p->tile_size;
			draw_y = p->center_y + (y - p->view_height / 2) * p->tile_size;
			// Check if tile is within circle
			distance = sqrt(pow(draw_x - p->center_x, 2) + pow(draw_y
						- p->center_y, 2));
			if (distance > p->radius)
				continue ;
			if (data->map[p->start_y + y][p->start_x + x] == '1')
				fill_circle(img, draw_x, draw_y, 3, CYAN);
			else if (data->map[p->start_y + y][p->start_x + x] == '0')
				fill_circle(img, draw_x, draw_y, 4, COOL_DARKGREY);
		}
	}
}

void	draw_direction_line_circular(t_data *img, int x0, int y0,
		t_map_params *p, t_cub *data)
{
	int		end_x;
	int		end_y;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	double	distance;

	end_x = x0 + (int)(10 * data->var.direction_x);
	end_y = y0 + (int)(10 * data->var.direction_y);
	dx = abs(end_x - x0);
	sx = x0 < end_x ? 1 : -1;
	dy = -abs(end_y - y0);
	sy = y0 < end_y ? 1 : -1;
	err = dx + dy;
	while (1)
	{
		// Check if line point is within the map circle
		distance = sqrt(pow(x0 - p->center_x, 2) + pow(y0 - p->center_y, 2));
		if (distance <= p->radius)
			my_mlx_pixel_put(img, x0, y0, WHITE);
		if (x0 == end_x && y0 == end_y)
			break ;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void	draw_player_on_circular_map(t_cub *data, t_data *img, t_map_params *p)
{
	int		player_x;
	int		player_y;
	double	distance;

	player_x = p->center_x + (int)((data->var.position_x - p->start_x
				- p->view_width / 2) * p->tile_size);
	player_y = p->center_y + (int)((data->var.position_y - p->start_y
				- p->view_height / 2) * p->tile_size);
	distance = sqrt(pow(player_x - p->center_x, 2) + pow(player_y - p->center_y,
				2));
	if (distance > p->radius)
		return ;
	fill_circle(img, player_x, player_y, 5, YELLOW);
	draw_direction_line_circular(img, player_x, player_y, p, data);
}

// Function to draw enemies on the 2D circular map
void draw_enemies_on_circular_map(t_game_state *game, t_data *img, t_map_params *p)
{
    int i;
    int draw_x, draw_y;
    double distance;
    
    // Check if enemies exist
    if (!game->enemy_manager.enemies || game->enemy_manager.enemy_count <= 0)
        return;
    
    // Draw each enemy
    for (i = 0; i < game->enemy_manager.enemy_count; i++)
    {
        t_enemy *enemy = &game->enemy_manager.enemies[i];
        
        // Calculate drawing position (similar to player drawing)
        draw_x = p->center_x + ((int)enemy->pos_x - p->start_x - p->view_width / 2) * p->tile_size;
        draw_y = p->center_y + ((int)enemy->pos_y - p->start_y - p->view_height / 2) * p->tile_size;
        
        // Check if enemy is within the circular view
        distance = sqrt(pow(draw_x - p->center_x, 2) + pow(draw_y - p->center_y, 2));
        if (distance > p->radius)
            continue;
        
        // Draw enemy (red circle)
        fill_circle(img, draw_x, draw_y, 3, RED);
    }
}

void	draw_2d_map(t_game_state *game)
{
	t_cub *data = game->data;
	t_data *img = game->img;
	t_map_params	p;

	p.tile_size = 12;
	p.radius = 80;
	p.center_x = 100;
	p.center_y = 100;
	p.view_width = 15;
	p.view_height = 15;
	p.start_x = (int)data->var.position_x - p.view_width / 2;
	p.start_y = (int)data->var.position_y - p.view_height / 2;
	p.start_x = (p.start_x < 0) ? 0 : p.start_x;
	p.start_y = (p.start_y < 0) ? 0 : p.start_y;
	draw_circular_map_border(img, p.center_x, p.center_y, p.radius);
	draw_map_tiles_circular_zoomed(data, img, &p);
	draw_player_on_circular_map(data, img, &p);
	draw_enemies_on_circular_map(game, img, &p);
}

void raycast(t_game_state *game)
{
    t_cub *data = game->data;
    t_data *img = game->img;
    int x;
    double ray_dir_x;
    double ray_dir_y;
    t_dda dda;
    t_render render;

    x = 0;
    while (x < SCREEN_WIDTH)
    {
        calc_ray_pos_dir(&data->var, x, &ray_dir_x, &ray_dir_y);
        set_dda_params(&dda, &data->var, ray_dir_x, ray_dir_y);
        perform_dda(&dda, data->map);
        calc_wall_params(&dda, &data->var, ray_dir_x, ray_dir_y, &render);
        
        // Store the perpendicular wall distance in the z_buffer
        game->z_buffer[x] = render.perp_wall_dist;
        
        calc_texture_coords(&dda, &data->var, &render, ray_dir_x, ray_dir_y);
        draw_floor_ceiling(img, x, &render, data);
        draw_textured_line(img, x, &render, data);
        x++;
    }
	   // Only try to update enemies if they were successfully initialized
    if (game->enemy_manager.enemies != NULL) {
        update_enemies(game);
        render_enemies(game, game->z_buffer);
    }
    draw_2d_map(game);
}

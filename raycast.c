/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:59:25 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/05 19:58:27 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// void my_mlx_pixel_put(t_data *data, int x, int y, int color)
// {
//     char *dst;

//     if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
//     {
//         printf("Warning: Attempt to draw outside screen bounds (x=%d, y=%d)\n", x, y);
//         return;
//     }

//     dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
//     *(unsigned int *)dst = color;

//     printf("Debug: Pixel drawn at (%d, %d) with color 0x%X\n", x, y, color);
// }


void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;
	// if(x > SCREEN_WIDTH || y > SCREEN_HEIGHT)
	// 	return;
	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

/*
** Draw floor and ceiling
*/
void	draw_floor_ceiling(t_data *img, int x, t_render *render, t_cub *data)
{
	int	y;

	// Draw ceiling
	y = 0;

	while (y < render->draw_start)
	{
		my_mlx_pixel_put(img, x, y, data->ceilling);
		y++;
	}
	// Draw floor
	y = render->draw_end;
	while (y < SCREEN_HEIGHT)
	{
		my_mlx_pixel_put(img, x, y, data->floor);
		y++;
	}
}

// void	draw_floor_ceiling(t_data *img, int x, t_render *render, t_cub *data)
// {
// 	int	y;

// 	// Debug: Print ceiling and floor colors
// 	printf("Debug: Ceiling Color = 0x%lX, Floor Color = 0x%lX\n",
// 	       (unsigned long)data->ceilling, (unsigned long)data->floor);

// 	// Draw ceiling
// 	y = 0;
// 	while (y < render->draw_start)
// 	{
// 		my_mlx_pixel_put(img, x, y, data->ceilling);
// 		printf("Debug: Ceiling Pixel at (%d, %d) = 0x%lX\n",
// 		       x, y, (unsigned long)data->ceilling);
// 		y++;
// 	}

// 	// Draw floor
// 	y = render->draw_end;
// 	while (y < SCREEN_HEIGHT)
// 	{
// 		my_mlx_pixel_put(img, x, y, data->floor);
// 		printf("Debug: Floor Pixel at (%d, %d) = 0x%lX\n",
// 		       x, y, (unsigned long)data->floor);
// 		y++;
// 	}
// }


// void draw_floor_ceiling(t_data *img, int x, t_render *render, t_cub *data)
// {
//     int y;

//     y = 0;
//     while (y < render->draw_start)
//     {
//         my_mlx_pixel_put(img, x, y, data->ceilling);
//         if (x == SCREEN_WIDTH / 2 && y == SCREEN_HEIGHT / 2) // Check middle pixel
//             printf("Debug: Ceiling pixel set at (%d, %d) with color: %lX\n", x, y, data->ceilling);
//         y++;
//     }

//     y = render->draw_end;
//     while (y < SCREEN_HEIGHT)
//     {
//         my_mlx_pixel_put(img, x, y, data->floor);
//         if (x == SCREEN_WIDTH / 2 && y == SCREEN_HEIGHT / 2) // Check middle pixel
//             printf("Debug: Floor pixel set at (%d, %d) with color: %lX\n", x, y, data->floor);
//         y++;
//     }
// }


/*
** Draw the vertical line with texture
*/
// void	draw_textured_line(t_data *img, int x, t_render *render, t_cub *data)
// {
// 	int		y;
// 	int		color;
// 	int		tex_y;
// 	double	step;
// 	double	tex_pos;

// 	step = 1.0 * TEXTURE_HEIGHT / render->line_height;
// 	tex_pos = (render->draw_start - SCREEN_HEIGHT / 2
// 			+ render->line_height / 2) * step;
// 	y = render->draw_start;
// 	while (y < render->draw_end)
// 	{
// 		tex_y = (int)tex_pos & (TEXTURE_HEIGHT - 1);
// 		tex_pos += step;
// 		color = data->texture[render->tex_num][TEXTURE_HEIGHT
// 			* tex_y + render->tex_x];
// 		if (render->side == 1)
// 			color = (color >> 1) & 8355711;  // Make color darker for y-sides
// 		my_mlx_pixel_put(img, x, y, color);
// 		y++;
// 	}
// }
#define CAMERA_HEIGHT_OFFSET 200  // Adjust this value as needed

void draw_textured_line(t_data *img, int x, t_render *render, t_cub *data)
{
    int     y;
    int     color;
    int     tex_y;
    double  step;
    double  tex_pos;

    step = 1.0 * TEXTURE_HEIGHT / render->line_height;
    
    int pitch = -CAMERA_HEIGHT_OFFSET;
    tex_pos = (render->draw_start - (SCREEN_HEIGHT / 2 + pitch) 
            + render->line_height / 2) * step;
            
    y = render->draw_start;
    while (y < render->draw_end)
    {
        tex_y = (int)tex_pos & (TEXTURE_HEIGHT - 1);
        tex_pos += step;
        color = data->texture[render->tex_num][TEXTURE_HEIGHT
            * tex_y + render->tex_x];
        if (render->side == 1)
            color = (color >> 1) & 8355711;
        my_mlx_pixel_put(img, x, y, color);
        y++;
    }
}

/*
** Calculate texture coordinates
*/
// void	calc_texture_coords(t_dda *dda, t_var *var, t_render *render,
// 		double ray_dir_x, double ray_dir_y)
// {
// 	double	wall_x;

// 	if (dda->side == 0)
// 		wall_x = var->position_y + render->perp_wall_dist * ray_dir_y;
// 	else
// 		wall_x = var->position_x + render->perp_wall_dist * ray_dir_x;

// 	wall_x -= floor(wall_x);
	
// 	render->tex_x = (int)(wall_x * (double)TEXTURE_WIDTH);

// 	if (dda->side == 0 && ray_dir_x > 0)
// 		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
// 	if (dda->side == 1 && ray_dir_y < 0)
// 		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
// 	render->tex_num = 0;
// 	if (dda->side == 0 && ray_dir_x < 0)
// 		render->tex_num = 0;  // North texture
// 	if (dda->side == 0 && ray_dir_x > 0)
// 		render->tex_num = 1;  // South texture
// 	if (dda->side == 1 && ray_dir_y < 0)
// 		render->tex_num = 2;  // West texture
// 	if (dda->side == 1 && ray_dir_y > 0)
// 		render->tex_num = 3;  // East texture
// }

void	calc_texture_coords(t_dda *dda, t_var *var, t_render *render,
		double ray_dir_x, double ray_dir_y)
{
	double	wall_x;

	// The issue is here: don't use position + perp_wall_dist
	if (dda->side == 0)
		wall_x = var->position_y + (dda->map_x - var->position_x + 
			(1 - dda->step_x) / 2) / ray_dir_x * ray_dir_y;
	else
		wall_x = var->position_x + (dda->map_y - var->position_y + 
			(1 - dda->step_y) / 2) / ray_dir_y * ray_dir_x;

	wall_x -= floor(wall_x);
	
	render->tex_x = (int)(wall_x * (double)TEXTURE_WIDTH);

	if (dda->side == 0 && ray_dir_x > 0)
		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
	if (dda->side == 1 && ray_dir_y < 0)
		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;
	render->tex_num = 0;
	if (dda->side == 0 && ray_dir_x < 0)
		render->tex_num = 0;  // North texture
	if (dda->side == 0 && ray_dir_x > 0)
		render->tex_num = 1;  // South texture
	if (dda->side == 1 && ray_dir_y < 0)
		render->tex_num = 2;  // West texture
	if (dda->side == 1 && ray_dir_y > 0)
		render->tex_num = 3;  // East texture
}

// void	calc_texture_coords(t_dda *dda, t_var *var, t_render *render,
// 		double ray_dir_x, double ray_dir_y)
// {
// 	double	wall_x;

// 	// Instead of using perp_wall_dist, compute wall_x based on the map hit location
// 	if (dda->side == 0)
// 		wall_x = var->position_y + (dda->map_x - var->position_x + (1 - dda->step_x) / 2) * (ray_dir_y / ray_dir_x);
// 	else
// 		wall_x = var->position_x + (dda->map_y - var->position_y + (1 - dda->step_y) / 2) * (ray_dir_x / ray_dir_y);

// 	wall_x -= floor(wall_x);  // Remove the integer part, keeping only fractional component

// 	// Compute texture X coordinate
// 	render->tex_x = (int)(wall_x * TEXTURE_WIDTH);
// 	if (render->tex_x < 0)
// 		render->tex_x = 0;
// 	if (render->tex_x >= TEXTURE_WIDTH)
// 		render->tex_x = TEXTURE_WIDTH - 1;

// 	// Correct mirroring effect
// 	if ((dda->side == 0 && ray_dir_x > 0) || (dda->side == 1 && ray_dir_y < 0))
// 		render->tex_x = TEXTURE_WIDTH - render->tex_x - 1;

// 	// Debugging
// 	// printf("wall_x: %f, tex_x: %d\n", wall_x, render->tex_x);

// 	// Assign correct texture index
// 	if (dda->side == 0)
// 		render->tex_num = (ray_dir_x > 0) ? 1 : 0;  // 0 = North, 1 = South
// 	else
// 		render->tex_num = (ray_dir_y > 0) ? 3 : 2;  // 2 = West, 3 = East
// }





// Add this before your function or as a constant

// Then modify your draw_start and draw_end calculations
/*
** Calculate wall distance, height and draw start/end positions
*/
// void	calc_wall_params(t_dda *dda, t_var *var, double ray_dir_x,
// 		double ray_dir_y, t_render *render)
// {
// 	if (dda->side == 0)
//     	render->perp_wall_dist = (dda->map_x - var->position_x + (1 - dda->step_x) / 2) / ray_dir_x;
// 	else
//     	render->perp_wall_dist = (dda->map_y - var->position_y + (1 - dda->step_y) / 2) / ray_dir_y;

// 	render->line_height = (int)(SCREEN_HEIGHT / render->perp_wall_dist);
// 	render->draw_start = -render->line_height / 2 + SCREEN_HEIGHT / 2 - CAMERA_HEIGHT_OFFSET;

// 	if (render->draw_start < 0)
// 		render->draw_start = 0;
// 	render->draw_end = render->line_height / 2 + SCREEN_HEIGHT / 2 - CAMERA_HEIGHT_OFFSET;

// 	if (render->draw_end >= SCREEN_HEIGHT)
// 		render->draw_end = SCREEN_HEIGHT - 1;
// }

void calc_wall_params(t_dda *dda, t_var *var, double ray_dir_x,
        double ray_dir_y, t_render *render)
{
    if (dda->side == 0)
        render->perp_wall_dist = (dda->map_x - var->position_x + (1 - dda->step_x) / 2) / ray_dir_x;
    else
        render->perp_wall_dist = (dda->map_y - var->position_y + (1 - dda->step_y) / 2) / ray_dir_y;

    render->line_height = (int)(SCREEN_HEIGHT / render->perp_wall_dist);
    
    // Use negative pitch to move the horizon line up
    int pitch = -CAMERA_HEIGHT_OFFSET;
    
    // Apply the pitch/camera height to the wall positioning
    render->draw_start = -render->line_height / 2 + SCREEN_HEIGHT / 2 + pitch;
    if (render->draw_start < 0)
        render->draw_start = 0;
    
    render->draw_end = render->line_height / 2 + SCREEN_HEIGHT / 2 + pitch;
    if (render->draw_end >= SCREEN_HEIGHT)
        render->draw_end = SCREEN_HEIGHT - 1;
}

/*
** Perform DDA (Digital Differential Analysis) algorithm to find wall hit
*/
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
/*
** Set the initial DDA parameters
*/
void	set_dda_params(t_dda *dda, t_var *var, double ray_dir_x, double ray_dir_y)
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
/*
** Calculate ray position and direction for each x-coordinate on screen
*/
void	calc_ray_pos_dir(t_var *var, int x, double *ray_dir_x, double *ray_dir_y)
{
	double	camera_x;

	camera_x = 2 * x / (double)SCREEN_WIDTH - 1;
	*ray_dir_x = var->direction_x + var->plane_x * camera_x;
	*ray_dir_y = var->direction_y + var->plane_y * camera_x;
}


// Helper function to get map height
int	get_map_height(char **map)
{
	int	height;

	height = 0;
	while (map[height])
		height++;
	return (height);
}

// void	draw_direction_line_on_circular_map(t_data *img, int x0, int y0, 
// 											int center_x, int center_y, 
// 											int map_radius, t_cub *data)
// {
// 	int		end_x;
// 	int		end_y;
// 	int		dx;
// 	int		dy;
// 	int		sx;
// 	int		sy;
// 	int		err;
// 	int		e2;
// 	double	distance;

// 	end_x = x0 + (int)(10 * data->var.direction_x);
// 	end_y = y0 + (int)(10 * data->var.direction_y);
	
// 	dx = abs(end_x - x0);
// 	sx = x0 < end_x ? 1 : -1;
// 	dy = -abs(end_y - y0);
// 	sy = y0 < end_y ? 1 : -1;
// 	err = dx + dy;

// 	while (1)
// 	{
// 		// Check if line point is within the map circle
// 		distance = sqrt(pow(x0 - center_x, 2) + pow(y0 - center_y, 2));
		
// 		if (distance <= map_radius)
// 			my_mlx_pixel_put(img, x0, y0, 0x00FF00);
		
// 		if (x0 == end_x && y0 == end_y)
// 			break;
		
// 		e2 = 2 * err;
// 		if (e2 >= dy)
// 		{
// 			err += dy;
// 			x0 += sx;
// 		}
// 		if (e2 <= dx)
// 		{
// 			err += dx;
// 			y0 += sy;
// 		}
// 	}
// }

// void	draw_player_on_circular_map(t_cub *data, t_data *img, int tile_size, 
// 									int center_x, int center_y, 
// 									int player_rel_x, int player_rel_y,
// 									int map_radius)
// {
// 	int		player_tile_x;
// 	int		player_tile_y;
// 	int		dx;
// 	int		dy;
// 	double	distance;

// 	player_tile_x = center_x + player_rel_x * tile_size;
// 	player_tile_y = center_y + player_rel_y * tile_size;
	
// 	dy = -2;
// 	while (dy <= 2)
// 	{
// 		dx = -2;
// 		while (dx <= 2)
// 		{
// 			// Check if player dot is within the map circle
// 			distance = sqrt(pow(player_tile_x + dx - center_x, 2) + 
// 						    pow(player_tile_y + dy - center_y, 2));
			
// 			if (distance <= map_radius)
// 				my_mlx_pixel_put(img, player_tile_x + dx, player_tile_y + dy, 0xFF0000);
// 			dx++;
// 		}
// 		dy++;
// 	}

// 	draw_direction_line_on_circular_map(img, player_tile_x, player_tile_y, 
// 										center_x, center_y, map_radius, data);
// }


// void	draw_2d_map(t_cub *data, t_data *img)
// {
// 	int		tile_size;
// 	int		map_radius;
// 	int		center_x;
// 	int		center_y;
// 	int		start_x;
// 	int		start_y;
// 	// int		end_x;
// 	// int		end_y;
// 	int		x;
// 	int		y;
// 	int		draw_x;
// 	int		draw_y;
// 	int		color;
// 	double	distance;

// 	// Configurable parameters
// 	tile_size = 10;
// 	map_radius = 75;  // Radius of the circular map
// 	center_x = 100;   // X position of map center
// 	center_y = 100;   // Y position of map center

// 	// Calculate the start and end of the visible map area
// 	start_x = (int)data->var.position_x - map_radius / tile_size / 2;
// 	start_y = (int)data->var.position_y - map_radius / tile_size / 2;
// 	// end_x = start_x + map_radius / tile_size;
// 	// end_y = start_y + map_radius / tile_size;

// 	// Adjust start and end to prevent out-of-bounds access
// 	start_x = (start_x < 0) ? 0 : start_x;
// 	start_y = (start_y < 0) ? 0 : start_y;

// 	y = -1;
// 	while (++y < map_radius / tile_size)
// 	{
// 		// Check if this map row exists
// 		if (start_y + y >= get_map_height(data->map))
// 			break;

// 		x = -1;
// 		while (++x < map_radius / tile_size)
// 		{
// 			// Check if this map column exists
// 			if (start_x + x >= (int)strlen(data->map[start_y + y]))
// 				break;

// 			draw_x = center_x + x * tile_size;
// 			draw_y = center_y + y * tile_size;
			
// 			// Calculate distance from map center
// 			distance = sqrt(pow(draw_x - center_x, 2) + pow(draw_y - center_y, 2));
			
// 			// Skip if outside circle
// 			if (distance > map_radius)
// 				continue;

// 			// Determine tile color
// 			color = 0x808080; // Default gray
// 			if (data->map[start_y + y][start_x + x] == '1')
// 				color = 0x333333; // Dark gray for walls
// 			else if (data->map[start_y + y][start_x + x] == '0')
// 				color = 0xCCCCCC; // Light gray for open spaces

// 			// Draw tile
// 			if (distance > map_radius - 3)  // Black border
// 				color = 0x000000;

// 			my_mlx_pixel_put(img, draw_x, draw_y, color);
// 		}
// 	}

// 	// Draw player in the center of the view
// 	draw_player_on_circular_map(data, img, tile_size, center_x, center_y, 
// 								(int)(data->var.position_x - start_x), 
// 								(int)(data->var.position_y - start_y),
// 								map_radius);
// }

typedef struct s_map_params
{
	int		tile_size;
	int		offset_x;
	int		offset_y;
	int		radius;
	int		center_x;
	int		center_y;
	int		start_x;
	int		start_y;
	int		view_width;
	int		view_height;
}	t_map_params;

int	is_in_circle_border(int x, int y, int center_x, int center_y, 
						   int outer_radius, int inner_radius)
{
	(void)center_x;
	(void)center_y;
	double	distance;

	distance = sqrt(x * x + y * y);
	return (distance <= outer_radius && distance >= inner_radius);
}

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
			if (is_in_circle_border(x, y, center_x, center_y, 
									outer_radius, inner_radius))
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
	int	x;
	int	y;
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

void	draw_map_tiles_circular_zoomed(t_cub *data, t_data *img, t_map_params *p)
{
	int	x;
	int	y;
	int	draw_x;
	int	draw_y;
	// int	color;
	double	distance;

	y = -1;
	while (++y < p->view_height)
	{
		if (p->start_y + y >= get_map_height(data->map))
			break;

		x = -1;
		while (++x < p->view_width)
		{
			if (p->start_x + x >= (int)strlen(data->map[p->start_y + y]))
				break;

			// Center the view around the player
			draw_x = p->center_x + (x - p->view_width / 2) * p->tile_size;
			draw_y = p->center_y + (y - p->view_height / 2) * p->tile_size;
			
			// Check if tile is within circle
			distance = sqrt(pow(draw_x - p->center_x, 2) + 
						    pow(draw_y - p->center_y, 2));
			if (distance > p->radius)
				continue;

			// Draw different sized dots based on map tile
			if (data->map[p->start_y + y][p->start_x + x] == '1')
				fill_circle(img, draw_x, draw_y, 3, CYAN);
			else if (data->map[p->start_y + y][p->start_x + x] == '0')
				fill_circle(img, draw_x, draw_y, 4, COOL_DARKGREY);
		}
	}
}

void	draw_map_tiles_circular(t_cub *data, t_data *img, t_map_params *p)
{
	int	x;
	int	y;
	int	draw_x;
	int	draw_y;
	// int	color;
	double	distance;

	y = -1;
	while (++y < p->view_height)
	{
		if (p->start_y + y >= get_map_height(data->map))
			break;

		x = -1;
		while (++x < p->view_width)
		{
			if (p->start_x + x >= (int)strlen(data->map[p->start_y + y]))
				break;

			draw_x = p->center_x + (x - p->view_width / 2) * p->tile_size;
			draw_y = p->center_y + (y - p->view_height / 2) * p->tile_size;
			
			// Check if tile is within circle
			distance = sqrt(pow(draw_x - p->center_x, 2) + 
						    pow(draw_y - p->center_y, 2));
			if (distance > p->radius)
				continue;

			// Draw different sized dots based on map tile
			if (data->map[p->start_y + y][p->start_x + x] == '1')
				fill_circle(img, draw_x, draw_y, 3, CYAN);
			else if (data->map[p->start_y + y][p->start_x + x] == '0')
				fill_circle(img, draw_x, draw_y, 3, COOL_DARKGREY);
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
			break;
		
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

	player_x = p->center_x + 
			   (int)((data->var.position_x - p->start_x - p->view_width / 2) * p->tile_size);
	player_y = p->center_y + 
			   (int)((data->var.position_y - p->start_y - p->view_height / 2) * p->tile_size);
	
	// Check if player is within circle
	distance = sqrt(pow(player_x - p->center_x, 2) + 
					pow(player_y - p->center_y, 2));
	if (distance > p->radius)
		return ;

	// Draw player
	fill_circle(img, player_x, player_y, 5, YELLOW);

	// Draw direction line
	draw_direction_line_circular(img, player_x, player_y, p, data);
}

void	draw_2d_map(t_cub *data, t_data *img)
{
	t_map_params	p;

	p.tile_size = 12;
	p.radius = 80;
	p.center_x = 100;
	p.center_y = 100;
	p.view_width = 15;
	p.view_height = 15;

	// p.tile_size = 15;  // Increased tile size for zoom effect
	// p.radius = 80;     // Slightly reduced radius
	// p.center_x = 150;  // Fixed center point
	// p.center_y = 150;  // Fixed center point
	// p.view_width = 7;  // Reduced view width
	// p.view_height = 7; // Reduced view height

	p.start_x = (int)data->var.position_x - p.view_width / 2;
	p.start_y = (int)data->var.position_y - p.view_height / 2;
	p.start_x = (p.start_x < 0) ? 0 : p.start_x;
	p.start_y = (p.start_y < 0) ? 0 : p.start_y;

	draw_circular_map_border(img, p.center_x, p.center_y, p.radius);
	// draw_map_tiles_circular(data, img, &p);
	draw_map_tiles_circular_zoomed(data, img, &p);
	draw_player_on_circular_map(data, img, &p);
}








// void	draw_direction_line(t_data *img, int x0, int y0, t_cub *data)
// {
// 	int	end_x;
// 	int	end_y;
// 	int	dx;
// 	int	dy;
// 	int	sx;
// 	int	sy;
// 	int	err;
// 	int	e2;

// 	end_x = x0 + (int)(10 * data->var.direction_x);
// 	end_y = y0 + (int)(10 * data->var.direction_y);
	
// 	dx = abs(end_x - x0);
// 	sx = x0 < end_x ? 1 : -1;
// 	dy = -abs(end_y - y0);
// 	sy = y0 < end_y ? 1 : -1;
// 	err = dx + dy;

// 	while (1)
// 	{
// 		my_mlx_pixel_put(img, x0, y0, 0x00FF00);
		
// 		if (x0 == end_x && y0 == end_y)
// 			break;
		
// 		e2 = 2 * err;
// 		if (e2 >= dy)
// 		{
// 			err += dy;
// 			x0 += sx;
// 		}
// 		if (e2 <= dx)
// 		{
// 			err += dx;
// 			y0 += sy;
// 		}
// 	}
// }

// void	draw_player_on_map(t_cub *data, t_data *img, int tile_size, int offset_x, int offset_y, 
// 						   int player_rel_x, int player_rel_y)
// {
// 	int	player_tile_x;
// 	int	player_tile_y;
// 	int	dx;
// 	int	dy;

// 	player_tile_x = offset_x + player_rel_x * tile_size;
// 	player_tile_y = offset_y + player_rel_y * tile_size;
	
// 	dy = -2;
// 	while (dy <= 2)
// 	{
// 		dx = -2;
// 		while (dx <= 2)
// 		{
// 			my_mlx_pixel_put(img, player_tile_x + dx, player_tile_y + dy, 0xFF0000);
// 			dx++;
// 		}
// 		dy++;
// 	}

// 	draw_direction_line(img, player_tile_x, player_tile_y, data);
// }

// void	draw_2d_map(t_cub *data, t_data *img)
// {
// 	int		tile_size;
// 	int		offset_x;
// 	int		offset_y;
// 	int		map_view_width;
// 	int		map_view_height;
// 	int		start_x;
// 	int		start_y;
// 	// int		end_x;
// 	// int		end_y;
// 	int		x;
// 	int		y;
// 	int		draw_x;
// 	int		draw_y;
// 	int		color;
// 	int		dx;
// 	int		dy;

// 	// Configurable parameters
// 	tile_size = 10;
// 	offset_x = 20;
// 	offset_y = 20;
// 	map_view_width = 15;  // Number of tiles to show horizontally
// 	map_view_height = 15; // Number of tiles to show vertically

// 	// Calculate the start and end of the visible map area
// 	start_x = (int)data->var.position_x - map_view_width / 2;
// 	start_y = (int)data->var.position_y - map_view_height / 2;
// 	// end_x = start_x + map_view_width;
// 	// end_y = start_y + map_view_height;

// 	// Adjust start and end to prevent out-of-bounds access
// 	start_x = (start_x < 0) ? 0 : start_x;
// 	start_y = (start_y < 0) ? 0 : start_y;

// 	y = -1;
// 	while (++y < map_view_height)
// 	{
// 		// Check if this map row exists
// 		if (start_y + y >= get_map_height(data->map))
// 			break;

// 		x = -1;
// 		while (++x < map_view_width)
// 		{
// 			// Check if this map column exists
// 			if (start_x + x >= (int)strlen(data->map[start_y + y]))
// 				break;

// 			draw_x = offset_x + x * tile_size;
// 			draw_y = offset_y + y * tile_size;
			
// 			// Determine tile color
// 			color = 0x808080; // Default gray
// 			if (data->map[start_y + y][start_x + x] == '1')
// 				color = 0x333333; // Dark gray for walls
// 			else if (data->map[start_y + y][start_x + x] == '0')
// 				color = 0xCCCCCC; // Light gray for open spaces

// 			// Draw tile
// 			dy = -1;
// 			while (++dy < tile_size)
// 			{
// 				dx = -1;
// 				while (++dx < tile_size)
// 				{
// 					if (dx == 0 || dy == 0 || dx == tile_size - 1 || dy == tile_size - 1)
// 						my_mlx_pixel_put(img, draw_x + dx, draw_y + dy, 0x000000);
// 					else
// 						my_mlx_pixel_put(img, draw_x + dx, draw_y + dy, color);
// 				}
// 			}
// 		}
// 	}

// 	// Draw player in the center of the view
// 	draw_player_on_map(data, img, tile_size, offset_x, offset_y, 
// 					   (int)(data->var.position_x - start_x), 
// 					   (int)(data->var.position_y - start_y));
// }




// +++++++++++++++++++++

// void	draw_direction_line(t_data *img, int x0, int y0, t_cub *data)
// {
// 	int	end_x;
// 	int	end_y;
// 	int	dx;
// 	int	dy;
// 	int	sx;
// 	int	sy;
// 	int	err;
// 	int	e2;

// 	end_x = x0 + (int)(10 * data->var.direction_x);
// 	end_y = y0 + (int)(10 * data->var.direction_y);
	
// 	dx = abs(end_x - x0);
// 	sx = x0 < end_x ? 1 : -1;
// 	dy = -abs(end_y - y0);
// 	sy = y0 < end_y ? 1 : -1;
// 	err = dx + dy;

// 	while (1)
// 	{
// 		my_mlx_pixel_put(img, x0, y0, 0x00FF00);
		
// 		if (x0 == end_x && y0 == end_y)
// 			break;
		
// 		e2 = 2 * err;
// 		if (e2 >= dy)
// 		{
// 			err += dy;
// 			x0 += sx;
// 		}
// 		if (e2 <= dx)
// 		{
// 			err += dx;
// 			y0 += sy;
// 		}
// 	}
// }

// void	draw_player_on_map(t_cub *data, t_data *img, int tile_size, int offset_x, int offset_y)
// {
// 	int	player_tile_x;
// 	int	player_tile_y;
// 	int	dx;
// 	int	dy;
// 	// int	end_x;
// 	// int	end_y;

// 	player_tile_x = offset_x + (int)(data->var.position_x * tile_size);
// 	player_tile_y = offset_y + (int)(data->var.position_y * tile_size);
	
// 	dy = -2;
// 	while (dy <= 2)
// 	{
// 		dx = -2;
// 		while (dx <= 2)
// 		{
// 			my_mlx_pixel_put(img, player_tile_x + dx, player_tile_y + dy, 0xFF0000);
// 			dx++;
// 		}
// 		dy++;
// 	}

// 	draw_direction_line(img, player_tile_x, player_tile_y, data);
// }

// void	draw_2d_map(t_cub *data, t_data *img)
// {
// 	// int	map_width;
// 	int	map_height;
// 	int	tile_size;
// 	int	offset_x;
// 	int	offset_y;
// 	int	x;
// 	int	y;
// 	int	draw_x;
// 	int	draw_y;
// 	int	color;
// 	int	dx;
// 	int	dy;

// 	// map_width = 0;
// 	map_height = 0;
// 	tile_size = 8;
// 	offset_x = 20;
// 	offset_y = 20;

// 	while (data->map[map_height])
// 		map_height++;
	
// 	y = -1;
// 	while (++y < map_height)
// 	{
// 		x = -1;
// 		while (++x < (int)strlen(data->map[y]))
// 		{
// 			draw_x = offset_x + x * tile_size;
// 			draw_y = offset_y + y * tile_size;
			
// 			color = 0x808080;
// 			if (data->map[y][x] == '1')
// 				color = 0x333333;
// 			else if (data->map[y][x] == '0')
// 				color = 0xCCCCCC;

// 			dy = -1;
// 			while (++dy < tile_size)
// 			{
// 				dx = -1;
// 				while (++dx < tile_size)
// 				{
// 					if (dx == 0 || dy == 0 || dx == tile_size - 1 || dy == tile_size - 1)
// 						my_mlx_pixel_put(img, draw_x + dx, draw_y + dy, 0x000000);
// 					else
// 						my_mlx_pixel_put(img, draw_x + dx, draw_y + dy, color);
// 				}
// 			}
// 		}
// 	}

// 	draw_player_on_map(data, img, tile_size, offset_x, offset_y);
// }

// +++++++++++++++++++++

void	raycast(t_cub *data, t_data *img)
{
	int			x;
	double		ray_dir_x;
	double		ray_dir_y;
	t_dda		dda;
	t_render	render;

	x = 0;
	while (x < SCREEN_WIDTH)
	{
		calc_ray_pos_dir(&data->var, x, &ray_dir_x, &ray_dir_y);
		set_dda_params(&dda, &data->var, ray_dir_x, ray_dir_y);
		perform_dda(&dda, data->map);
		calc_wall_params(&dda, &data->var, ray_dir_x, ray_dir_y, &render);
		calc_texture_coords(&dda, &data->var, &render, ray_dir_x, ray_dir_y);
		draw_floor_ceiling(img, x, &render, data);
		draw_textured_line(img, x, &render, data);
		x++;
	}

	draw_2d_map(data, img);
}

// void raycast(t_cub *data, t_data *img)
// {
// 	data = get_cub_data();
//     printf("Debug: Entering raycast function.\n");
//     int x = 0;
//     while (x < SCREEN_WIDTH)
//     {
//         printf("Debug: Processing column %d\n", x);
//         double ray_dir_x, ray_dir_y;
//         t_dda dda;
//         t_render render;

//         calc_ray_pos_dir(&data->var, x, &ray_dir_x, &ray_dir_y);
//         set_dda_params(&dda, &data->var, ray_dir_x, ray_dir_y);
//         perform_dda(&dda, data->map);
//         calc_wall_params(&dda, &data->var, ray_dir_x, ray_dir_y, &render);
//         calc_texture_coords(&dda, &data->var, &render, ray_dir_x, ray_dir_y);
        
//         draw_floor_ceiling(img, x, &render, data);
//         draw_textured_line(img, x, &render, data);

//         x++;
//     }
//     printf("Debug: Finished raycast function.\n");
// }
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:09:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/07 01:57:15 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Function to draw a filled rectangle
void	draw_rectangle(t_data *img, int x, int y, int width, int height,
		int color)
{
	int	i;
	int	j;

	i = x;
	while (i < x + width)
	{
		j = y;
		while (j < y + height)
		{
			my_mlx_pixel_put(img, i, j, color);
			j++;
		}
		i++;
	}
}

// Function to draw a line between two points
void	draw_line(t_data *img, int x1, int y1, int x2, int y2, int color)
{
	double	dx;
	double	dy;
	double	step;
	double	x;
	double	y;
	int		i;

	dx = x2 - x1;
	dy = y2 - y1;
	step = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx = dx / step;
	dy = dy / step;
	x = x1;
	y = y1;
	i = 0;
	while (i <= step)
	{
		my_mlx_pixel_put(img, (int)x, (int)y, color);
		x += dx;
		y += dy;
		i++;
	}
}

// Function to set initial player direction based on map character
void	set_player_direction(t_cub *data, char dir_char)
{
	if (dir_char == 'N')
	{
		data->var.direction_x = 0;
		data->var.direction_y = -1;
		data->var.plane_x = 0.66;
		data->var.plane_y = 0;
	}
	else if (dir_char == 'S')
	{
		data->var.direction_x = 0;
		data->var.direction_y = 1;
		data->var.plane_x = -0.66;
		data->var.plane_y = 0;
	}
	else if (dir_char == 'E')
	{
		data->var.direction_x = 1;
		data->var.direction_y = 0;
		data->var.plane_x = 0;
		data->var.plane_y = 0.66;
	}
	else if (dir_char == 'W')
	{
		data->var.direction_x = -1;
		data->var.direction_y = 0;
		data->var.plane_x = 0;
		data->var.plane_y = -0.66;
	}
}

char	*ft_strchr(const char *s, int c)
{
	while ((*s != '\0') && (*s != c))
	{
		s++;
	}
	if (*s == c)
	{
		return ((char *)s);
	}
	return ((char *)NULL);
}

// // Function for raycasting rendering
// void clear_screen(t_data *img, t_cub *data, int window_width,
	// int window_height)
// {
//     draw_rectangle(img, 0, 0, window_width, window_height / 2,
	// data->ceilling);
//     draw_rectangle(img, 0, window_height / 2, window_width, window_height
	// / 2, data->floor);
// }

// void   initialize_ray(int x, int window_width, t_cub *data, t_dda *dda,
	// double *ray_dir_x, double *ray_dir_y)
// {
//     double camera_x = 2 * x / (double)window_width - 1;
//     *ray_dir_x = data->var.direction_x + data->var.plane_x * camera_x;
//     *ray_dir_y = data->var.direction_y + data->var.plane_y * camera_x;

//     dda->map_x = (int)data->var.position_x;
//     dda->map_y = (int)data->var.position_y;
//     dda->delta_dist_x = (*ray_dir_x == 0) ? 1e30 : fabs(1 / *ray_dir_x);
//     dda->delta_dist_y = (*ray_dir_y == 0) ? 1e30 : fabs(1 / *ray_dir_y);
// }

// void calculate_step_and_side_distance(t_cub *data, t_dda *dda,
	// double ray_dir_x, double ray_dir_y)
// {
//     if (ray_dir_x < 0)
//     {
//         dda->step_x = -1;
//         dda->side_dist_x = (data->var.position_x - dda->map_x)
	// * dda->delta_dist_x;
//     }
//     else
//     {
//         dda->step_x = 1;
//         dda->side_dist_x = (dda->map_x + 1.0 - data->var.position_x)
	// * dda->delta_dist_x;
//     }

//     if (ray_dir_y < 0)
//     {
//         dda->step_y = -1;
//         dda->side_dist_y = (data->var.position_y - dda->map_y)
	// * dda->delta_dist_y;
//     }
//     else
//     {
//         dda->step_y = 1;
//         dda->side_dist_y = (dda->map_y + 1.0 - data->var.position_y)
	// * dda->delta_dist_y;
//     }
// }

// void do_dda(t_cub *data, t_dda *dda)
// {
//     dda->hit = 0;
//     while (dda->hit == 0)
//     {
//         if (dda->side_dist_x < dda->side_dist_y)
//         {
//             dda->side_dist_x += dda->delta_dist_x;
//             dda->map_x += dda->step_x;
//             dda->side = 0;
//         }
//         else
//         {
//             dda->side_dist_y += dda->delta_dist_y;
//             dda->map_y += dda->step_y;
//             dda->side = 1;
//         }

//         if (dda->map_y >= 0 && dda->map_y < ft_array_length(data->map) &&
//             dda->map_x >= 0
	// && dda->map_x < (int)strlen(data->map[dda->map_y]) &&
//             data->map[dda->map_y][dda->map_x] == '1')
//             dda->hit = 1;
//     }
// }

// double calculate_wall_distance(t_cub *data, t_dda *dda, double ray_dir_x,
	// double ray_dir_y)
// {
//     if (dda->side == 0)
//         return (dda->map_x - data->var.position_x + (1 - dda->step_x) / 2)
	// / fabs(ray_dir_x);
//     else
//         return (dda->map_y - data->var.position_y + (1 - dda->step_y) / 2)
	// / fabs(ray_dir_y);
// }

// void calculate_wall_height(t_render *render, int window_height,
	// double perp_wall_dist)
// {
//     render->perp_wall_dist = perp_wall_dist < 0.0001 ? 0.0001 : perp_wall_dist;
//     render->line_height = (int)(window_height / render->perp_wall_dist);
//     render->draw_start = -render->line_height / 2 + window_height / 2;
//     render->draw_start = (render->draw_start < 0) ? 0 : render->draw_start;
//     render->draw_end = render->line_height / 2 + window_height / 2;
//     render->draw_end = (render->draw_end >= window_height) ? window_height
	// - 1 : render->draw_end;
// }

// void draw_wall_column(t_data *img, int x, t_render *render, int wall_color)
// {
//     int y = render->draw_start;
//     while (y < render->draw_end)
//     {
//         my_mlx_pixel_put(img, x, y, wall_color);
//         y++;
//     }
// }

// void render_raycasting(t_data *img, t_cub *data)
// {
//     int x;
//     double ray_dir_x, ray_dir_y;
//     t_dda dda;
//     t_render render;
//     int window_width = ft_max_length(data->map) * CELL_SIZE;
//     int window_height = ft_array_length(data->map) * CELL_SIZE;

//     // Clear the screen - draw ceiling and floor
//     clear_screen(img, data, window_width, window_height);

//     x = 0;
//     while (x < window_width)
//     {
//         // Step 1: Initialize the ray
//         initialize_ray(x, window_width, data, &dda, &ray_dir_x, &ray_dir_y);

//         // Step 2: Determine step direction and side distances
//         calculate_step_and_side_distance(data, &dda, ray_dir_x, ray_dir_y);

//         // Step 3: Perform DDA to detect wall hit
//         do_dda(data, &dda);

//         // Step 4: Calculate wall distance
//         double perp_wall_dist = calculate_wall_distance(data, &dda,
		// ray_dir_x, ray_dir_y);

//         // Step 5: Calculate wall height on screen
//         calculate_wall_height(&render, window_height, perp_wall_dist);

//         // Step 6: Determine wall color based on side hit
//         int wall_color = (dda.side == 0) ? RED : DARK_RED;
	// Red or Darker Red

//         // Step 7: Draw the wall column
//         draw_wall_column(img, x, &render, wall_color);

//         x++;
//     }
// }

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

int	render_frame(void *param)
{
	t_game_state	*game;
	t_data			img;

	game = (t_game_state *)param;
	img.img = mlx_new_image(game->data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!img.img)
		error_exit_cleanup(ERR_CREATE_IMAGE);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
			&img.endian);
	if (!img.addr)
	{
		mlx_destroy_image(game->data->mlx, img.img);
		error_exit_cleanup(ERR_GET_IMAGE_ADDR);
	}
	// render_raycasting(&img, game->data);
	raycast(game->data, &img);
	mlx_put_image_to_window(game->data->mlx, game->data->win, img.img, 0, 0);
	mlx_destroy_image(game->data->mlx, img.img);
	return (0);
}

void	init(void)
{
	t_cub			*data;
	t_game_state	*g;
	int				win_height;
	int				win_width;

	data = get_cub_data();
	// int win_height = ft_array_length(data->map) * CELL_SIZE;
	win_height = SCREEN_HEIGHT;
	// int win_width = ft_max_length(data->map) * CELL_SIZE;
	win_width = SCREEN_WIDTH;
	g = malloc(sizeof(t_game_state));
	if (!g)
		error_exit_cleanup(ERR_MEMORY_ALLOCATION);
	g->data = data;
	g->img = malloc(sizeof(t_data));
	if (!g->img)
	{
		free(g);
		error_exit_cleanup(ERR_MEMORY_ALLOCATION);
	}
	init_window(data, g->img, win_width, win_height);
	if (init_textures_and_colors(data))
	{
		free(g->img);
		free(g);
		return ;
	}
	// render_raycasting(g->img, data);
	mlx_put_image_to_window(data->mlx, data->win, g->img->img, 0, 0);
	mlx_hook(data->win, 2, 1L << 0, handle_keypress, g);
	mlx_hook(data->win, 17, 0L, on_destroy, data);
	mlx_loop_hook(data->mlx, render_frame, g);
	mlx_loop(data->mlx);
	free(g->img);
	free(g);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:09:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/07 02:48:13 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Function to draw a filled rectangle
// void	draw_rectangle(t_data *img, int x, int y, int width, int height,
// 		int color)
// {
// 	int	i;
// 	int	j;

// 	i = x;
// 	while (i < x + width)
// 	{
// 		j = y;
// 		while (j < y + height)
// 		{
// 			my_mlx_pixel_put(img, i, j, color);
// 			j++;
// 		}
// 		i++;
// 	}
// }

// Function to draw a line between two points
// void	draw_line(t_data *img, int x1, int y1, int x2, int y2, int color)
// {
// 	double	dx;
// 	double	dy;
// 	double	step;
// 	double	x;
// 	double	y;
// 	int		i;

// 	dx = x2 - x1;
// 	dy = y2 - y1;
// 	step = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
// 	dx = dx / step;
// 	dy = dy / step;
// 	x = x1;
// 	y = y1;
// 	i = 0;
// 	while (i <= step)
// 	{
// 		my_mlx_pixel_put(img, (int)x, (int)y, color);
// 		x += dx;
// 		y += dy;
// 		i++;
// 	}
// }

// Function to set initial player direction based on map character
// void	set_player_direction(t_cub *data, char dir_char)
// {
// 	if (dir_char == 'N')
// 	{
// 		data->var.direction_x = 0;
// 		data->var.direction_y = -1;
// 		data->var.plane_x = 0.66;
// 		data->var.plane_y = 0;
// 	}
// 	else if (dir_char == 'S')
// 	{
// 		data->var.direction_x = 0;
// 		data->var.direction_y = 1;
// 		data->var.plane_x = -0.66;
// 		data->var.plane_y = 0;
// 	}
// 	else if (dir_char == 'E')
// 	{
// 		data->var.direction_x = 1;
// 		data->var.direction_y = 0;
// 		data->var.plane_x = 0;
// 		data->var.plane_y = 0.66;
// 	}
// 	else if (dir_char == 'W')
// 	{
// 		data->var.direction_x = -1;
// 		data->var.direction_y = 0;
// 		data->var.plane_x = 0;
// 		data->var.plane_y = -0.66;
// 	}
// }

char	*ft_strchr(const char *s, int c)
{
	while ((*s != '\0') && (*s != c))
		s++;
	if (*s == c)
		return ((char *)s);
	return ((char *) NULL);
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
	win_height = SCREEN_HEIGHT;
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
	mlx_put_image_to_window(data->mlx, data->win, g->img->img, 0, 0);
	mlx_hook(data->win, 2, 1L << 0, handle_keypress, g);
	mlx_hook(data->win, 17, 0L, on_destroy, data);
	mlx_loop_hook(data->mlx, render_frame, g);
	mlx_loop(data->mlx);
	free(g->img);
	free(g);
}

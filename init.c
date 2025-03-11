/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:48:05 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 06:08:43 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_camera_plane(double direction_x, double direction_y, double pla_x, double pla_y)
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

void init_enemy(t_enemy *enemy, double pos_x, double pos_y)
{
    enemy->pos_x = pos_x;
    enemy->pos_y = pos_y;
    enemy->dir_x = 1.0;
    enemy->dir_y = 0.0;
    enemy->move_speed = 0.005;
    enemy->state = 0;
    enemy->detection_radius = 3.0;
    enemy->move_timer = 0;
}

void init_input_state(t_game_state *g)
{
    g->keys.w_pressed = 0;
    g->keys.a_pressed = 0;
    g->keys.s_pressed = 0;
    g->keys.d_pressed = 0;
    g->keys.left_pressed = 0;
    g->keys.right_pressed = 0;
    g->prev_mouse_x = SCREEN_WIDTH / 2;
}

void init_window_and_hooks(t_cub *data, t_game_state *g)
{
    mlx_put_image_to_window(data->mlx, data->win, g->img->img, 0, 0);
    
    mlx_hook(data->win, 2, 1L << 0, handle_keypress, g);
    mlx_hook(data->win, 3, 1L << 1, handle_keyrelease, g);
    mlx_hook(data->win, 6, 1L << 6, handle_mouse_move, g);
    mlx_hook(data->win, 17, 0L, on_destroy, data);
    mlx_mouse_hide(data->mlx, data->win);
    mlx_mouse_move(data->mlx, data->win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    mlx_loop_hook(data->mlx, render_frame, g);
}

t_game_state *init_game_state(t_cub *data)
{
    t_game_state *g;
    
    g = malloc(sizeof(t_game_state));
    if (!g)
        error_exit_cleanup(ERR_MEMORY_ALLOCATION);    
    init_input_state(g);
    g->data = data;
    g->img = malloc(sizeof(t_data));
    if (!g->img)
    {
        free(g);
        error_exit_cleanup(ERR_MEMORY_ALLOCATION);
    }
    g->rand_state = 6294;
    init_window(data, g->img, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (init_textures_and_colors(data))
        return (free(g->img), free(g), NULL);
    if (initialize_enemies(g))
        return (free(g->img), free(g), NULL);
    return g;
}

void init(void)
{
    t_cub *data;
    t_game_state *g;

    data = get_cub_data();
    g = init_game_state(data);
    if (!g)
        return;
    init_window_and_hooks(data, g);
    mlx_loop(data->mlx);
    free(g->img);
    free(g);
}
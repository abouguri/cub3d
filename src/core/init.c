/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:48:05 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/14 03:55:58 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_enemy(t_enemy *enemy, double pos_x, double pos_y)
{
	enemy->pos_x = pos_x;
	enemy->pos_y = pos_y;
	enemy->dir_x = 1.0;
	enemy->dir_y = 0.0;
	enemy->move_speed = 0.01;
	enemy->state = 0;
	enemy->detection_radius = 3.0;
	enemy->move_timer = 0;
}

void	init_input_state(t_game_state *g)
{
	g->keys.w_pressed = 0;
	g->keys.a_pressed = 0;
	g->keys.s_pressed = 0;
	g->keys.d_pressed = 0;
	g->keys.left_pressed = 0;
	g->keys.right_pressed = 0;
	g->prev_mouse_x = SCREEN_WIDTH / 2;
}

void	init_window_and_hooks(t_cub *data, t_game_state *g)
{
	mlx_put_image_to_window(data->mlx, data->win, g->img->img, 0, 0);
	mlx_hook(data->win, 2, 1L << 0, handle_keypress, g);
	mlx_hook(data->win, 3, 1L << 1, handle_keyrelease, g);
	mlx_hook(data->win, 6, 1L << 6, handle_mouse_move, g);
	mlx_hook(data->win, 17, 0L, on_destroy, g);
	mlx_mouse_hide(data->mlx, data->win);
	mlx_mouse_move(data->mlx, data->win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	mlx_loop_hook(data->mlx, render_frame, g);
}

t_game_state	*init_game_state(t_cub *data)
{
	t_game_state	*g;

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
	return (g);
}

void	init(void)
{
	t_cub			*data;
	t_game_state	*g;

	data = get_cub_data();
	g = init_game_state(data);
	if (!g)
		return ;
	init_window_and_hooks(data, g);
	mlx_loop(data->mlx);
	cleanup_resources(g);
}

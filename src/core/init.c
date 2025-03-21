/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:48:05 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/21 00:48:36 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

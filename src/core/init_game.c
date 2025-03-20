/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 01:00:20 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/20 04:26:08 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_player_state(t_game_state *g, t_cub *data)
{
	g->player.health = 100;
	g->player.max_health = 100;
	g->player.damage_cooldown = 0;
	g->player.is_damaged = 0;
	g->initial_pos_x = data->var.position_x;
	g->initial_pos_y = data->var.position_y;
}

static int	init_game_resources(t_game_state *g, t_cub *data)
{
	init_window(data, g->img, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (init_textures_and_colors(data))
	{
		cleanup_game_image(g, data);
		free(g);
		free_array(&data->textures);
		free_array(&data->colors);
		free_array(&data->map);
		cleanup_texture_images(data);
		cleanup_mlx(data);
		error_exit_cleanup("");
		return (1);
	}
	return (0);
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
	init_player_state(g, data);
	g->game_status = GAME_ACTIVE;
	g->game_over_timer = 0;
	if (init_game_resources(g, data))
		return (free(g->img), free(g), NULL);
	if (initialize_enemies(g))
		return (free(g->img), free(g), NULL);
	return (g);
}

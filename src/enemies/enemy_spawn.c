/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_spawn.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 05:38:14 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 05:41:01 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	load_enemy_texture(t_game_state *game, char *path)
{
	t_img2	img;

	if (load_image(game->data, &img, path))
		return (1);
	resize_texture(game->data->texture[4], &img, 0, 0);
	mlx_destroy_image(game->data->mlx, img.img_ptr);
	return (0);
}

int	place_enemies(t_game_state *game, t_position *valid_positions,
		int valid_count)
{
	int		i;
	int		random_index;
	double	pos_x;
	double	pos_y;

	game->enemy_manager.enemies = malloc(sizeof(t_enemy) * ENEMIES_COUNT);
	if (!game->enemy_manager.enemies)
		return (1);
	i = 0;
	while (i < ENEMIES_COUNT)
	{
		random_index = custom_rand(game) % valid_count;
		pos_x = valid_positions[random_index].x + 0.5;
		pos_y = valid_positions[random_index].y + 0.5;
		init_enemy(&game->enemy_manager.enemies[i], pos_x, pos_y);
		valid_positions[random_index] = valid_positions[valid_count - 1];
		valid_count--;
		i++;
	}
	game->enemy_manager.enemy_count = ENEMIES_COUNT;
	return (0);
}

void	place_enemies_randomly(t_game_state *game)
{
	int			valid_positions_count;
	t_position	*valid_positions;

	int map_height, map_width;
	valid_positions_count = 0;
	get_map_dimensions(game->data, &map_width, &map_height);
	if (map_height <= 0)
		return ;
	valid_positions = collect_valid_positions(game->data, map_width, map_height,
			&valid_positions_count);
	if (!valid_positions || valid_positions_count == 0)
	{
		free(valid_positions);
		return ;
	}
	custom_srand(game, map_width * 1000 + map_height);
	if (place_enemies(game, valid_positions, valid_positions_count) != 0)
	{
		free(valid_positions);
		return ;
	}
	free(valid_positions);
}

int	initialize_enemies(t_game_state *game)
{
	game->enemy_manager.enemies = NULL;
	game->enemy_manager.enemy_count = ENEMIES_COUNT;
	if (load_enemy_texture(game, ENEMY_PATH))
		return (1);
	place_enemies_randomly(game);
	return (0);
}

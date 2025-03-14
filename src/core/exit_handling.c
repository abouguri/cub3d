/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:59:15 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/14 03:52:59 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	cleanup_enemies(t_game_state *game)
{
	if (game->enemy_manager.enemies)
	{
		free(game->enemy_manager.enemies);
		game->enemy_manager.enemies = NULL;
	}
}

void	cleanup_game_image(t_game_state *game, t_cub *cub_data)
{
	if (game->img)
	{
		if (game->img->img)
		{
			mlx_destroy_image(cub_data->mlx, game->img->img);
			game->img->img = NULL;
		}
		free(game->img);
		game->img = NULL;
	}
}

void	cleanup_texture_images(t_cub *cub_data)
{
	int	i;

	i = 0;
	while (i < 5)
	{
		if (cub_data->img2[i])
		{
			if (cub_data->img2[i]->img_ptr)
			{
				mlx_destroy_image(cub_data->mlx, cub_data->img2[i]->img_ptr);
				cub_data->img2[i]->img_ptr = NULL;
			}
			free(cub_data->img2[i]);
			cub_data->img2[i] = NULL;
		}
		i++;
	}
}

void	cleanup_mlx(t_cub *cub_data)
{
	if (cub_data->mlx)
	{
		if (cub_data->win)
		{
			mlx_destroy_window(cub_data->mlx, cub_data->win);
			cub_data->win = NULL;
		}
		mlx_destroy_display(cub_data->mlx);
		free(cub_data->mlx);
		cub_data->mlx = NULL;
	}
}

void	cleanup_resources(t_game_state *game)
{
	t_cub	*cub_data;

	cub_data = game->data;
	cleanup_enemies(game);
	cleanup_game_image(game, cub_data);
	free_array(&cub_data->textures);
	free_array(&cub_data->colors);
	free_array(&cub_data->map);
	cleanup_texture_images(cub_data);
	cleanup_mlx(cub_data);
	free(game);
}

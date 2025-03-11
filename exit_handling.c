/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:59:15 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 00:00:15 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	exit_game(t_game_state *game)
{
	t_cub	*data;
	data = game->data;

	// data = (t_cub *)data;
	// End the MLX loop first
	mlx_loop_end(data->mlx);
	// Destroy window
	if (data->win)
		mlx_destroy_window(data->mlx, data->win);
	// Destroy display (Linux only)
	if (data->mlx)
	{
		// mlx_destroy_image(data->mlx, data->img2);
		mlx_destroy_display(data->mlx);
		free(data->mlx);
	}
	exit(0);
	return (0);
}

int	on_destroy(void *data)
{
	t_cub	*cub_data;

	cub_data = (t_cub *)data;
	// End the MLX loop first
	mlx_loop_end(cub_data->mlx);
	// Destroy window
	if (cub_data->win)
		mlx_destroy_window(cub_data->mlx, cub_data->win);
	// Destroy display (Linux only)
	if (cub_data->mlx)
	{
		mlx_destroy_display(cub_data->mlx);
		free(cub_data->mlx);
	}
	exit(0);
	return (0);
}

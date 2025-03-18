/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 05:37:04 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/18 00:34:59 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_enemies(t_game_state *game)
{
	int		i;
	t_enemy	*enemy;

	if (game->game_status != GAME_ACTIVE)
		return ;
	i = 0;
	while (i < game->enemy_manager.enemy_count)
	{
		enemy = &game->enemy_manager.enemies[i];
		if (enemy->state == 0)
		{
			if (is_player_detected(enemy, game->data))
				enemy->state = 1;
			else
				update_enemy_random_movement(game, enemy);
		}
		else
		{
			update_enemy_following(game, enemy);
			check_enemy_damage(game, enemy);
		}
		i++;
	}
}

static void	swap_enemies_if_needed(t_game_state *game, t_cub *data, int j)
{
	double	dist1;
	double	dist2;
	t_enemy	temp;

	dist1 = pow(data->var.position_x - game->enemy_manager.enemies[j].pos_x, 2)
		+ pow(data->var.position_y - game->enemy_manager.enemies[j].pos_y, 2);
	dist2 = pow(data->var.position_x - game->enemy_manager.enemies[j + 1].pos_x,
			2) + pow(data->var.position_y - game->enemy_manager.enemies[j
			+ 1].pos_y, 2);
	if (dist1 < dist2)
	{
		temp = game->enemy_manager.enemies[j];
		game->enemy_manager.enemies[j] = game->enemy_manager.enemies[j + 1];
		game->enemy_manager.enemies[j + 1] = temp;
	}
}

void	sort_enemies_by_distance(t_game_state *game)
{
	t_index	i;
	t_cub	*data;

	i.i = 0;
	data = game->data;
	while (i.i < game->enemy_manager.enemy_count - 1)
	{
		i.j = 0;
		while (i.j < game->enemy_manager.enemy_count - i.i - 1)
		{
			swap_enemies_if_needed(game, data, i.j);
			i.j++;
		}
		i.i++;
	}
}

void	check_enemy_damage(t_game_state *game, t_enemy *enemy)
{
	double	dx;
	double	dy;
	double	distance;

	dx = game->data->var.position_x - enemy->pos_x;
	dy = game->data->var.position_y - enemy->pos_y;
	distance = sqrt(dx * dx + dy * dy);
	if (game->player.damage_cooldown <= 0 && game->game_status == 0)
	{
		if (distance <= 0.5)
		{
			game->player.health -= 10;
			game->player.is_damaged = 1;
			game->player.damage_cooldown = 60;
			if (game->player.health <= 0)
			{
				game->player.health = 0;
				game->game_status = GAME_OVER;
				game->game_over_timer = 180;
			}
		}
	}
	else if (game->player.damage_cooldown > 0)
	{
		game->player.damage_cooldown--;
		if (game->player.damage_cooldown == 0)
			game->player.is_damaged = 0;
	}
}

void	draw_rectangle(t_game_state *game, int x, int y, int width, int height,
		int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < width)
	{
		j = 0;
		while (j < height)
		{
			my_mlx_pixel_put(game->img, x + i, y + j, color);
			j++;
		}
		i++;
	}
}

int	get_health_bar_color(int health, int max_health)
{
	int	health_percent;

	health_percent = (health * 100) / max_health;
	if (health_percent > 70)
		return (GREEN);
	else if (health_percent > 30)
		return (YELLOW);
	else
		return (RED);
}

void	draw_health_bar(t_game_state *game)
{
	t_health_bar	bar;

	bar.width = 200;
	bar.height = 20;
	bar.x = SCREEN_WIDTH / 2 - bar.width / 2;
	bar.y = SCREEN_HEIGHT - 40;
	bar.fill_width = (game->player.health * bar.width)
		/ game->player.max_health;
	bar.bg_color = DARK_RED;
	bar.fill_color = get_health_bar_color(game->player.health,
			game->player.max_health);
	draw_rectangle(game, bar.x, bar.y, bar.width, bar.height, bar.bg_color);
	draw_rectangle(game, bar.x, bar.y, bar.fill_width, bar.height,
		bar.fill_color);
}

void	apply_damage_effect(t_game_state *game)
{
	int	i;
	int	j;
	int	color;

	if (game->player.is_damaged)
	{
		i = 0;
		while (i < SCREEN_WIDTH)
		{
			j = 0;
			while (j < SCREEN_HEIGHT)
			{
				color = ((unsigned char *)game->img->addr)[j
					* game->img->line_length + i * (game->img->bits_per_pixel
						/ 8)];
				color = ((color & 0xFF0000)) | ((color & 0x00FF00)) | ((color & 0x0000FF));
				my_mlx_pixel_put(game->img, i, j, color);
				j++;
			}
			i++;
		}
	}
}

void	draw_game_over(t_game_state *game)
{
	int	i;
	int	j;
	int	current_color;
	int	darkened;
	int	alpha;

	alpha = 255;
	if (game->game_over_timer > 120)
		alpha = 255 - (game->game_over_timer - 120) * 4;
	if (alpha < 0)
		alpha = 0;
	i = 0;
	while (i < SCREEN_WIDTH)
	{
		j = 0;
		while (j < SCREEN_HEIGHT)
		{
			current_color = *(int *)(game->img->addr + (j
						* game->img->line_length + i
						* (game->img->bits_per_pixel / 8)));
			darkened = (current_color & ALPHA_MASK) | ((current_color & RED_MASK) >> 2) | ((current_color & GREEN_MASK) >> 2) | ((current_color & BLUE_MASK) >> 2);
			my_mlx_pixel_put(game->img, i, j, darkened);
		}
	}
	game->render_game_over_text = 1;
}

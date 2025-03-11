/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 05:37:04 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 05:37:38 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void update_enemies(t_game_state *game)
{
    int i;
    i = 0;
    while (i < game->enemy_manager.enemy_count)
    {
        t_enemy *enemy = &game->enemy_manager.enemies[i];
        if (enemy->state == 0)
        {
            if (is_player_detected(enemy, game->data))
                enemy->state = 1;
            else
                update_enemy_random_movement(game, enemy);
        }
        else
            update_enemy_following(game, enemy);
        i++;
    }
}

void sort_enemies_by_distance(t_game_state *game)
{
    int i = 0, j;
    t_cub *data = game->data;
    while (i < game->enemy_manager.enemy_count - 1)
    {
        j = 0;
        while (j < game->enemy_manager.enemy_count - i - 1)
        {
            double dist1 = pow(data->var.position_x - game->enemy_manager.enemies[j].pos_x, 2) +
                           pow(data->var.position_y - game->enemy_manager.enemies[j].pos_y, 2);
            double dist2 = pow(data->var.position_x - game->enemy_manager.enemies[j + 1].pos_x, 2) +
                           pow(data->var.position_y - game->enemy_manager.enemies[j + 1].pos_y, 2);
            if (dist1 < dist2)
            {
                t_enemy temp = game->enemy_manager.enemies[j];
                game->enemy_manager.enemies[j] = game->enemy_manager.enemies[j + 1];
                game->enemy_manager.enemies[j + 1] = temp;
            }
            j++;
        }
        i++;
    }
}

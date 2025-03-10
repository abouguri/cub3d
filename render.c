/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:09:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/10 01:46:34 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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


// Initialize a single enemy
void init_enemy(t_enemy *enemy, double pos_x, double pos_y)
{
    enemy->pos_x = pos_x;
    enemy->pos_y = pos_y;
    enemy->dir_x = 1.0;  // Default direction
    enemy->dir_y = 0.0;
    enemy->move_speed = 0.005;
    enemy->state = 0;  // Start with random movement
    enemy->detection_radius = 3.0;  // Detect player within 3 units
    enemy->move_timer = 0;
}

int can_enemy_move(t_game_state *game, double new_x, double new_y)
{
    int map_x = (int)new_x;
    int map_y = (int)new_y;
    if (map_y >= 0 && map_x >= 0 && game->data->map[map_y] && game->data->map[map_y][map_x] == '1')
        return (0);
    return (1);
}

void update_enemy_random_movement(t_game_state *game, t_enemy *enemy)
{
    double new_x;
    double new_y;
    
    enemy->move_timer++;
    if (enemy->move_timer >= 100)
    {
        enemy->move_timer = 0;
        // Generate random angle between 0 and 2π
        double angle = (custom_rand(game) % 628) / 100.0;  // 0 to 6.28 (2π)
        
        // Set new direction
        enemy->dir_x = cos(angle);
        enemy->dir_y = sin(angle);
    }
    
    new_x = enemy->pos_x + enemy->dir_x * enemy->move_speed;
    new_y = enemy->pos_y + enemy->dir_y * enemy->move_speed;
    
    if (can_enemy_move(game, new_x, new_y))
    {
        enemy->pos_x = new_x;
        enemy->pos_y = new_y;
    }
    else
    {
        enemy->dir_x = -enemy->dir_x;
        enemy->dir_y = -enemy->dir_y;
        enemy->move_timer = 90;  // Nearly time for a new random direction
    }
}

// Check if player is within detection radius
int is_player_detected(t_enemy *enemy, t_cub *data)
{
    double dx = data->var.position_x - enemy->pos_x;
    double dy = data->var.position_y - enemy->pos_y;
    double distance = sqrt(dx * dx + dy * dy);
    
    return (distance <= enemy->detection_radius);
}

// Update enemy following behavior
void update_enemy_following(t_game_state *game, t_enemy *enemy)
{
    t_cub *data = game->data;
    
    // Calculate direction vector toward player
    double dx = data->var.position_x - enemy->pos_x;
    double dy = data->var.position_y - enemy->pos_y;
    double length = sqrt(dx * dx + dy * dy);
    
    // Normalize direction vector
    if (length > 0)
    {
        dx /= length;
        dy /= length;
    }
    
    // Set direction
    enemy->dir_x = dx;
    enemy->dir_y = dy;
    
    // Calculate new position
    double new_x = enemy->pos_x + enemy->dir_x * enemy->move_speed;
    double new_y = enemy->pos_y + enemy->dir_y * enemy->move_speed;
    
    // Check if the enemy can move to the new position
    if (can_enemy_move(game, new_x, new_y))
    {
        enemy->pos_x = new_x;
        enemy->pos_y = new_y;
    }
    
    // Check if player is still in range
    if (!is_player_detected(enemy, data))
    {
        enemy->state = 0;  // Return to random movement
    }
}

// Update all enemies
void update_enemies(t_game_state *game)
{
    int i;
    for (i = 0; i < game->enemy_manager.enemy_count; i++)
    {
        t_enemy *enemy = &game->enemy_manager.enemies[i];
        
        // printf("+++++++++++\n");
        if (enemy->state == 0)
        { // Random movement state
            // Check if player is detected
            if (is_player_detected(enemy, game->data))
            {
                enemy->state = 1;  // Switch to following state
            }
            else
            {
                update_enemy_random_movement(game, enemy);
            }
        }
        else
        {  // Following state
            update_enemy_following(game, enemy);
        }
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

void calculate_enemy_transform(t_render_context *ctx)
{
    t_cub *data = ctx->game->data;
    double sprite_x = ctx->enemy->pos_x - data->var.position_x;
    double sprite_y = ctx->enemy->pos_y - data->var.position_y;
    double inv_det = 1.0 / (data->var.plane_x * data->var.direction_y - data->var.direction_x * data->var.plane_y);
    ctx->transform.transform_x = inv_det * (data->var.direction_y * sprite_x - data->var.direction_x * sprite_y);
    ctx->transform.transform_y = inv_det * (-data->var.plane_y * sprite_x + data->var.plane_x * sprite_y);
}

void setup_render_context(t_render_context *ctx)
{
    calculate_enemy_transform(ctx);
    if (ctx->transform.transform_y <= 0) return;
    
    ctx->transform.sprite_screen_x = (int)((SCREEN_WIDTH / 2) * (1 + ctx->transform.transform_x / ctx->transform.transform_y));
    ctx->transform.sprite_height = abs((int)(SCREEN_HEIGHT / ctx->transform.transform_y));
    ctx->transform.draw_start_y = fmax(0, -ctx->transform.sprite_height / 2 + SCREEN_HEIGHT / 2);
    ctx->transform.draw_end_y = fmin(SCREEN_HEIGHT - 1, ctx->transform.sprite_height / 2 + SCREEN_HEIGHT / 2);
    ctx->transform.sprite_width = abs((int)(SCREEN_HEIGHT / ctx->transform.transform_y));
    ctx->transform.draw_start_x = fmax(0, -ctx->transform.sprite_width / 2 + ctx->transform.sprite_screen_x);
    ctx->transform.draw_end_x = fmin(SCREEN_WIDTH - 1, ctx->transform.sprite_width / 2 + ctx->transform.sprite_screen_x);
}

void render_enemy(t_render_context *ctx)
{
    if (ctx->transform.transform_y <= 0) return;
    int stripe = ctx->transform.draw_start_x;
    while (stripe < ctx->transform.draw_end_x)
    {
        if (ctx->transform.transform_y > 0 && stripe > 0 && stripe < SCREEN_WIDTH && ctx->transform.transform_y < ctx->z_buffer[stripe])
        {
            int tex_x = (int)(256 * (stripe - (-ctx->transform.sprite_width / 2 + ctx->transform.sprite_screen_x)) * TEXTURE_WIDTH / ctx->transform.sprite_width) / 256;
            int y = ctx->transform.draw_start_y;
            while (y < ctx->transform.draw_end_y)
            {
                int d = y * 256 - SCREEN_HEIGHT * 128 + ctx->transform.sprite_height * 128;
                int tex_y = ((d * TEXTURE_HEIGHT) / ctx->transform.sprite_height) / 256;
                int color = ctx->game->data->texture[4][TEXTURE_WIDTH * tex_y + tex_x];
                if ((color & 0x00FFFFFF) != 0)
                {
                    my_mlx_pixel_put(ctx->game->img, stripe, y, color);
                }
                y++;
            }
        }
        stripe++;
    }
}

void render_enemies(t_game_state *game, double *z_buffer)
{
    sort_enemies_by_distance(game);
    int i = 0;
    while (i < game->enemy_manager.enemy_count)
    {
        t_transform transform = {0};
        t_render_context ctx = {game, &game->enemy_manager.enemies[i], z_buffer, transform};
        setup_render_context(&ctx);
        render_enemy(&ctx);
        i++;
    }
}


// Load enemy texture
int load_enemy_texture(t_game_state *game, char *path)
{
    t_img2 img;
    
    if (load_image(game->data, &img, path))
        return (1);
    resize_texture(game->data->texture[4], &img, 0, 0);
    mlx_destroy_image(game->data->mlx, img.img_ptr);
    return (0);
}

unsigned int custom_rand(t_game_state *g) 
{
    g->rand_state = g->rand_state * 1103515245 + 12345;
    return (g->rand_state / 65836) % 32768;
}

void custom_srand(t_game_state *g, unsigned int seed) 
{
    g->rand_state = seed;
}

// Get the dimensions of the map
void get_map_dimensions(t_cub *data, int *width, int *height)
{
    int i;
    
    *height = ft_array_length(data->map);
    *width = 0;
    i = 0;
    while (i < *height)
    {
        int current_width = 0;
        while (data->map[i][current_width])
            current_width++;
        if (current_width > *width)
            *width = current_width;
        i++;
    }
}

// Count and collect valid positions for enemy placement
// Calculate the length of a map row
int get_map_row_length(char *row)
{
    int len = 0;
    
    while (row[len])
        len++;
    
    return len;
}

// Check if a position is valid for enemy placement
int valid_position(t_cub *data, int x, int y)
{
    int row_len = get_map_row_length(data->map[y]);
    
    return (x < row_len && data->map[y][x] == '0');
}

// Count the number of valid positions on the map
int count_valid_positions(t_cub *data, int map_width, int map_height)
{
    int x, y;
    int valid_count = 0;
    
    y = 1;
    while (y < map_height - 1)
    {
        x = 1;
        while (x < map_width - 1)
        {
            if (valid_position(data, x, y))
                valid_count++;
            x++;
        }
        y++;
    }
    
    return valid_count;
}

// Store valid positions in the preallocated array
void store_valid_positions(t_cub *data, int map_width, int map_height, 
                          t_position *valid_positions)
{
    int x, y;
    int position_index = 0;
    
    y = 1;
    while (y < map_height - 1)
    {
        x = 1;
        while (x < map_width - 1)
        {
            if (valid_position(data, x, y))
            {
                valid_positions[position_index].x = x;
                valid_positions[position_index].y = y;
                position_index++;
            }
            x++;
        }
        y++;
    }
}

// Main function to collect valid positions
t_position *collect_valid_positions(t_cub *data, int map_width, int map_height, int *count)
{
    t_position *valid_positions;
    
    // First pass: count valid positions
    *count = count_valid_positions(data, map_width, map_height);
    
    if (*count <= 0)
        return NULL;
    
    // Allocate memory for valid positions
    valid_positions = malloc(sizeof(t_position) * (*count));
    if (!valid_positions)
        return NULL;
    
    // Second pass: store valid positions
    store_valid_positions(data, map_width, map_height, valid_positions);
    
    return valid_positions;
}

// Initialize and place the enemies
int place_enemies(t_game_state *game, t_position *valid_positions, int valid_count)
{
    int i;
    
    // Allocate memory for enemies
    game->enemy_manager.enemies = malloc(sizeof(t_enemy) * ENEMIES_COUNT);
    if (!game->enemy_manager.enemies)
        return 1; // Error
    
    // Place enemies at random valid positions
    for (i = 0; i < ENEMIES_COUNT; i++) {
        int random_index = custom_rand(game) % valid_count;
        double pos_x = valid_positions[random_index].x + 0.5; // Center in tile
        double pos_y = valid_positions[random_index].y + 0.5; // Center in tile
        
        // Initialize enemy at this position
        init_enemy(&game->enemy_manager.enemies[i], pos_x, pos_y);
        
        // Remove this position from valid positions
        valid_positions[random_index] = valid_positions[valid_count - 1];
        valid_count--;
    }
    
    // Set the number of enemies
    game->enemy_manager.enemy_count = ENEMIES_COUNT;
    
    return 0; // Success
}

// Main function to place enemies randomly
void place_enemies_randomly(t_game_state *game)
{
    int map_height, map_width;
    int valid_positions_count = 0;
    t_position *valid_positions;
    
    // Get map dimensions
    get_map_dimensions(game->data, &map_width, &map_height);
    if (map_height <= 0)
        return; // No map data
    
    // Collect valid positions
    valid_positions = collect_valid_positions(game->data, map_width, map_height, &valid_positions_count);
    if (!valid_positions || valid_positions_count == 0)
    {
        free(valid_positions); // In case it was allocated but count is 0
        return;
    }
    
    // Seed our custom random number generator
    custom_srand(game, map_width * 1000 + map_height);
    
    // Place the enemies
    if (place_enemies(game, valid_positions, valid_positions_count) != 0)
    {
        free(valid_positions);
        return;
    }
    
    // Free the valid positions array
    free(valid_positions);
}

int initialize_enemies(t_game_state *game) {
    // Initialize enemy manager
    game->enemy_manager.enemies = NULL;
    game->enemy_manager.enemy_count = ENEMIES_COUNT;
    
    // Load enemy texture
    if (load_enemy_texture(game, ENEMY_PATH))
        return (1);
    // Create enemies
    place_enemies_randomly(game);
    // init_enemies(game);
    
    return (0);
}

// Handle player movement based on key states
void handle_player_movement(t_game_state *game)
{
    double move_speed = 0.02;
    double rot_speed = 0.02;
    
    if (game->keys.w_pressed)
        move_forward_backward(game->data, move_speed, 1);
    if (game->keys.s_pressed)
        move_forward_backward(game->data, move_speed, -1);
    if (game->keys.a_pressed)
        move_strafe(game->data, move_speed, 1);
    if (game->keys.d_pressed)
        move_strafe(game->data, move_speed, -1);
    if (game->keys.left_pressed)
        rotate_view(game->data, rot_speed, -1);
    if (game->keys.right_pressed)
        rotate_view(game->data, rot_speed, 1);
}

// Create and prepare a new image for rendering
t_data *create_frame_image(t_cub *data)
{
    t_data *img;
    
    img = malloc(sizeof(t_data));
    if (!img)
        return NULL;
    
    img->img = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!img->img)
    {
        free(img);
        error_exit_cleanup(ERR_CREATE_IMAGE);
        return NULL;
    }
    
    img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, 
                                &img->line_length, &img->endian);
    if (!img->addr)
    {
        mlx_destroy_image(data->mlx, img->img);
        free(img);
        error_exit_cleanup(ERR_GET_IMAGE_ADDR);
        return NULL;
    }
    
    return img;
}

// Render the current frame and display it
void render_and_display(t_game_state *game, t_data *frame_img)
{
    t_data *saved_img = game->img;
    
    // Set the temporary image for rendering
    game->img = frame_img;
    
    // Perform raycasting to render the frame
    raycast(game);
    
    // Restore the original image
    game->img = saved_img;
    
    // Display the rendered frame
    mlx_put_image_to_window(game->data->mlx, game->data->win, frame_img->img, 0, 0);
}

// Main render frame function (callback for mlx_loop_hook)
int render_frame(void *param)
{
    t_game_state *game;
    t_data *frame_img;
    
    game = (t_game_state *)param;
    
    // Handle player movement
    handle_player_movement(game);
    
    // Create a new image for this frame
    frame_img = create_frame_image(game->data);
    if (!frame_img)
        return (1);
    
    // Render the scene and display it
    render_and_display(game, frame_img);
    
    // Clean up the temporary image
    mlx_destroy_image(game->data->mlx, frame_img->img);
    free(frame_img);
    
    return (0);
}

// Initialize input state
void init_input_state(t_game_state *g)
{
    g->keys.w_pressed = 0;
    g->keys.a_pressed = 0;
    g->keys.s_pressed = 0;
    g->keys.d_pressed = 0;
    g->keys.left_pressed = 0;
    g->keys.right_pressed = 0;
    g->prev_mouse_x = SCREEN_WIDTH / 2; // Start with mouse centered
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
// Main initialization function
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
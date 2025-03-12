/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:03:44 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 05:21:31 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/* ************************************************************************** */
/*                                  INCLUDES                                  */
/* ************************************************************************** */

# include "libft/libft.h"
# include "mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/* ************************************************************************** */
/*                                 CONSTANTS                                  */
/* ************************************************************************** */

/* Game Settings */
# define TEXTURE_COUNT 4
# define RGB_COUNT 2
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64
# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 720
# define CELL_SIZE 40
# define CAMERA_HEIGHT_OFFSET 0
# define MAX_DIST 10.0

/* Player Direction */
# define NORTH 'N'
# define SOUTH 'S'
# define EAST 'E'
# define WEST 'W'

/* Map Elements */
# define VALID_MAP_SYMBOLS "01NSEW "

/* Enemy Settings */
# define ENEMIES_COUNT 7
# define ENEMY_PATH "./textures/enemy3.xpm"

/* Input Controls - Linux */
# define KEY_ESC 65307
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_LEFT 65361
# define KEY_RIGHT 65363

/* Colors */
# define BLACK 0x000000
# define WHITE 0xFFFFFF
# define RED 0xFF0000
# define DARK_RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF
# define YELLOW 0xFFFF00
# define CYAN 0x00FFFF
# define MAGENTA 0xFF00FF
# define ORANGE 0xFFA500
# define PURPLE 0x400040
# define BROWN 0x8B4513
# define PINK 0xFFC0CB
# define GRAY 0x808080
# define LIGHT_GRAY 0xD3D3D3
# define DARK_GRAY 0x505050
# define LIGHT_BLUE 0xADD8E6
# define DARK_BLUE 0x00008B
# define LIGHT_GREEN 0x90EE90
# define DARK_GREEN 0x006400
# define GOLD 0xFFD700
# define SILVER 0xC0C0C0
# define BEIGE 0xF5F5DC
# define TEAL 0x008080
# define VIOLET 0xEE82EE
# define COOL_DARKGREY 0x50505A
# define ALMOST_BLACK 0x0A0A14
# define BRIGHT_RED 0xC80000

/* Error Messages */
# define ERR_TOO_FEW_ARGS "Error\nToo few arguments. Provide a .cub file.\n"
# define ERR_TOO_MANY_ARGS "Error\nToo many arguments. Only .cub is required.\n"
# define ERR_INVALID_EXT "Error\nInvalid file extension. .cub extension.\n"
# define ERR_FILE_OPEN "Error\nCannot open the map file.\n"
# define ERR_INVALID_INFO "Error\nInvalid map information in the file.\n"
# define ERR_INVALID_MAP "Error\nInvalid map structure.\n"
# define ERR_TEXTURE_LOAD "Error\nCannot load textures\n"
# define ERR_COLORS_LOAD "Error\nCannot load colors\n"
# define DUPLICATE_TEXTURE "Error\nDuplicate texture identifier: "
# define DUPLICATE_COLOR "Error\nDuplicate color identifier: "
# define UNKNOWN_IDENTIFIER "Error\nUnknown identifier: "
# define BOUNDARY_CELL_NOT_ENCLOSED "Error\nCell (%d, %d) is not enclosed\n"
# define CELL_NOT_ENCLOSED "Error\nCell (%d, %d) is not enclosed\n"
# define SMALL_MAP "Error\nMap is too small\n"
# define INVALID_CHARACTERS "Error\nMap contains invalid characters\n"
# define TRAILING_EMPTY_LINES "Error\nMap contains trailing empty lines\n"
# define SINGLE_PLAYER_NEEDED "Error\nOne player needed. (found %d players)\n"
# define ERR_INIT_MLX "Error\nFailed to initialize MLX"
# define ERR_CREATE_WINDOW "Error\nFailed to create window"
# define ERR_CREATE_IMAGE "Error\nFailed to create image"
# define ERR_GET_IMAGE_ADDR "Error\nFailed to get image address"
# define ERR_ADJ_INCOMPLETE_ROW "Error\nCell (%d, %d) to an incomplete row\n"
# define ERR_MEMORY_ALLOCATION "Error\nFailed to allocate memory\n"

/* ************************************************************************** */
/*                                 STRUCTURES                                 */
/* ************************************************************************** */

/* Basic Types */
typedef struct s_position
{
	int				x;
	int				y;
}					t_position;

typedef struct s_index
{
	int				i;
	int				j;
}					t_index;

typedef struct s_vector
{
	double			x;
	double			y;
}					t_vector;

typedef struct s_rgb
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;
}					t_rgb;

/* Player Camera/View */
typedef struct s_var
{
	double			position_x;
	double			position_y;
	double			direction_x;
	double			direction_y;
	double			plane_x;
	double			plane_y;
}					t_var;

/* Input Handling */
typedef struct s_keys
{
	int				w_pressed;
	int				a_pressed;
	int				s_pressed;
	int				d_pressed;
	int				left_pressed;
	int				right_pressed;
}					t_keys;

/* Graphics */
typedef struct s_data
{
	void			*img;
	char			*addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
}					t_data;

typedef struct s_img2
{
	void			*img_ptr;
	int				*data_addr;
	int				bpp;
	int				line_size;
	int				endian;
	int				width;
	int				height;
}					t_img2;

/* Raycasting */
typedef struct s_dda
{
	int				map_x;
	int				map_y;
	double			side_dist_x;
	double			side_dist_y;
	double			delta_dist_x;
	double			delta_dist_y;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
}					t_dda;

typedef struct s_render
{
	double			perp_wall_dist;
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				tex_num;
	int				tex_x;
	int				side;
}					t_render;

typedef struct s_line_params
{
	int				start_x;
	int				start_y;
	int				end_x;
	int				end_y;
	int				dx;
	int				dy;
	int				sx;
	int				sy;
	int				err;
}					t_line_params;

/* Game Data */
typedef struct s_cub
{
	void			*mlx;
	void			*win;
	t_img2			*img2[5];
	char			**textures;
	char			**colors;
	char			**map;
	int				texture[5][TEXTURE_HEIGHT * TEXTURE_WIDTH];
	long			floor;
	long			ceilling;
	t_var			var;
}					t_cub;

/* Enemy System */
typedef struct s_enemy
{
	double			pos_x;
	double			pos_y;
	double			dir_x;
	double			dir_y;
	double			move_speed;
	int				state;
	double			detection_radius;
	int				move_timer;
}					t_enemy;

typedef struct s_enemy_manager
{
	t_enemy			*enemies;
	int				enemy_count;
}					t_enemy_manager;

/* Minimap */
typedef struct s_map_params
{
	int				tile_size;
	int				offset_x;
	int				offset_y;
	int				radius;
	int				center_x;
	int				center_y;
	int				start_x;
	int				start_y;
	int				view_width;
	int				view_height;
	double			rotation_angle;
}					t_map_params;

/* Game State */
typedef struct s_game_state
{
	t_cub			*data;
	t_data			*img;
	t_keys			keys;
	t_enemy_manager	enemy_manager;
	double			z_buffer[SCREEN_WIDTH];
	int				prev_mouse_x;
	unsigned int	rand_state;
}					t_game_state;

/* Sprite Rendering */
typedef struct s_transform
{
	double			transform_x;
	double			transform_y;
	int				sprite_screen_x;
	int				sprite_height;
	int				draw_start_y;
	int				draw_end_y;
	int				sprite_width;
	int				draw_start_x;
	int				draw_end_x;
}					t_transform;

typedef struct s_render_context
{
	t_game_state	*game;
	t_enemy			*enemy;
	double			*z_buffer;
	t_transform		transform;
}					t_render_context;

typedef struct s_raycast
{
	t_cub			*data;
	t_data			*img;
	t_vector		ray_dir;
	t_dda			dda;
	t_render		render;
}					t_raycast;

typedef struct s_step_calc
{
	int				*step;
	double			*side_dist;
	double			ray_dir;
	double			pos;
	int				map_pos;
	double			delta_dist;
}					t_step_calc;

typedef struct s_tex_data
{
	int				tex_y;
	double			step;
	double			tex_pos;
	unsigned int	color;
	t_rgb			rgb;
}					t_tex_data;

/* ************************************************************************** */
/*                               FUNCTION PROTOTYPES                          */
/* ************************************************************************** */

/* Core Game Functions */
int					main(int argc, char **argv);
t_cub				*get_cub_data(void);
int					initialize_resources(t_cub *data);
int					render_frame(void *param);
void				init(void);

/* Initialization */
t_game_state		*init_game_state(t_cub *data);
void				init_input_state(t_game_state *g);
void				init_window_and_hooks(t_cub *data, t_game_state *g);
void				init_window(t_cub *data, t_data *img, int win_width,
						int win_height);
void				init_camera_plane(double direction_x, double direction_y,
						double pla_x, double pla_y);
void				init_player(int x, int y);
int					initialize_enemies(t_game_state *game);
void				init_enemy(t_enemy *enemy, double pos_x, double pos_y);

/* File Parsing */
int					parse(char *file);
int					parse_file(int fd);
int					parse_map(int fd);
int					parse_line(char *line);
int					process_line(char *line);
int					parse_texture(t_cub *data, char *identifier, char *path,
						char **tokens);
int					parse_color(t_cub *data, char *identifier, char *value,
						char **tokens);
int					parse_rgb(char *str);
int					init_textures_and_colors(t_cub *data);

/* Map Validation */
int					validate_file_extension(const char *filename);
int					validate_inputs(int fd, char **line);
int					validate_full_map(void);
int					validate_cells_and_players(t_cub *data);
int					validate_trailing_lines(int i, t_cub *data);
int					validate_characters(void);
int					check_cell_enclosure(int i, int j);
int					check_trailing_map_lines(int index);

/* Map Processing */
int					initialize_map(t_cub *data, char *line);
int					append_line_to_map(t_cub *data, char *line);
int					add_line_to_map(char *line);
char				*trim_line(const char *line);
int					get_map_height(char **map);
void				get_map_dimensions(t_cub *data, int *width, int *height);
t_position			*collect_valid_positions(t_cub *data, int map_width,
						int map_height, int *count);

/* Map Validation Helpers */
int					is_boundary_cell(int i, int j, char **map);
int					print_boundary_error(int i, int j);
int					has_adjacent_space(int i, int j, char **map);
int					print_adjacent_space_error(int i, int j);
int					is_adjacent_to_incomplete_row(int i, int j, char **map);
int					print_incomplete_row_error(int i, int j);

/* Raycasting */
void				raycast(t_game_state *game);
void				calc_ray_pos_dir(t_var *var, int x, double *ray_dir_x,
						double *ray_dir_y);
void				set_dda_params(t_dda *dda, t_var *var, double ray_dir_x,
						double ray_dir_y);
void				perform_dda(t_dda *dda, char **map);
void				calc_wall_params(t_dda *dda, t_var *var, t_vector ray_dir,
						t_render *render);
void				calc_texture_coords(t_dda *dda, t_var *var,
						t_render *render, t_vector ray_dir);

/* Rendering */
void				draw_textured_line(t_data *img, int x, t_render *render,
						t_cub *data);
void				draw_floor_ceiling(t_data *img, int x, t_render *render,
						t_cub *data);
void				my_mlx_pixel_put(t_data *data, int x, int y, int color);
void				resize_texture(int *texture, t_img2 *img, int x, int y);

/* Input Handling */
int					handle_keypress(int keycode, t_game_state *game);
int					handle_keyrelease(int keycode, t_game_state *game);
int					handle_mouse_move(int x, int y, t_game_state *game);
void				handle_player_movement(t_game_state *game);
int					exit_game(t_game_state *game);
int					on_destroy(void *data);

/* Movement */
void				move_forward_backward(t_cub *data, double move_speed,
						int direction);
void				move_strafe(t_cub *data, double move_speed, int direction);
void				rotate_view(t_cub *data, double rot_speed, int direction);
int					is_valid_position(char **map, double pos_x, double pos_y);

/* Enemy Handling */
void				update_enemies(t_game_state *game);
void				render_enemies(t_game_state *game, double *z_buffer);
void				sort_enemies_by_distance(t_game_state *game);
void				setup_render_context(t_render_context *ctx);
int					is_player_detected(t_enemy *enemy, t_cub *data);
void				update_enemy_random_movement(t_game_state *game,
						t_enemy *enemy);
void				update_enemy_following(t_game_state *game, t_enemy *enemy);

/* Minimap Rendering */
void				draw_2d_map(t_game_state *game);
void				draw_circular_map_border(t_data *img, int center_x,
						int center_y, int radius);
void				draw_map_tiles_circular_zoomed(t_cub *data, t_data *img,
						t_map_params *p);
void				draw_player_on_circular_map(t_cub *data, t_data *img,
						t_map_params *p);
void				draw_enemies_on_circular_map(t_game_state *game,
						t_data *img, t_map_params *p);
void				fill_circle(t_data *img, t_position center, int radius,
						int color);
int					is_in_circle_border(int x, int y, int outer_radius,
						int inner_radius);

/* Texture Loading */
int					load_texture(t_cub *data, int index, char *path);
int					load_image(t_cub *data, t_img2 *img, char *path);

/* Utility Functions */
void				error_exit(const char *message);
void				error_exit_fd_cleanup(const char *message, int fd);
void				error_exit_cleanup(const char *message);
void				free_array(char ***array);
void				free_resources(t_cub *data);
char				**reallocate_map_memory(char **pointer, int size);
void				map_info_error(char **tokens, char *s);
void				custom_srand(t_game_state *g, unsigned int seed);
unsigned int		custom_rand(t_game_state *g);

/* Buffer Handling */
void				clear_buffer(char **buffer);
int					initialize_buffer(char **buffer);
int					read_to_buffer(char **buffer, int fd, int *bytes_read);

/* Get Next Line */
int					get_next_line(int fd, char **line);
int					contains_newline(char *buffer);
int					extract_line(char **buffer, char **line);
int					read_and_append_to_buffer(char **buffer, int fd,
						int *bytes_read);
int					append_to_buffer(char **buffer, char *new_content,
						char **temp);

/* Color Management */
t_rgb				to_rgb(unsigned int clr);
unsigned int		to_int(t_rgb rgb);
void				applyshading(t_rgb *rgb, double d);

/* Array Functions */
int					ft_array_length(char **array);
char				**ft_split(char const *s, char c);

#endif
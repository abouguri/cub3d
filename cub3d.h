/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:03:44 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/06 21:13:12 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <math.h>
# include "mlx.h"

/* Constants */
# define TEXTURE_COUNT 4
# define RGB_COUNT 2
# define TEXTURE_WIDTH 64
# define TEXTURE_HEIGHT 64
# define SCREEN_WIDTH 1280
# define SCREEN_HEIGHT 720
# define CELL_SIZE 40

/* Player orientation */
# define NORTH 'N'
# define SOUTH 'S'
# define EAST 'E'
# define WEST 'W'

/* Map symbols */
# define VALID_MAP_SYMBOLS "01NSEW "

/* Colors */
# define BLACK       	0x000000
# define WHITE       	0xFFFFFF
# define RED         	0xFF0000
# define DARK_RED		0xFF0000
# define GREEN       	0x00FF00
# define BLUE       	0x0000FF
# define YELLOW      	0xFFFF00
# define CYAN        	0x00FFFF
# define MAGENTA     	0xFF00FF
# define ORANGE      	0xFFA500
# define PURPLE      	0x400040
# define BROWN       	0x8B4513
# define PINK        	0xFFC0CB
# define GRAY        	0x808080
# define LIGHT_GRAY  	0xD3D3D3
# define DARK_GRAY   	0x505050
# define LIGHT_BLUE  	0xADD8E6
# define DARK_BLUE   	0x00008B
# define LIGHT_GREEN 	0x90EE90
# define DARK_GREEN  	0x006400
# define GOLD        	0xFFD700
# define SILVER      	0xC0C0C0
# define BEIGE       	0xF5F5DC
# define TEAL			0x008080
# define VIOLET			0xEE82EE
# define COOL_DARKGREY	0x50505A
# define ALMOST_BLACK	0x0A0A14
# define BRIGHT_RED		0xC80000


/* Key codes */
// # define KEY_ESC 53
// # define KEY_W 13
// # define KEY_A 0
// # define KEY_S 1
// # define KEY_D 2
// # define KEY_LEFT 123
// # define KEY_RIGHT 124

/* Key codes for Linux */
# define KEY_ESC 65307     // Escape key
# define KEY_W 119         // w
# define KEY_A 97          // a
# define KEY_S 115         // s
# define KEY_D 100         // d
# define KEY_LEFT 65361    // Left arrow
# define KEY_RIGHT 65363   // Right arrow

/* Error Messages */
# define ERR_TOO_FEW_ARGS "Error\nToo few arguments. Provide a .cub file.\n"
# define ERR_TOO_MANY_ARGS "Error\nToo many arguments. Only the .cub file is required.\n"
# define ERR_INVALID_EXT "Error\nInvalid file extension. The file must have a .cub extension.\n"
# define ERR_FILE_OPEN "Error\nCannot open the map file.\n"
# define ERR_INVALID_INFO "Error\nInvalid map information in the file.\n"
# define ERR_INVALID_MAP "Error\nInvalid map structure.\n"
# define ERR_TEXTURE_LOAD "Error\nCannot load textures\n"
# define ERR_COLORS_LOAD "Error\nCannot load colors\n"
# define DUPLICATE_TEXTURE "Error\nDuplicate texture identifier: "
# define DUPLICATE_COLOR "Error\nDuplicate color identifier: "
# define UNKNOWN_IDENTIFIER "Error\nUnknown identifier: "
# define BOUNDARY_CELL_NOT_ENCLOSED "Error\nCell (%d, %d) is at the boundary and not enclosed\n"
# define CELL_NOT_ENCLOSED "Error\nCell (%d, %d) is not enclosed\n"
# define SMALL_MAP "Error\nMap is too small\n"
# define INVALID_CHARACTERS "Error\nMap contains invalid characters\n"
# define TRAILING_EMPTY_LINES "Error\nMap contains trailing empty lines\n"
# define SINGLE_PLAYER_NEEDED "Error\nMap must contain exactly one player (found %d)\n"
# define ERR_INIT_MLX "Error\nFailed to initialize MLX"
# define ERR_CREATE_WINDOW "Error\nFailed to create window"
# define ERR_CREATE_IMAGE "Error\nFailed to create image"
# define ERR_GET_IMAGE_ADDR "Error\nFailed to get image address"
# define ERR_ADJ_INCOMPLETE_ROW "Error\nCell (%d, %d) is adjacent to an incomplete row\n"
# define ERR_MEMORY_ALLOCATION "Error\nFailed to allocate memory\n"

/* Structures */
typedef struct s_var
{
	double	position_x;
	double	position_y;
	double	direction_x;
	double	direction_y;
	double	plane_x;
	double	plane_y;
}	t_var;

typedef struct s_img2
{
	void	*img_ptr;
	int		*data_addr;
	int		bpp;
	int		line_size;
	int		endian;
	int		width;
	int		height;
}	t_img2;

typedef struct s_cub
{
	void			*mlx;
	void			*win;
	t_img2			*img2[4];
	char			**textures;
	char			**colors;
	char			**map;
	int				texture[4][TEXTURE_HEIGHT * TEXTURE_WIDTH];
	long	floor;
	long	ceilling;
	t_var			var;
}	t_cub;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_data;

/* DDA and rendering structures */
typedef struct s_dda
{
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		side;
}	t_dda;

typedef struct s_render
{
	double	perp_wall_dist;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_num;
	int		tex_x;
	int		side;
}	t_render;

typedef struct s_game_state
{
	t_cub	*data;
	t_data	*img;
}	t_game_state;

/* Function prototypes */
/* Main and utilities */
int				main(int argc, char **argv);
t_cub			*get_cub_data(void);
void			error_exit(const char *message);
void			error_exit_fd_cleanup(const char *message, int fd);
void			error_exit_cleanup(const char *message);
void			free_array(char ***array);
void			free_resources(t_cub *data);
char			**reallocate_map_memory(char **pointer, int size);
int				initialize_map(t_cub *data, char *line);
void			render_map_cells(t_data *img, char **map, int cell_size);
void			draw_grid(t_data *img, int map_width, int map_height, int cell_size);
int				render_frame(void *param);

/* Array functions */
int				ft_array_length(char **array);
int				ft_max_length(char **array);
char			**ft_split(char const *s, char c);
int				ft_strncmp(const char *s1, char *s2, size_t n);

/* File validation */
int				validate_file_extension(const char *filename);
int				parse(char *file);

/* Get next line */
int				get_next_line(int fd, char **line);
int				contains_newline(char *buffer);
int				extract_line(char **buffer, char **line);
int				read_and_append_to_buffer(char **buffer, int fd, int *bytes_read);
int				append_to_buffer(char **buffer, char *new_content, char **temp);

/* Map parsing */
int				parse_file(int fd);
int				parse_map(int fd);
int				parse_line(char *line);
int				validate_full_map(void);
int				initialize_resources(t_cub *data);
int				process_line(char *line);
int				parse_texture(t_cub *data, char *identifier, char *path, char **tokens);
int				parse_color(t_cub *data, char *identifier, char *value, char **tokens);

/* Raycasting and rendering */
void			raycast(t_cub *data, t_data *img);
void			calc_ray_pos_dir(t_var *var, int x, double *ray_dir_x, double *ray_dir_y);
void			set_dda_params(t_dda *dda, t_var *var, double ray_dir_x, double ray_dir_y);
void			perform_dda(t_dda *dda, char **map);
void			calc_wall_params(t_dda *dda, t_var *var, double ray_dir_x,
					double ray_dir_y, t_render *render);
void			calc_texture_coords(t_dda *dda, t_var *var, t_render *render,
					double ray_dir_x, double ray_dir_y);
void			draw_textured_line(t_data *img, int x, t_render *render, t_cub *data);
void			draw_floor_ceiling(t_data *img, int x, t_render *render, t_cub *data);
void			my_mlx_pixel_put(t_data *data, int x, int y, int color);

/* Texture loading */
int				load_texture(t_cub *data, int index, char *path);
int				parse_rgb(char *str);
int				init_textures_and_colors(t_cub *data);

/* Movement and input */
int				handle_keypress(int keycode, t_game_state *game);
void			move_forward_backward(t_cub *data, double move_speed, int direction);
void			move_strafe(t_cub *data, double move_speed, int direction);
void			rotate_view(t_cub *data, double rot_speed, int direction);
int				is_valid_position(char **map, double pos_x, double pos_y);
int				exit_game(t_game_state *game);
int				on_destroy(void *data);
// void			render_frame(t_game_state *game);

/* Rendering helpers */
void			init_window(t_cub *data, t_data *img, int win_width, int win_height);
void			render_map_to_image(t_data *img, char **map, int cell_size);
void			init(void);

#endif
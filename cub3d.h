/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 18:03:44 by abouguri          #+#    #+#             */
/*   Updated: 2025/01/12 18:20:50 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
#define CUB3D_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// Constants
#define BUFFER_SIZE 42
#define TEXTURE_COUNT 4
#define RGB_COUNT 2

// Error Messages
#define ERR_TOO_FEW_ARGS "Error: Too few arguments. Provide a .cub file.\n"
#define ERR_TOO_MANY_ARGS "Error: Too many arguments. Only the .cub file is required.\n"
#define ERR_INVALID_EXT "Error: Invalid file extension. The file must have a .cub extension.\n"
#define ERR_FILE_OPEN "Error: Cannot open the map file.\n"
#define ERR_INVALID_INFO "Error: Invalid map information in the file.\n"
#define ERR_INVALID_MAP "Error: Invalid map structure.\n"

// Structs

typedef struct s_var
{
	double			position_x;
	double			position_y;
	double			direction_x;
	double			direction_y;
	double			plane_x;
	double			plane_y;
	double			camera_x;
	double			ray_direction_x;
	double			ray_direction_y;
	double			side_distance_x;
	double			side_distance_y;
	double			delta_distance_x;
	double			delta_distance_y;
	double			perspective_wall_distance;
	double			wall_x;
	double			step;
	double			texture_position;
	double			frame_time;
	double			move_speed;
	double			rotate_speed;

	int				map_x;
	int				map_y;
	int				step_x;
	int				step_y;
	int				hit;
	int				side;
	int				line_height;
	int				draw_start;
	int				draw_end;
	int				texture_number;
	int				texture_y;
	int				texture_x;

	unsigned int	color;
}			t_var;

typedef struct s_cub {
    char    **textures;
    char    **colors;
    char    **map;
    char    **rgb;
	char    **xpm;
    t_var			var;
} t_cub;

// Function Prototypes
void error_exit(const char *message);
void error_exit_with_cleanup(const char *message, int fd);
t_cub *get_cub_data(void);
void free_array(char ***array);

int validate_file_extension(const char *filename);
int parse(char *file);
int parse_file(int fd);
int parse_map(int fd);
int parse_textures(char *line);
int parse_colors(char *line);

int get_next_line(int fd, char **line);

#endif

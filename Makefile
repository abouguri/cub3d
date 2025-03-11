# Name of the executable
NAME := cub3d

# Compiler and flags
CC := gcc 
CFLAGS := -Wall -Wextra -Werror -DBUFFER_SIZE=100 -Iminilibx-linux -I.

# Source files
SRC := main.c array.c extra.c file.c get_next_line.c movement.c parse.c \
raycast.c render.c textures.c utility.c buffer_utils.c line_utils.c \
read_utils.c rotation.c	input_handling.c mouse_handling.c exit_handling.c \
map_validation.c map_validation2.c map_handling.c file_parsing.c \
texture_parsing.c validate_full_map.c validation_cells.c validation_trailing.c \
validation_characters.c init.c raycast_utils.c math_utils.c minimap.c \
minimap_draw.c dda.c map_render.c image_utils.c draw_utils.c enemy_render.c \
enemy_movement.c enemy_update.c enemy_spawn.c player_movement.c map_utils2.c \

# Object files
OBJ := $(SRC:.c=.o)

# Library paths and linking
LIBRARY := -Lminilibx-linux -lmlx -L/usr/lib/x86_64-linux-gnu -lXext -lX11 -lm
MINILIBX := minilibx-linux/

# Default target
all: $(NAME)

# Build the executable
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBRARY) -o $(NAME)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJ)

# Full clean, including the executable
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

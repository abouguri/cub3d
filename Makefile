# Name of the executable
NAME := cub3d

# Compiler and flags
CC := gcc 
CFLAGS := -Wall -Wextra -Werror -DBUFFER_SIZE=100 -Iminilibx-linux -I. -Ilibft

# Source files
SRC := \
	src/core/main.c src/core/init.c src/core/exit_handling.c src/core/input_handling.c \
	src/core/mouse_handling.c src/core/init2.c src/core/init_game.c src/core/init3.c \
	src/parsing/parse.c src/parsing/file_parsing.c src/parsing/texture_parsing.c \
	src/parsing/get_next_line.c src/parsing/textures.c \
	src/validation/map_validation.c src/validation/map_validation2.c \
	src/validation/validate_full_map.c \
	src/validation/validation_cells.c src/validation/validation_trailing.c \
	src/validation/validation_characters.c \
	src/rendering/render.c src/rendering/raycast.c src/rendering/raycast_utils.c \
	src/rendering/minimap.c src/rendering/minimap_draw.c src/rendering/draw_utils.c \
	src/rendering/map_render.c src/rendering/image_utils.c src/rendering/dda.c \
	src/movement/movement.c src/movement/rotation.c src/movement/player_movement.c \
	src/enemies/enemy_render.c src/enemies/enemy_movement.c src/enemies/enemy_update.c \
	src/enemies/enemy_spawn.c src/enemies/enemy_damage.c src/enemies/enemy_gameover.c \
	src/enemies/enemy_effect.c src/enemies/enemy_deadcolor.c \
	src/map/map_handling.c src/map/map_utils2.c \
	src/utils/utility.c src/utils/extra.c src/utils/math_utils.c \
	src/utils/buffer_utils.c \
	src/utils/line_utils.c src/utils/read_utils.c src/utils/array.c

# Object files
OBJ := $(SRC:.c=.o)

# Libft settings
LIBFT_DIR := libft
LIBFT := $(LIBFT_DIR)/libft.a

# Library paths and linking
LIBRARY := -Lminilibx-linux -lmlx -L/usr/lib/x86_64-linux-gnu -lXext -lX11 -lm -L$(LIBFT_DIR) -lft
MINILIBX := minilibx-linux/

# Default target
all: $(LIBFT) $(NAME)

# Build libft
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Build the executable
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(LIBRARY) -o $(NAME)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

# Full clean, including the executable
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything
re: fclean all

# Add .PHONY to prevent conflicts with files of the same name
.PHONY: all clean fclean re
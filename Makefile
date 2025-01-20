# Name of the executable
NAME := cub3d

# Compiler and flags
CC := gcc  -g3 -fsanitize=address
CFLAGS := -Wall -Wextra -Werror -DBUFFER_SIZE=100 -Iminilibx-linux -I.

# Source files
SRC := main.c 

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

NAME = ChessAI

# Sources
SRCS = $(addprefix srcs/, main.c gui/board.c gui/gl_init.c utils/board_utils.c game/move.c)

OBJS = ${SRCS:.c=.o}

CFLAGS = -g
CC = gcc
RM = rm -f

LIBFT_DIR = includes/libft

# Include paths
INCL = -I $(LIBFT_DIR) -I/usr/include/SOIL

# Libraries
LIBS = -L $(LIBFT_DIR) -lft -lGL -lGLU -lglut -lSOIL

%.o: %.c
	$(CC) $(CFLAGS) ${INCL} -c $< -o ${<:.c=.o}

$(NAME): $(OBJS)
	make -C includes/libft
	$(CC) $(CFLAGS) $(OBJS) $(INCL) $(LIBS) -fsanitize=address -o $(NAME)

all: ${NAME}

clean:
	$(RM) $(OBJS)
	make clean -C includes/libft

fclean: clean
	${RM} ${NAME}
	make fclean -C includes/libft

re: fclean all

.PHONY: all clean fclean re

NAME = codexion
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
OBJ_DIR = obj
SRCS = main.c \
       parser.c \
       init.c \
       sync.c \
       simulation.c \
       time.c \
       routine.c \
       actions.c \
       dongles.c \
       scheduler.c \
       heap.c \
       heap_sift.c \
       monitor.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

help:
	@echo "🧩 Codexion Commands:"
	@echo "make			→ Compile the program"
	@echo "make clean		→ Remove object files (.o)"
	@echo "make fclean		→ Remove object files and executable"
	@echo "make re			→ Recompile everything"

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: %.c codexion.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re help
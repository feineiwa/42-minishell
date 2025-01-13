# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/16 11:21:59 by nrasamim          #+#    #+#              #
#    Updated: 2025/01/13 10:07:38 by frahenin         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

CC		=	cc

FLAG = 

DIRLIB	=	./libft/
FILELIB	=	libft.a
NAMELFT	=	$(addprefix $(DIRLIB), $(FILELIB))

SRC_DIR	=	src/
OBJ_DIR	=	obj/
INCLUDE	=	-g -I ./inc -I ./libft -lreadline

FILES	=	main ft_env_utils ft_parse ft_expand ft_free ft_error_parsing parse_utils extra_libft \
			exec ft_cat ft_echo ft_pwd heredoc pipe ft_export ft_env ft_unset

SRCS	=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(FILES)))
OBJS	=	$(addprefix $(OBJ_DIR), $(FILES:=.o))

all : $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(NAME) : $(OBJS)
	make -C $(DIRLIB)
	$(CC) $(FLAG) $(OBJS) $(NAMELFT) $(INCLUDE) -o $(NAME)

$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(OBJ_DIR)
	$(CC) $(FLAG) $(INCLUDE) -c $< -o $@

clean :
	@ make clean -C $(DIRLIB)
	@rm -rf $(OBJ_DIR)

fclean : clean
	@make fclean -C $(DIRLIB)
	@rm -rf $(NAME)

re : fclean all

d : $(NAME)
	make clean && clear && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=debug -s ./minishell

run : fclean all
	@clear ; ./minishell

.PHONY : all clean fclean re
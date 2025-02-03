NAME    =   minishell

CC      =   gcc

FLAGS   =   -Wall -Werror -Wextra -g

DIRLIB  =   ./libft/
FILELIB =   libft.a
NAMELFT =   $(addprefix $(DIRLIB), $(FILELIB))

SRCS_DIR    =   src/
OBJS_DIR    =   obj/
INCLUDE     =   -I ./inc -I ./libft -lreadline

PARSE_DIR = parsing/
PARSE_FILES = ft_expand ft_parse parse_utils

BUILTINS_DIR = builtins/
BUILTINS_FILES = ft_cd ft_echo ft_env ft_exit ft_export ft_pwd ft_unset

EXEC_DIR = execution/
EXEC_FILES = exec_utils exec_utils2 exec heredoc hdoc_utils hdoc_utils2 pipe redirection

SIGNAL_DIR = signals/
SIGNAL_FILES = ft_signal ft_signal_fork

UTILS_DIR = utils/
UTILS_FILES = extra_libft extra_libft2 ft_env_utils ft_env_utils2 ft_free ft_free_utils error_utils

SRC_FILES += $(addprefix $(PARSE_DIR), $(PARSE_FILES))
SRC_FILES += $(addprefix $(BUILTINS_DIR), $(BUILTINS_FILES))
SRC_FILES += $(addprefix $(EXEC_DIR), $(EXEC_FILES))
SRC_FILES += $(addprefix $(SIGNAL_DIR), $(SIGNAL_FILES))
SRC_FILES += $(addprefix $(UTILS_DIR), $(UTILS_FILES))
SRC_FILES += main

SRCS = $(addprefix $(SRCS_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJS = $(addprefix $(OBJS_DIR), $(addsuffix .o, $(SRC_FILES)))

OBJSF = .cache_exits


####################################################################

all : $(NAME)

$(NAME) : $(OBJSF) $(OBJS)
	@make -C $(DIRLIB)
	$(CC) $(FLAGS) $(OBJS) $(NAMELFT) $(INCLUDE) -o $@

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c
	$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@

$(OBJSF):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)$(PARSE_DIR)
	@mkdir -p $(OBJS_DIR)$(BUILTINS_DIR)
	@mkdir -p $(OBJS_DIR)$(EXEC_DIR)
	@mkdir -p $(OBJS_DIR)$(SIGNAL_DIR)
	@mkdir -p $(OBJS_DIR)$(UTILS_DIR)
	@touch $(OBJSF)

clean :
	@make clean -C $(DIRLIB)
	@rm -rf $(OBJS_DIR)
	@rm -rf $(OBJSF)

fclean : clean
	@make fclean -C $(DIRLIB)
	@rm -rf $(NAME)

re : fclean all

debug : $(NAME)
	@make clean && clear && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --suppressions=debug -s ./minishell

VAL: 
	valgrind --track-fds=yes --suppressions=readline.supp --leak-check=full --show-leak-kinds=all ./minishell

run : fclean all
	@clear && ./minishell

.PHONY : all clean fclean re run debug

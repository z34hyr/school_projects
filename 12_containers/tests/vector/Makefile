
INC = -I../../includes -I../
FLAGS = -Wall -Wextra -Werror --std=c++98
NAME = ft_vector
all:
	clang++ $(FLAGS) $(INC) -o $(NAME) test_vector.cpp
	./$(NAME)

valgr:
	valgrind ./$(NAME)
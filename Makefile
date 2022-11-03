#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/25 13:22:33 by gtoubol           #+#    #+#              #
#    Updated: 2022/11/03 17:53:52 by gtoubol          ###   ########.fr        #
#                                                                              #
#******************************************************************************#


# List of all the sources (.cpp)
# -------------------------------------------------------------------------
SRCS =		main.cpp

# List of the related directories
# -------------------------------------------------------------------------
SRCS_DIR =	srcs
HEAD_DIR =	includes
TPP_DIR =	templates

# List of all compilation options
# -------------------------------------------------------------------------
CXX = 		c++
CXXFLAGS =	-Wall -Wextra -Werror --std=c++98 -g

# Description of the final target
# -------------------------------------------------------------------------
NAME =		webserv

# Libraries
# -------------------------------------------------------------------------

# General rules on makefile
# -------------------------------------------------------------------------
OBJS = 		$(addprefix $(SRCS_DIR)/,$(SRCS:.cpp=.o))
DEPS =		$(OBJS:.o=.d)

INCLUDES =	$(addprefix -I,$(HEAD_DIR) $(TPP_DIR))

RM =		rm -f

vpath %.c $(SRCS_DIR)
vpath %.h $(HEAD_DIR)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -D FT=std $(INCLUDES) -o $@ $(OBJS_STD)

%.o:	%.cpp Makefile
			$(CXX) $(CXXFLAGS) -D FT=std $(INCLUDES) -o $@ -c $<

all:		$(NAME)

clean:		dclean
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
.NOTPARALLEL: re

# Library rules
# -----------------------------------------------------------------------

# General dependences management
# ------------------------------------------------------------------------
%.d:		%.cpp
			$(CXX) -MM -MT $(@:.d=.o) $(CXXFLAGS) -D FT=ft $(INCLUDES) $< > $@

dclean:
			$(RM) $(DEPS)

.PHONY:		dclean
.SILENT:    $(DEPS) dclean

-include	$(DEPS)

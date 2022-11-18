#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtoubol <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/07/25 13:22:33 by gtoubol           #+#    #+#              #
#    Updated: 2022/11/17 16:16:16 by gtoubol          ###   ########.fr        #
#                                                                              #
#******************************************************************************#

SHELL=/bin/bash

# List of all the sources (.cpp)
# -------------------------------------------------------------------------
SRCS =		$(addprefix config/,											\
				configure.cpp												\
				ConfigEntry.cpp												\
				VirtualServer.cpp											\
			)

# List of test sources (.cpp)
# -------------------------------------------------------------------------
TEST = 		$(addprefix $(SRCS_DIR)/,										\
				$(addprefix config/,										\
					test_configure.cpp										\
				)															\
			)

TEST_DIR =	tests
TEST_OBJS =	$(TEST:.cpp=.o)
DOC_FILE = doxygen.conf

TEST_EXE = $(TEST:.cpp=.test)

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
DEPS =		$(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

INCLUDES =	$(addprefix -I,$(HEAD_DIR))

RM =		rm -f

vpath %.c $(SRCS_DIR)
vpath %.h $(HEAD_DIR)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS)

%.o:		%.cpp Makefile
			$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ -c $<

all:		$(NAME)

clean:		dclean
			$(RM) $(OBJS)

tclean:
			$(RM) $(TEST_OBJS) $(TEST_OBJS:.o=.test)

fclean:		clean tclean
			$(RM) $(NAME)


doc:
			doxygen $(DOC_FILE)

test:		$(TEST_EXE)
			pytest

%.test:		%.o $(filter-out main.cpp,$(OBJS))
			$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

re:			fclean all

.PHONY:		all clean fclean re doc tests
.SILENT:	test
.NOTPARALLEL: re

# Library rules
# -----------------------------------------------------------------------

# General dependences management
# ------------------------------------------------------------------------
%.d:		%.cpp
			echo -n "$@ " > $@
			$(CXX) -MM -MT $(@:.d=.o) $(CXXFLAGS) $(INCLUDES) $< >> $@

dclean:
			$(RM) $(DEPS)

.PHONY:		dclean
.SILENT:    $(DEPS) dclean

-include	$(DEPS)

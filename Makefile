# Configuration
NAME := ft_ls
SOURCE_DIR := Source
OBJECT_DIR := Object
INCLUDE_DIR := Header
LIBFT_DIR := Lib/libft/utils/

# Compiler and flags
CC := cc
CFLAGS_COMMON := -I$(INCLUDE_DIR) -Wall -Wextra -Werror -MMD -MP
CFLAGS_DEBUG := $(CFLAGS_COMMON) -g -gdwarf-3 -DDEBUG
CFLAGS_RELEASE := $(CFLAGS_COMMON) -O3 -DNDEBUG

# Default build type (used when calling 'make' with no target)
MODE ?= debug

ifeq ($(MODE),debug)
    CFLAGS := $(CFLAGS_DEBUG)
    BUILD_DIR := $(OBJECT_DIR)/debug
    TARGET := $(NAME)_debug
else
    CFLAGS := $(CFLAGS_RELEASE)
    BUILD_DIR := $(OBJECT_DIR)/release
    TARGET := $(NAME)
endif

# Source files
SRCS := $(wildcard $(SOURCE_DIR)/*.c)
OBJS := $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Libft
LIBFT := $(LIBFT_DIR)/libft.a

# Targets
.PHONY: all debug release clean fclean re libft

all: $(TARGET)

debug:
	@$(MAKE) MODE=debug

release:
	@$(MAKE) MODE=release

$(NAME): $(BUILD_DIR) libft $(OBJS)
	@echo "Linking release version..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@
	@echo "Release executable created: $@"

$(NAME)_debug: $(BUILD_DIR) libft $(OBJS)
	@echo "Linking debug version..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $@
	@echo "Debug executable created: $@"

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

libft:
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@echo "Cleaning up object files..."
	@rm -rf $(OBJECT_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean
	@echo "Object files cleaned."

fclean: clean
	@echo "Cleaning up executables..."
	@rm -f $(NAME) $(NAME)_debug
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "Executables cleaned."

re: fclean all

-include $(DEPS)
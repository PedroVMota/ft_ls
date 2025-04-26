# Project settings
SOURCE_DIR   := Source
OBJECT_DIR   := Object
INCLUDE_DIR  := Header
EXECUTABLE   := ft_ls
CC           := cc

# Path to libft
LIBFT_DIR    := Lib/libft
LIBFT        := $(LIBFT_DIR)/utils/libft.a

# Build mode: can be “debug” or “release”; default is debug
BUILD ?= debug

# Compiler flags per build
CFLAGS_DEBUG   := -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -g -gdwarf-3 -DDEBUG -MMD -MP
CFLAGS_RELEASE := -I$(INCLUDE_DIR) -I$(LIBFT_DIR) -O2 -Wall -Wextra -Werror -MMD -MP

# Choose flags and object dir based on BUILD
ifeq ($(BUILD),debug)
  CFLAGS   := $(CFLAGS_DEBUG)
  OBJ_DIR  := $(OBJECT_DIR)/debug
else ifeq ($(BUILD),release)
  CFLAGS   := $(CFLAGS_RELEASE)
  OBJ_DIR  := $(OBJECT_DIR)/release
else
  $(error Unknown BUILD mode "$(BUILD)"; use “debug” or “release”)
endif

# Source, objects, and dependency files
SRCS := $(wildcard $(SOURCE_DIR)/*.c)
OBJS := $(patsubst $(SOURCE_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# Default target: build executable in the current BUILD mode
all: $(LIBFT) $(EXECUTABLE)


install: $(EXECUTABLE)
	@mkdir -p $(HOME)/.local/bin
	@if [ -e $(HOME)/.local/bin/$(EXECUTABLE) ]; then \
		echo "Removing existing link..."; \
		rm -f $(HOME)/.local/bin/$(EXECUTABLE); \
	fi
	@echo "Exporting the .local/bin to path"
	@export PATH="$(HOME)/.local/bin:$(PATH)"
	@echo "Creating symlink in $(HOME)/.local/bin/"
	@ln -sf $(CURDIR)/$(EXECUTABLE) $(HOME)/.local/bin/$(EXECUTABLE)
	@echo "Installation complete. Make sure $(HOME)/.local/bin is in your PATH."

# Link step
$(EXECUTABLE): $(OBJS)
	@echo "Compiling $(BUILD) version: Linking..."
	@$(CC) $(CFLAGS) $^ $(LIBFT) -o $@
	@echo "$(BUILD) version executable created: $@"

# Compile step (creates $(OBJ_DIR) as needed)
$(OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "Compiling $(BUILD) version: $<"
	@$(CC) $(CFLAGS) -c $< -o $@

# Build libft
$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)/utils

# Clean only object files (both modes)
clean:
	@echo "Cleaning all object files..."
	@rm -rf $(OBJECT_DIR)
	@$(MAKE) -C $(LIBFT_DIR)/utils clean

# Clean objects and executable
fclean: clean
	@echo "Cleaning executable..."
	@rm -f $(EXECUTABLE)
	@rm -f $(HOME)/.local/bin/$(EXECUTABLE)
	@$(MAKE) -C $(LIBFT_DIR)/utils fclean

# Rebuild everything
re: fclean all

# Shortcut targets to switch modes
.PHONY: all clean fclean re debug release test

debug:
	@$(MAKE) BUILD=debug

release:
	@$(MAKE) BUILD=release

test:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(EXECUTABLE) 2> valgrind > ft_ls_output

# Include dependency files
-include $(DEPS)
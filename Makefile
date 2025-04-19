SOURCE_DIR := Source
OBJECT_DIR := Object
INCLUDE_DIR := Header
EXECUTABLE := ft_ls

CC := cc
CFLAGS := -I$(INCLUDE_DIR) -g -MMD -MP -gdwarf-3

SRCS := $(wildcard $(SOURCE_DIR)/*.c)
OBJS := $(patsubst $(SOURCE_DIR)/%.c,$(OBJECT_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)




all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo "Linking..."
	@$(CC) $(CFLAGS) $(OBJS) -g  -o $@;
	@echo "Executable created: $@"

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(OBJECT_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -g  -c $< -o $@

clean:
	@echo "Cleaning up object files..."
	@rm -rf $(OBJECT_DIR)
	@echo "Object files cleaned."

fclean: clean
	@echo "Cleaning up executable..."
	@rm -f $(EXECUTABLE)
	@echo "Executable cleaned."

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re

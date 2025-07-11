TARGET_EXEC ?= msh

BUILD_DIR ?= ./build
TARGET_DIR ?= ./bin
SRC_DIRS ?= .

SRCS := $(shell find $(SRC_DIRS) -type f -name "*.c" -not -path "*/.git/*")
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d -not -path "*/.git*")
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CFLAGS ?= $(INC_FLAGS) -MMD -MP -O3 -Wall -Wextra -Wno-unused-parameter

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(MKDIR_P) $(dir $@)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
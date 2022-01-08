# Taget (executable program)
TARGET_EXE = bldr_uart

# Compiler
CC = clang
CXX = clang++

# Directories
BUILD_DIR = ./bin
SRC_DIR = ./src
SRC_CACHE_DIR = $(SRC_DIR)/Cache

# Source files
SRCS = $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c')

# Object files
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)

# Object dependency files
DEPS = $(OBJS:.o=.d)

# Include directories (with adding -I prefix)
INC_DIRS = $(shell find $(SRC_DIR) -type d)
INC_FLAGS = $(addprefix -I,$(INC_DIRS))

# Common compiler flags
FLAGS = -MMD -MP -Wall -Wextra -pedantic -Werror

# Compiler flags
CFLAGS = $(INC_FLAGS) $(FLAGS) -std=c99
CPPFLAGS = $(INC_FLAGS) $(FLAGS) -std=c++11 -stdlib=libc++

# Link *.o files to target executable
$(BUILD_DIR)/$(TARGET_EXE): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile *.c files
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile *.cpp files
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -c $< -o $@

# Cache source files (from STM32 firmware)
.PHONY: cache-create
cache-create: cache-clean
	mkdir $(SRC_CACHE_DIR)

	cp ./../../../Library/Bootloader/bldr_crc16.h $(SRC_CACHE_DIR)/bldr_crc16.h
	cp ./../../../Library/Bootloader/bldr_crc16.c $(SRC_CACHE_DIR)/bldr_crc16.c

	cp ./../../../Library/Bootloader/bldr_misc.h $(SRC_CACHE_DIR)/bldr_misc.h
	cp ./../../../Library/Bootloader/bldr_misc.c $(SRC_CACHE_DIR)/bldr_misc.c

	cp ./../../../Library/Bootloader/bldr_defs.h $(SRC_CACHE_DIR)/bldr_defs.h
	cp ./../../../Library/Common/basedefs.h $(SRC_CACHE_DIR)/basedefs.h

.PHONY: cache-clean
cache-clean:
	rm -rf $(SRC_CACHE_DIR)

.PHONY: build-create
build-create:
	mkdir $(BUILD_DIR)

.PHONY: build-clean
build-clean:
	rm -rf $(BUILD_DIR)

.PHONY: create
create: clean cache-create build-create

.PHONY: clean
clean: cache-clean build-clean

-include $(DEPS)

# external parameters
export DEBUG = 1
export OPT_LEVEL = 1

# judge if is debug mode
ifeq ($(DEBUG), 0)
	DEBUG_ARG = 
	OPT_ARG = -O$(OPT_LEVEL)
else
	DEBUG_ARG = -DDEBUG -g
	OPT_ARG = 
endif

# directories
export TOP_DIR = $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
TARGET_DIR = $(TOP_DIR)/build
OBJ_DIR = $(TOP_DIR)/build/obj
MIPSIM_OBJ_DIR = $(OBJ_DIR)/mipsim
TEST_OBJ_DIR = $(OBJ_DIR)/test
SRC_DIR = $(TOP_DIR)/src
TEST_DIR = $(TOP_DIR)/test
INC_PATH := /usr/local/include
LIB_PATH := /usr/local/lib
INCLUDE_ARG := -I$(SRC_DIR) -I$(TEST_DIR) -I$(INC_PATH)
LIBRARY_ARG := -L$(LIB_PATH) -lSDL2

# files
MIPSIM_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(MIPSIM_OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
MIPSIM_OBJS += $(patsubst $(SRC_DIR)/%.cpp, $(MIPSIM_OBJ_DIR)/%.o, $(wildcard $(SRC_DIR)/**/*.cpp))
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp, $(TEST_OBJ_DIR)/%.o, $(wildcard $(TEST_DIR)/*.cpp))
TEST_OBJS += $(patsubst $(TEST_DIR)/%.cpp, $(TEST_OBJ_DIR)/%.o, $(wildcard $(TEST_DIR)/**/*.cpp))
TEST_OBJS += $(filter-out $(MIPSIM_OBJ_DIR)/main.o, $(MIPSIM_OBJS))
MIPSIM_TARGET := $(TARGET_DIR)/mipsim
TEST_TARGET := $(TARGET_DIR)/test

# compiler & linker
# NOTE: only support macOS & Debian
OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	include make/macos.make
else
	include make/debian.make
endif

.PHONY: all mipsim test clean

all: mipsim test

mipsim: $(TARGET_DIR) $(MIPSIM_OBJ_DIR) $(MIPSIM_TARGET)

test: $(TARGET_DIR) $(TEST_OBJ_DIR) $(TEST_TARGET)

clean:
	-rm -rf $(MIPSIM_OBJ_DIR)
	-rm -rf $(TEST_OBJ_DIR)
	-rm -f $(MIPSIM_TARGET)
	-rm -f $(TEST_TARGET)

# directories
$(TARGET_DIR):
	mkdir $(TARGET_DIR)

$(MIPSIM_OBJ_DIR):
	mkdir -p $(MIPSIM_OBJ_DIR)

$(TEST_OBJ_DIR):
	mkdir -p $(TEST_OBJ_DIR)

# MIPSim
$(MIPSIM_TARGET): $(MIPSIM_OBJS)
	$(LD) -o $@ $^

$(MIPSIM_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	-mkdir -p $(dir $@)
	$(CC) -o $@ $^

# unit test
$(TEST_TARGET): $(TEST_OBJS)
	$(LD) -o $@ $^

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	-mkdir -p $(dir $@)
	$(CC) -o $@ $^

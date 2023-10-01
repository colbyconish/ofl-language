# Project Configuration
EXE ?= ofc
SRC_DIR ?= src
BUILD_DIR ?= build
LIB_DIR ?= lib
INC_DIR ?= include
TEST_DIR ?= tests

TEST_PRE ?= test_

# C++ configuration
PRODFLAG=-DOFL_PRODUCTION
override CPPFLAGS += -MMD -MP

OPTFLAGS ?= -g3 -O0#-Ofast -flto 
override CXXFLAGS += --std=c++20 $(addprefix -I,$(INC_DIR)) $(OPTFLAGS)

override LIBS := $(shell dir $(LIB_DIR)/*)

override LDLIBS +=-L$(LIB_DIR) -LC:/"Third Party Libraries"/lib 
override LDFLAGS +=-lswe

# Static Configuration
override SRCS := $(shell dir $(SRC_DIR)/*.cpp)
override OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
override TEST_SRCS := $(shell dir $(TEST_DIR)/*.test)
override TEST_OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
override MAIN_TEST_OBJS := $(patsubst $(BUILD_DIR)/%.o,$(BUILD_DIR)/$(TEST_PRE)%.o,$(OBJS))
override TARGET := $(BUILD_DIR)/$(EXE)
override TEST_TARGET := $(BUILD_DIR)/$(TEST_PRE)$(EXE)
override DEPS := $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	echo $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDLIBS) $(LDFLAGS) $^ -o $@

$(TEST_TARGET):

$(OBJS): $(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(PRODFLAG) $(CPPFLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET)
	$(TARGET) run example.xx

.PHONY: debug
debug: $(TARGET)
	gdb $(TARGET) -ex "b main" -ex "layout src" -ex "r run example.xx" 

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

-include $(DEPS)


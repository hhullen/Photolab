PROJECT_NAME=Photolab

CPUS := $(( ($(nproc) + 1) / 2 ))
MAKEFLAGS += -j$(CPUS)

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pthread
TEST_FLAGS := -g -MMD -MP
PROJECT_INCLIDE := -I src/$(PROJECT_NAME)
BUILD_DIR := build
QT_PRO := src/$(PROJECT_NAME)/$(PROJECT_NAME).pro
APP_NAME = $(BUILD_DIR)/release/$(PROJECT_NAME)$(EXTENSION)

IMAGE_PROCESSOR_DIR := src/$(PROJECT_NAME)/image_processor
MATRIX_DIR := src/$(PROJECT_NAME)/libs/matrix

TEST_BUILD_DIR := test_build
TESTS_EXECUTABLE = $(TEST_BUILD_DIR)/tests$(EXTENSION_BIN)
GTEST_LIB_DIR := gtest
GTEST_URL := https://github.com/google/googletest.git

GMOCK_SRC := $(GTEST_LIB_DIR)/googlemock/src/gmock-all.cc
GTEST_SRC := $(GTEST_LIB_DIR)/googletest/src/gtest-all.cc

GMOCK_OBJ := $(TEST_BUILD_DIR)/gmock-all.o
GTEST_OBJ := $(TEST_BUILD_DIR)/gtest-all.o

GMOCK_MAIN := $(GTEST_LIB_DIR)/googlemock/src/gmock_main.cc
GMOCK_MAIN_OBJ := $(TEST_BUILD_DIR)/gmock_main.o

GTEST_INCLUDE += -I $(GTEST_LIB_DIR)/
GTEST_INCLUDE += -I $(GTEST_LIB_DIR)/googletest
GTEST_INCLUDE += -I $(GTEST_LIB_DIR)/googletest/include 
GTEST_INCLUDE += -I $(GTEST_LIB_DIR)/googlemock
GTEST_INCLUDE += -I $(GTEST_LIB_DIR)/googlemock/include

TEST_SRC += $(wildcard $(IMAGE_PROCESSOR_DIR)/*_test.cc)
TEST_SRC += $(wildcard $(MATRIX_DIR)/*_test.cc)

TEST_OBJ := $(addprefix $(TEST_BUILD_DIR)/, $(notdir $(TEST_SRC:.cc=.o)))
TEST_DEP := $(TEST_OBJ:.o=.d)

ifeq ($(OS),Windows_NT)
RM := powershell -NoProfile -Command "Remove-Item -Recurse -Force"
MKDIR := powershell -NoProfile -Command "New-Item -ItemType Directory -Force"
MV := powershell -NoProfile -Command "Move-Item -Recurse -Force"
CP := powershell -NoProfile -Command "Copy-Item -Recurse -Force"
RUN_BIN := ./
RUN := powershell -NoProfile -Command "Start-Process"
EXTENSION_BIN := .exe
EXTENSION := .exe
DEPLOYER := windeployqt.exe
else # MacOs
MKDIR := mkdir -p
MV := mv 
CP := cp
RUN_BIN := ./
EXTENSION_BIN := .out
EXTENSION := .app
RUN := open
DEPLOYER := macdeployqt
endif

.PHONY: all install uninstall dvi dist tests clean

all: tests install
	make dist

install: 
	qmake6 -o $(BUILD_DIR)/Makefile $(QT_PRO) CONFIG+=release
	make -C $(BUILD_DIR) -j$(CPUS)

run:
	$(RUN) $(APP_NAME)

uninstall:
	-@$(RM) $(BUILD_DIR)

dvi:
	pdflatex src/doc.tex

ifeq ($(OS),Windows_NT)
SPACE := $(subst ,, )
COLON := ','
DLLS := $(wildcard $(BUILD_DIR)/release/*.dll)
DLLS := '$(subst $(SPACE),$(COLON),$(DLLS))'
COPY_WIN_DLLS = powershell -Command "Copy-Item -Path $(DLLS) -Destination '$(PROJECT_NAME)'"
endif

dist: install
	-@$(MKDIR) $(PROJECT_NAME)
	$(CP) $(APP_NAME) $(PROJECT_NAME)
	$(DEPLOYER) --dir $(PROJECT_NAME) $(APP_NAME)

tests: $(TEST_BUILD_DIR) $(GTEST_LIB_DIR)
	make $(TESTS_EXECUTABLE)
	$(RUN_BIN)$(TEST_BUILD_DIR)/$@$(EXTENSION_BIN)

$(TEST_BUILD_DIR):
	-@$(MKDIR) $(TEST_BUILD_DIR)
	
$(TESTS_EXECUTABLE): $(GTEST_LIB_DIR) $(GTEST_OBJ) $(GMOCK_OBJ) $(GMOCK_MAIN_OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(filter-out $<,$^) -o $@

$(GTEST_LIB_DIR):
	git clone --depth=1 $(GTEST_URL) $(GTEST_LIB_DIR)
	
$(GTEST_OBJ): $(GTEST_SRC)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -c $< -o $@

$(GMOCK_OBJ): $(GMOCK_SRC)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -c $< -o $@

$(GMOCK_MAIN_OBJ): $(GMOCK_MAIN)
	$(CXX) $(CXXFLAGS) $(GTEST_INCLUDE) -c $< -o $@

$(TEST_BUILD_DIR)/%_test.o:
	$(CXX) $(CXXFLAGS) $(TEST_FLAGS) $(PROJECT_INCLIDE) $(GTEST_INCLUDE) -c $(filter %/$(@F:.o=.cc),$(TEST_SRC)) -o $@

-include $(TEST_DEP)

clean:
	-@$(RM) $(TEST_BUILD_DIR)
	-@$(RM) $(GTEST_LIB_DIR)
	-@$(RM) $(BUILD_DIR)
	-@$(RM) $(PROJECT_NAME)

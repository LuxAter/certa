
SHELL=/bin/bash
ifndef .VERBOSE
    .SILENT:
endif
ROOT=$(shell pwd)
ROOT=/home/arden/Programming/cpp/linalg
CXX=clang++
CC=clang
CCIGNORE=
CXXIGNORE=
CCFLAGS=-fPIC -Wall -Wpedantic --static
CXXFLAGS=-std=c++17 -fPIC -Wall -Wpedantic --static
LINK=
SOURCE=src
INCLUDE_DIR=include
INCLUDE=
BUILD=build
COMMON_INCLUDE=-I$(ROOT)/$(INCLUDE_DIR) $(INCLUDE)

SCAN_COLOR=\033[1;35m
BUILD_COLOR=\033[32m
CLEAN_COLOR=\033[1;33m
LINK_COLOR=\033[1;32m
INSTALL_COLOR=\033[1;36m
CMD_COLOR=\033[1;34m
HELP_COLOR=\033[1;34m

define scan_target
printf "%b%s%b\n" "$(SCAN_COLOR)" "Scaning dependencies for target $(1)" "\033[0m"
endef
define complete_target
printf "%s\n" "Built target $(1)"
endef
define clean_target
printf "%b%s%b\n" "$(CLEAN_COLOR)" "Cleaning target $(1)" "\033[0m"
endef
define install_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Installing target $(1)" "\033[0m"
endef
define uninstall_target
printf "%b%s%b\n" "$(INSTALL_COLOR)" "Unnstalling target $(1)" "\033[0m"
endef
define print_build_c
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C object $$str" "\033[0m"
endef
define print_build_cpp
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(BUILD_COLOR)" "Building C++ object $$str" "\033[0m"
endef
define print_link_lib
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking static library $$str" "\033[0m"
endef
define print_link_exe
str=$$(realpath --relative-to="$(ROOT)" "$(1)");    printf "%b%s%b\n" "$(LINK_COLOR)" "Linking executable $$str" "\033[0m"
endef
define print_run_cmd
printf "%b%s%b\n" "$(CMD_COLOR)" "Running '$(1)'" "\033[0m"
endef
define help
printf "%b%*s%b: %s\n" "$(HELP_COLOR)" 20 "$(1)" "\033[0m" "$(2)"
endef

all: build-linalg
clean: clean-linalg
install: install-linalg
uninstall: uninstall-linalg

# LINALG {{{

LINALG=linalg
LINALG_FILES=src/main.cpp
LINALG_OBJS=$(LINALG_FILES:%=$(ROOT)/$(BUILD)/%.o)
-include $(LINALG_OBJS:.o=.d)

build-linalg:  pre-linalg $(LINALG)
	$(call complete_target,$(shell basename $(LINALG)))

clean-linalg:
	$(call clean_target,$(shell basename $(LINALG)))
	if [ -e "$(LINALG)" ]; then rm $(LINALG); fi

pre-linalg:
	$(call scan_target,$(shell basename $(LINALG)))

$(LINALG): $(LINALG_OBJS) FORCE
	$(call print_link_exe,$(shell basename $(LINALG)))
	$(CXX) $(LINALG_OBJS)  $(LINK) $(COMMON_INCLUDE) -o $(LINALG)

install-linalg: build-linalg
	$(call install_target,$(shell basename $(LINALG)))
	mkdir -p $(INSTALL_PATH)/bin/
	cp $(LINALG) $(INSTALL_PATH)/bin

uninstall-linalg:
	$(call uninstall_target,$(shell basename $(LINALG)))
	if [ -e "$(INSTALL_PATH)/bin/$(shell basename $(LINALG))" ]; then rm $(INSTALL_PATH)/bin/$(shell basename $(LINALG)); fi

# }}}
# GTEST {{{

build-gtest:  pre-gtest $(BUILD)/libgtest.a
	$(call complete_target,googletest)

clean-gtest:
	$(call clean_target,googletest)
	if [ -d "$(BUILD)/googletest" ]; then rm $(BUILD)/googletest -r; fi
	if [ -e "$(BUILD)/libgtest.a" ]; then rm $(BUILD)/libgtest.a; fi

pre-gtest:
	$(call scan_target,googletest)

$(BUILD)/libgtest.a: gtest/googletest/src/gtest-all.cc gtest/googletest/src/gtest_main.cc
	mkdir -p $(BUILD)/googletest
	$(call print_build_cpp,$(BUILD)/googletest/gtests_all.o)
	$(CXX) -isystem gtest/googletest/include -Igtest/googletest -pthread -c gtest/googletest/src/gtest-all.cc -o $(BUILD)/googletest/gtests_all.o
	$(call print_build_cpp,$(BUILD)/googletest/gtests_main.o)
	$(CXX) -isystem gtest/googletest/include -Igtest/googletest -pthread -c gtest/googletest/src/gtest_main.cc -o $(BUILD)/googletest/gtests_main.o
	$(call print_link_lib,libgtest.a)
	ar -rc $(BUILD)/libgtest.a $(BUILD)/googletest/gtests_main.o $(BUILD)/googletest/gtests_all.o

# }}}
# TESTS {{{

TESTS=unit-test
TESTS_FILES=$(shell find "tests/" -name "*.cpp")
TESTS_OBJS=$(TESTS_FILES:%=$(ROOT)/$(BUILD)/%.o)
-include $(TESTS_OBJS:.o=.d)

build-tests: build-gtest pre-tests $(TESTS)
	$(call complete_target,$(shell basename $(TESTS)))

clean-tests:
	$(call clean_target,$(shell basename $(TESTS)))
	if [ -e "$(TESTS)" ]; then rm $(TESTS); fi

pre-tests:
	$(call scan_target,$(shell basename $(TESTS)))

$(TESTS): $(TESTS_OBJS) FORCE
	$(call print_link_exe,$(shell basename $(TESTS)))
	$(CXX) $(TESTS_OBJS)  $(BUILD)/libgtest.a -lpthread $(LINK) $(COMMON_INCLUDE) -o $(TESTS)

install-tests: build-tests
	$(call install_target,$(shell basename $(TESTS)))
	mkdir -p $(INSTALL_PATH)/bin/
	cp $(TESTS) $(INSTALL_PATH)/bin

uninstall-tests:
	$(call uninstall_target,$(shell basename $(TESTS)))
	if [ -e "$(INSTALL_PATH)/bin/$(shell basename $(TESTS))" ]; then rm $(INSTALL_PATH)/bin/$(shell basename $(TESTS)); fi

# }}}
# TEST {{{

test: build-tests
	$(call execute,test)
	./unit-test --gtest_color=yes
	$(call complete_target,test)


# }}}

$(ROOT)/$(BUILD)/%.cpp.o: %.cpp
	mkdir -p $(@D)
	$(call print_build_cpp,$@)
	$(CXX) $(CXXFLAGS) -MMD -c $(COMMON_INCLUDE) $< -o $@

$(ROOT)/$(BUILD)/%.c.o: %.c
	mkdir -p $(@D)
	$(call print_build_c,$@)
	$(CC) $(CCFLAGS) -MMD -c $(COMMON_INCLUDE) $< -o $@

FORCE:

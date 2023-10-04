# Variables
BUILD_DIR=build
CLANG_FILE=.clang-format
CLANG_FILE_WAY=..$(SEP)materials$(SEP)linters$(SEP)$(CLANG_FILE)
CPPCH_SETUP= \
			--enable=all --suppress=missingInclude \
			-v --language=c++ --std=c++17
CHECK_FILES=$ \
			cli/*.h \
			cli/*.cc \
			main.cc   \
			tests/*.h  \
			tests/*.cc  \
			lib/bin_tree/*.h \
			lib/bin_tree/*.cc \
			lib/hash_table/*.h \
			lib/hash_table/*.cc \
			lib/vault/engine/*.h \
			lib/vault/engine/*.cc \
			lib/vault/data_structure/*.h \
			lib/vault/data_structure/*.cc \
			lib/vault/engine/channel/channel.h
TO_DELETE_FILES=*.o *.a *.out *.dSYM *.gch *.gcda \
				*.gcno .DS_Store $(CLANG_FILE) *.info
TO_DELETE_FOLDERS=$(BUILD_DIR) $(BUILD_DIR)_tests report *.dSYM


# Crossplatform specs
SEP='\'
MAKEDIR=md
DELETE_FOLDER=RD /S/Q
DELETE_FILE=DEL /F
COPY=copy
OPEN=start
OS=$(shell uname)

ifeq ($(OS), Linux)
	OPEN=xdg-open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp
endif

ifeq ($(OS), Darwin)
	OPEN=open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp
endif

# Targets
.PHONY: all check tests clean gcov_report cli test_cli leaks

all: clean tests cli check

check:
	cppcheck $(CPPCH_SETUP) $(CHECK_FILES)
	$(COPY) $(CLANG_FILE_WAY) $(CLANG_FILE)
	clang-format -i --style=Google $(CHECK_FILES)
	clang-format -n --style=Google $(CHECK_FILES)

tests: 
	$(MAKEDIR) $(BUILD_DIR)_tests
	cmake -S . -B $(BUILD_DIR)_tests -D BUILD_MODE=test
	cd $(BUILD_DIR)_tests && make && ./KeyValueVault_tests

gcov_report: gcov_build
	lcov --ignore-errors mismatch --ignore-errors empty -t "test" -o test.info -c -d .
	genhtml --ignore-errors mismatch --ignore-errors empty -o report test.info

gcov_build:
	$(MAKEDIR) $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) -D BUILD_MODE=gcov_build
	cd $(BUILD_DIR) && make

cli:
	$(MAKEDIR) $(BUILD_DIR)
	cmake -S . -B $(BUILD_DIR) -D BUILD_MODE=build
	cd $(BUILD_DIR) && make

clean:
	$(DELETE_FOLDER) $(TO_DELETE_FOLDERS)
	$(DELETE_FILE) $(TO_DELETE_FILES)

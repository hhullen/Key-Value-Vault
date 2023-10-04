cmake_minimum_required(VERSION 3.22)

project(KeyValueVault)

set(CODE_DIAG_FLAGS ${CODE_DIAG_FLAGS}
  "-g --all-warnings --extra-warnings -Werror -Wpedantic -Wshadow \
  -Wdiv-by-zero -Wcomment -Wdeprecated -Wdouble-promotion \
  -Wconversion -Wdangling-else -Wnull-dereference -Wsign-conversion  \
  -Wunused-variable -Wunused-value -Wformat-security"
)

set(GCOV_FLAGS ${GCOV_FLAGS}
    -g
    --coverage
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17 CACHE PATH "" FORCE)
# set(CMAKE_BUILD_TYPE Release CACHE PATH "" FORCE)

include_directories(${CMAKE_HOME_DIRECTORY})


set(BUILD_SOURCES ${BUILD_SOURCES} 
    lib/vault/engine/vault_engine.cc
    lib/bin_tree/bin_tree.cc
    include/StrPlus/str_plus.cc
)

# set(GCOV_SOURCES ${GCOV_SOURCES} 
#     lib/vault/engine/vault_engine.cc
# )

set(TEST_SOURCES ${TEST_SOURCES}
    tests/KeyValueVault_tests.cc 
    lib/bin_tree/bin_tree.cc
    include/StrPlus/str_plus.cc
)

if(BUILD_MODE STREQUAL "build")
    set(CMAKE_CXX_COMPILER g++ CACHE PATH "" FORCE)
    set(CMAKE_CXX_FLAGS ${CODE_DIAG_FLAGS} CACHE PATH "" FORCE)
    add_executable(KeyValueVault main.cc ${BUILD_SOURCES})
elseif(BUILD_MODE STREQUAL "gcov_build")
    set(CMAKE_CXX_FLAGS ${TEST_FLAGS}" "${GCOV_FLAGS} CACHE PATH "" FORCE)
    add_executable(KeyValueVault_gcov main.cc)
elseif(BUILD_MODE STREQUAL "test")
    include(FetchContent)
    FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_executable(KeyValueVault_tests ${TEST_SOURCES})
    target_link_libraries(KeyValueVault_tests GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(KeyValueVault_tests)
else()
    message("\nAchtung! No build mode specified. Add \"-D BUILD_MODE=<any_build_mode>\" to cmake command\n")
endif()

cmake_minimum_required(VERSION 3.22)

project(KeyValueVault)

set(CODE_DIAG_FLAGS ${CODE_DIAG_FLAGS}
    -Wall
    -Wextra
    -Werror
    -Wshadow
    -Wpedantic
    -Wconversion
    -Wsign-conversion
    -Wnull-dereference
)

set(TEST_FLAGS ${TEST_FLAGS}
    -lgtest
    -pthread
    -lgtest_main
)

set(GCOV_FLAGS ${GCOV_FLAGS}
    --coverage
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17 CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER "g++" CACHE PATH "" FORCE)
set(CMAKE_BUILD_TYPE Release CACHE PATH "" FORCE)

include_directories(${CMAKE_HOME_DIRECTORY})


set(BUILD_SOURCES ${BUILD_SOURCES} 
    lib/vault/engine/vault_engine.cc
)

# set(GCOV_SOURCES ${GCOV_SOURCES} 
#     lib/vault/engine/vault_engine.cc
# )

# set(TEST_SOURCES ${TEST_SOURCES} 
#     lib/vault/engine/vault_engine.cc
# )

if(BUILD_MODE STREQUAL "build")
    set(CMAKE_CXX_FLAGS ${CODE_DIAG_FLAGS} CACHE PATH "" FORCE)
    add_executable(KeyValueVault main.cc ${BUILD_SOURCES})
elseif(BUILD_MODE STREQUAL "gcov_build")
    set(CMAKE_CXX_FLAGS ${TEST_FLAGS}" "${GCOV_FLAGS} CACHE PATH "" FORCE)
    add_executable(KeyValueVault_gcov main.cc)
elseif(BUILD_MODE STREQUAL "test")
    set(CMAKE_CXX_FLAGS ${TEST_FLAGS} CACHE PATH "" FORCE)
    add_executable(KeyValueVault_tests main.cc)
    # include(FetchContent)
    # FetchContent_Declare(
    #   googletest
    #   # Specify the commit you depend on and update it regularly.
    #   URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
    # )
    # # For Windows: Prevent overriding the parent project's compiler/linker settings
    # set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    # FetchContent_MakeAvailable(googletest)

    # # Now simply link against gtest or gtest_main as needed. Eg
    # add_executable(example example.cpp)
    # target_link_libraries(example gtest_main)
    # add_test(NAME example_test COMMAND example)
else()
    message("\nAchtung! No build mode specified. Add \"-D BUILD_MODE=<any_build_mode>\" to cmake command\n")
endif()

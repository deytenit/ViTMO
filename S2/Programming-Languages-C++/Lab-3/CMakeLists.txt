cmake_minimum_required(VERSION 3.14)

project(Big1 LANGUAGES CXX)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
      DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
      TLS_VERIFY ON
  )
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "TESTS_BUILD_TYPE ${TESTS_BUILD_TYPE}")

if (TESTS_BUILD_TYPE MATCHES ASAN)
    set(COMPILE_OPTS -Wall -Wextra -Werror -pedantic -pedantic-errors -O1 -fsanitize=address -fno-omit-frame-pointer
        -fno-inline -fno-sanitize-recover=all)
    set(LINK_OPTS -fsanitize=address)
endif()
if (TESTS_BUILD_TYPE MATCHES MSAN)
    set(COMPILE_OPTS -Wall -Wextra -Werror -pedantic -pedantic-errors -O1 -fsanitize=leak
        -fno-omit-frame-pointer -fno-sanitize-recover=all)
    set(LINK_OPTS -fsanitize=leak)
endif()
if (TESTS_BUILD_TYPE MATCHES USAN)
    set(COMPILE_OPTS -Wall -Wextra -Werror -pedantic -pedantic-errors -O1
        -fsanitize=undefined,float-cast-overflow,float-divide-by-zero
        -fno-omit-frame-pointer -fno-sanitize-recover=all
        -fsanitize-recover=alignment)
    set(LINK_OPTS
        -fsanitize=undefined,float-cast-overflow,float-divide-by-zero)
endif()

# Configure clang-tidy
if (${USE_CLANG_TIDY})
    set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(
    REQUIRES gtest/1.13.0
    GENERATORS cmake_find_package
)

conan_cmake_autodetect(settings)

conan_cmake_install(
    PATH_OR_REFERENCE .
    BUILD missing
    REMOTE conancenter
    SETTINGS ${settings}
)

add_subdirectory(libraries)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE big1::SeamCarver)

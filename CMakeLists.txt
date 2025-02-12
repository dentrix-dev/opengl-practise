cmake_minimum_required(VERSION 3.5)

project(openglpractise LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the glad library (C language)
add_library(glad STATIC glad.c)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Add the main executable
add_executable(openglpractise main.cpp)

# Find GLFW
find_package(glfw3 REQUIRED)

# Link everything properly
target_link_libraries(openglpractise PRIVATE glad glfw GL dl)

include(GNUInstallDirs)
install(TARGETS openglpractise
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

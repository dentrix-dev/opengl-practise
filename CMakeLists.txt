cmake_minimum_required(VERSION 3.5)

project(learnopengl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(learnopengl main.cpp glad.c)

# Add the include directory (relative to the project root)
target_include_directories(learnopengl PRIVATE ${CMAKE_SOURCE_DIR}/include)
# Find and link GLFW (ensure version 3.3 or newer is installed)
find_package(glfw3 3.3 REQUIRED)
# Link the GLFW library to your target.
# Use 'glfw3::glfw' if that's the imported target provided by your GLFW installation.
target_link_libraries(learnopengl PRIVATE glfw dl)
include(GNUInstallDirs)
install(TARGETS learnopengl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

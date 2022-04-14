cmake_minimum_required(VERSION 3.5)

project(BrickBreak VERSION 0.1)

add_subdirectory(Dependencies/glad)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(self_include_dir "${PROJECT_SOURCE_DIR}/BrickBreak/src")
set(glad_include_dir "${PROJECT_SOURCE_DIR}/Dependencies/glad/include")

file(GLOB source_files
  "${PROJECT_SOURCE_DIR}/BrickBreak/src/*.cpp"
)
file(GLOB include_files 
  "${self_include_dir}"
  "${glad_include_dir}"
)

add_executable(BrickBreak "${source_files}")
target_include_directories(BrickBreak PUBLIC "${include_files}")

target_link_libraries(BrickBreak PRIVATE glfw dl glad pthread)

add_custom_command(
    TARGET BrickBreak
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/BrickBreak/shader ${PROJECT_SOURCE_DIR}/bin/shader
)

add_custom_command(
    TARGET BrickBreak
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/BrickBreak/textures ${PROJECT_SOURCE_DIR}/bin/textures
)

add_custom_command(
    TARGET BrickBreak
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/BrickBreak/levels ${PROJECT_SOURCE_DIR}/bin/levels
)
# install(TARGETS BrickBreak DESTINATION lib)

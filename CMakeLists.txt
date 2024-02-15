cmake_minimum_required(VERSION 3.27)
project(ymir)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

find_library(glfw glfw REQUIRED)

set(YMIR_LIBS_PATH ${CMAKE_SOURCE_DIR}/libs)
set(YMIR_CLI_SOURCES
  src/cli_main.cpp
  src/project.cpp
)

set(YMIR_GUI_SOURCES
  src/gui_main.cpp
  src/project.cpp
  src/gui.cpp
)

set(LIBS_SOURCES
  ${YMIR_LIBS_PATH}/src/imgui/imgui.cpp 
  ${YMIR_LIBS_PATH}/src/imgui/imgui_demo.cpp 
  ${YMIR_LIBS_PATH}/src/imgui/imgui_draw.cpp 
  ${YMIR_LIBS_PATH}/src/imgui/imgui_tables.cpp 
  ${YMIR_LIBS_PATH}/src/imgui/imgui_widgets.cpp 
  
  ${YMIR_LIBS_PATH}/src/imgui/imgui_impl_glfw.cpp 
  ${YMIR_LIBS_PATH}/src/imgui/imgui_impl_opengl3.cpp 

  ${YMIR_LIBS_PATH}/src/glad/gl.c
)

# CLI executable
add_executable(ymir ${YMIR_CLI_SOURCES})

# GUI executable
add_executable(ymir-gui ${YMIR_GUI_SOURCES} ${LIBS_SOURCES})
target_include_directories(ymir-gui BEFORE PUBLIC ${YMIR_LIBS_PATH}/include)
target_link_libraries(ymir-gui PRIVATE ${glfw})
target_compile_definitions(ymir-gui PUBLIC GLFW_INCLUDE_NONE)
target_compile_features(ymir-gui PUBLIC cxx_std_20)

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Will install the project to be able to be used as a CLI command
install(TARGETS ${PROJECT_NAME} ymir-gui DESTINATION bin)

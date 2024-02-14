cmake_minimum_required(VERSION 3.27)
project(ymir)

set(YMIR_SOURCES
  src/main.cpp
  src/project.cpp
)

add_executable(${PROJECT_NAME} ${YMIR_SOURCES})

# Will install the project to be able to be used as a CLI command
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

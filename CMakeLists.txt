cmake_minimum_required(VERSION 2.8...3.5)

project(assignment)


add_executable(Factory.out Factory.cpp)

set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


target_link_libraries( Factory.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
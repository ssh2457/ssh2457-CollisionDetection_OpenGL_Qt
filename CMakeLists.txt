cmake_minimum_required (VERSION 3.8)

project ("CollisionDetection_OpenGL")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (${PROJECT_NAME} "main.cpp"
				"include/KDTree.h" "KDTree.cpp"
				"include/Node.h"
				"include/Point.h")
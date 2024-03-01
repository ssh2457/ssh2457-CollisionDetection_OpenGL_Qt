cmake_minimum_required (VERSION 3.8)

project ("CollisionDetection_OpenGL_Qt")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include sub-projects.
add_subdirectory ("GLFW")
add_subdirectory ("GLEW")
add_subdirectory ("GLM")
add_subdirectory ("SPDLOG")

add_executable (${PROJECT_NAME} "main.cpp"
				"include/Particle2D.h" "Particle2D.cpp"
				"include/Point2D.h" 
				"include/Common.h"
				"include/Rectangle.h" "Rectangle.cpp"
				"include/Shape2D.h"
				"include/QuadTree.h" "QuadTree.cpp"
				"include/SimpleShader.h" "SimpleShader.cpp"
				"include/Window.h" "Window.cpp"
				"include/Graphic.h" "Graphic.cpp")
                
# TODO: Add tests and install targets if needed.
target_link_libraries(${CMAKE_PROJECT_NAME}
	glfw
	libglew_static
	spdlog
	glm)
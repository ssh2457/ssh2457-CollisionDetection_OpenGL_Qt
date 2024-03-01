#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "include/Common.h"
#include "include/Rectangle.h"
#include "include/Particle2D.h"
#include "include/QuadTree.h"
#include "include/SimpleShader.h"
#include "include/Window.h"

#include "include/Graphic.h"

#include <cstdlib>
#include <set>

using namespace std;

const GLint WIDTH = 1366;
const GLint HEIGHT = 768;

static const int X = 0;
static const int Y = 1;

/*
*			  +y
*		      |
*		      |
*			  |
*     ------(0,0) -----> +x
*             |
*			  |
*			  |
*/

int main(void) 
{

	// Window
	SPDLOG_INFO("Create GLFW window");

	unique_ptr<Window> mainWindow = make_unique<Window>(WIDTH, HEIGHT, 60.f);
	mainWindow->Initialise();

	unique_ptr<Graphic> graphic_controller = Graphic::GetInstance();
	graphic_controller->Initialize(WIDTH, HEIGHT);

	//Rectangle boundary(0.f, 0.f, 2 * HALF_SIMUL_WIDTH - 1, 2 * HALF_SIMUL_HEIGHT - 1, glm::vec4(0.f, 0.5f, 0.5f, 1.f));
	Rectangle boundary(0.f, 0.f, WIDTH - 1, HEIGHT - 1, glm::vec4(0.f, 0.5f, 0.5f, 1.f));
	
	QuadTree qTree(boundary, 4);

	for (int i = 0; i < 100; ++i) {
		int x = rand() % ((WIDTH / 2) - 10);
		if (rand() % 2 == 0) {
			x = -x;
		}

		int y = rand() % ((HEIGHT / 2) - 10);
		if (rand() % 2 == 0) {
			y = -y;
		}

		int xVel = rand() % 300;
		if (rand() % 2 == 0) {
			xVel = -xVel;
		}


		int yVel = rand() % 300;
		if (rand() % 2 == 0) {
			yVel = -yVel;
		}

		float r = (rand() % 101) / 100.f;
		float g = (rand() % 101) / 100.f;
		float b = (rand() % 101) / 100.f;

		glm::vec4 rgba(r, g, b, 1.f);


		Particle2D particle(x, y, 15.f, xVel, yVel, rgba);
		qTree.Insert(particle);
	}

	
	

	GLfloat last = 0.0f;
	GLfloat delta = 0.0f;

	GLuint frame = 0;
	GLfloat time_sum = 0.f;

	// Loop until window closed
	while (!mainWindow->GetShouldClose()) {
		GLfloat current = glfwGetTime(); // SDL_GetPerformanceCounter();
		delta = current - last;
		last = current;

		time_sum += delta;
		++frame;
		if (time_sum >= 1.f) {
			GLfloat fps = frame / time_sum;
			SPDLOG_INFO("FPS: {}", fps);
			time_sum = 0.f;
			frame = 0;
		}



		// Get + Handle User Input
		glfwPollEvents();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		qTree.Update(boundary, delta, &qTree);
		qTree.ShowPartition(graphic_controller.get());
		qTree.showParticle(graphic_controller.get());
		

		mainWindow->SwapBuffers();
	}

    return 0;
}
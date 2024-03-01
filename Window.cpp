#include "include/Window.h"


Window::Window()
	: mWidth(800)
	, mHeight(600)
	, mWindow(nullptr)
	, mBufferWidth(0)
	, mBufferHeight(0)
	, mLastX(0.f)
	, mLastY(0.f)
	, mXChange(0.f)
	, mYChange(0.f)
	, mMouseFirstMoved(true)
	, mMouseControl(false)
	, mFOV(45.f) {
	//memset(mKeys, false, 512);
	for (int i = 0; i < 512; ++i) {
		mKeys[i] = false;
	}
}

Window::Window(const GLint windowWidth, const GLint windowHeight, GLfloat fov)
	: mWidth(windowWidth)
	, mHeight(windowHeight)
	, mWindow(nullptr)
	, mBufferWidth(0)
	, mBufferHeight(0), mLastX(0.f)
	, mLastY(0.f)
	, mXChange(0.f)
	, mYChange(0.f)
	, mMouseFirstMoved(true)
	, mMouseControl(false)
	, mFOV(fov) {
	//memset(mKeys, false, 512);
	for (int i = 0; i < 512; ++i) {
		mKeys[i] = false;
	}
}

Window::~Window() {
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

int Window::Initialise() {
	// Initialise GLFW

	if (!glfwInit()) {
		SPDLOG_ERROR("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = No Backwards compatiblity
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Create window
	mWindow = glfwCreateWindow(mWidth, mHeight, "Main window", NULL, NULL);
	if (!mWindow) {
		SPDLOG_ERROR("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get Buffer size information
	glfwGetFramebufferSize(mWindow, &mBufferWidth, &mBufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mWindow);

	// Handle key + mouse input
	CreateCallbacks();

	// Hide mouse cursor in window
	// glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Allow mordern extension features
	glewExperimental = GL_TRUE;

	// Initialise GLEW
	if (glewInit() != GLEW_OK) {
		SPDLOG_ERROR("GLEW initialisation failed!");;
		glfwDestroyWindow(mWindow);
		glfwTerminate();
		return 1;
	}

	// Depth test
	//glEnable(GL_DEPTH_TEST);

	// Face cull 
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Setup viewport size
	glViewport(0, 0, mBufferWidth, mBufferHeight);

	// Set window user pointer
	glfwSetWindowUserPointer(mWindow, this);

	return 0;
}

bool Window::GetShouldClose() {
	return glfwWindowShouldClose(mWindow);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(mWindow);
}

GLint Window::GetBufferWidth() const {
	return mBufferWidth;
}

GLint Window::GetBufferHeight() const {
	return mBufferHeight;
}

bool* Window::GetKeys()
{
	return mKeys;
}

GLfloat Window::GetXChange() {
	GLfloat theChange = mXChange;
	mXChange = 0;
	return theChange;
}

GLfloat Window::GetYChange() {
	GLfloat theChange = mYChange;
	mYChange = 0;
	return theChange;
}

GLfloat Window::GetFOV() const {
	return mFOV;
}

bool Window::GetMouseControl() const {
	return mMouseControl;
}

void Window::CreateCallbacks(){
	glfwSetKeyCallback(mWindow, HandleKeys);
	glfwSetScrollCallback(mWindow, HandleScroll);
	glfwSetFramebufferSizeCallback(mWindow, HandleWindowSize);
	glfwSetCursorPosCallback(mWindow, HandleMousePos);
	glfwSetMouseButtonCallback(mWindow, HandleOnMouseButton);
}

void Window::Reshape(GLint width, GLint height) {
	mBufferWidth = width;
	mBufferHeight = height;

	glViewport(0, 0, mBufferWidth, mBufferHeight);
}

void Window::HandleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (0 <= key && key < 512) {
		if (action == GLFW_PRESS) {
			theWindow->mKeys[key] = true;
			// SPDLOG_INFO("Pressed {}", key);
		}
		else if (action == GLFW_RELEASE) {
			theWindow->mKeys[key] = false;
			// SPDLOG_INFO("Released {}", key);
		}
	}
}

void Window::HandleMousePos(GLFWwindow* window, double xPos, double yPos) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mMouseControl == false) {
		theWindow->mXChange = 0.f;
		theWindow->mYChange = 0.f;

		theWindow->mLastX = static_cast<GLfloat>(xPos);
		theWindow->mLastY = static_cast<GLfloat>(yPos);
		return;
	}

	if (theWindow->mMouseFirstMoved) {
		theWindow->mLastX = static_cast<GLfloat>(xPos);
		theWindow->mLastY = static_cast<GLfloat>(yPos);
		theWindow->mMouseFirstMoved = false;
	}

	theWindow->mXChange = static_cast<GLfloat>(xPos - theWindow->mLastX);
	theWindow->mYChange = static_cast<GLfloat>(theWindow->mLastY - yPos);

	theWindow->mLastX = static_cast<GLfloat>(xPos);
	theWindow->mLastY = static_cast<GLfloat>(yPos);

	// printf("x: %.6f, y: %.6f\n", theWindow->mXChange, theWindow->mYChange);
}


void Window::HandleOnMouseButton(GLFWwindow* window, int button, int action, int modifier) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	
	theWindow->MouseButton(button, action);

}

void Window::MouseButton(int button, int action) {
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			mMouseControl = true;
		}
		else if (action == GLFW_RELEASE) {
			mMouseControl = false;
		}
	}
}

void Window::HandleScroll(GLFWwindow* window, double xOffset, double yOffset) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mFOV >= 1.0f && theWindow->mFOV <= 60.0f) {
		theWindow->mFOV -= static_cast<GLfloat>(yOffset);
	}
		
	if (theWindow->mFOV <= 1.0f) {
		theWindow->mFOV = 1.0f;
	}

	if (theWindow->mFOV >= 60.0f) {
		theWindow->mFOV = 60.0f;
	}
		
}

void Window::HandleWindowSize(GLFWwindow* window, GLint width, GLint height) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	theWindow->Reshape(width, height);
}

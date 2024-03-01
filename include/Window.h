#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <memory>
#include <utility>

#include "Common.h"

class Window {
	static void HandleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void HandleMousePos(GLFWwindow* window, double xPos, double yPos);
	static void HandleOnMouseButton(GLFWwindow* window, int button, int action, int modifier);
	static void HandleScroll(GLFWwindow* window, double xOffset, double yOffset);
	static void HandleWindowSize(GLFWwindow* window, GLint width, GLint height);
	
public:
	Window();
	Window(const GLint windowWidth, const GLint windowHeight, GLfloat fov);
	virtual ~Window();

	int Initialise();

	bool GetShouldClose();
	void SwapBuffers();

	GLint GetBufferWidth() const;
	GLint GetBufferHeight() const;

	bool* GetKeys();
	GLfloat GetXChange();
	GLfloat GetYChange();
	GLfloat GetFOV() const;
	bool GetMouseControl() const;

private:
	GLFWwindow* mWindow;

	// Window dimensions
	GLint mWidth, mHeight;
	GLint mBufferWidth, mBufferHeight;

	// FOV: Field Of View
	GLfloat mFOV;

	bool mKeys[512];
	GLfloat mLastX, mLastY, mXChange, mYChange;
	bool mMouseFirstMoved;
	bool mMouseControl;

	void CreateCallbacks();

	void Reshape(GLint width, GLint height);

	void MouseButton(int button, int action);
};
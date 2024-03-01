#include "include/Graphic.h"

const int Graphic::step = 100;
const float Graphic::angle = glm::pi<float>() * 2.f / step;

std::unique_ptr<Graphic> Graphic::instance = nullptr;

std::unique_ptr<Graphic> Graphic::GetInstance()
{
	if (instance == nullptr) {
		instance = std::make_unique<Graphic>();
		SPDLOG_INFO("A Graphic instance is created");
		return std::move(instance);
	}
	return std::move(instance);
}

Graphic::Graphic()
	: mShader(nullptr)
	, mVAOs()
	, mVBOs()
	, mProjection(glm::mat4(1.f))
{
	mShader = new SimpleShader();
	mShader->CreateFromFiles("../Shaders/SimpleShader.vert", "../Shaders/SimpleShader.frag");


	mVAOs[0] = 0;
	mVAOs[1] = 0;

	mVBOs[0] = 0;
	mVBOs[0] = 0;
}

Graphic::~Graphic()
{
	delete mShader; mShader = nullptr;
}

void Graphic::SetProjection(const GLint width, const GLint height)
{
	const float half_width = width / 2.f;
	const float half_height = height / 2.f;
	mProjection = glm::ortho(-half_width, half_width, -half_height, half_height, 0.0f, 1.0f);
}

void Graphic::Initialize(const GLint width, const GLint height)
{
	SetProjection(width, height);
	CreatePolygons(width, height);
}

void Graphic::CreateCircle()
{
	mCircleVertexNum = (step + 1) * 9;
	
	float*  vertices = new float[mCircleVertexNum];

	float xPos = 0; float yPos = 0; float radius = 1.f;

	float prevX = xPos;
	float prevY = yPos + radius;

	for (int i = 0; i <= step; ++i) {
		float newX = radius * sin(angle * i);
		float newY = -radius * cos(angle * i);

		vertices[(i * 9) + 0] = 0.0f;
		vertices[(i * 9) + 1] = 0.0f;
		vertices[(i * 9) + 2] = 0.0f;

		vertices[(i * 9) + 3] = prevX;
		vertices[(i * 9) + 4] = prevY;
		vertices[(i * 9) + 5] = 0.0f;

		vertices[(i * 9) + 6] = newX;
		vertices[(i * 9) + 7] = newY;
		vertices[(i * 9) + 8] = 0.0f;

		prevX = newX;
		prevY = newY;
	}

	glGenVertexArrays(1, &mVAOs[0]);
	glGenBuffers(1, &mVBOs[0]);

	glBindVertexArray(mVAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mCircleVertexNum, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	delete[] vertices;
}


void Graphic::CreateRectangle(const GLint width, const GLint height)
{
	float ratio = width / (float)height;

	mRectangleVertexNum = 24;
	float* vertices = new float[mRectangleVertexNum];
	vertices[3 * 0 + 0] = -1.f;
	vertices[3 * 0 + 1] = 1.f / ratio;
	vertices[3 * 0 + 2] = 0.f;

	vertices[3 * 1 + 0] = 1.f;
	vertices[3 * 1 + 1] = 1.f / ratio;
	vertices[3 * 1 + 2] = 0.f;

	//
	vertices[3 * 2 + 0] = 1.f;
	vertices[3 * 2 + 1] = 1.f / ratio;
	vertices[3 * 2 + 2] = 0.f;

	vertices[3 * 3 + 0] = 1.f;
	vertices[3 * 3 + 1] = -1.f / ratio;
	vertices[3 * 3 + 2] = 0.f;

	//
	vertices[3 * 4 + 0] = 1.f;
	vertices[3 * 4 + 1] = -1.f / ratio;
	vertices[3 * 4 + 2] = 0.f;

	vertices[3 * 5 + 0] = -1.0f;
	vertices[3 * 5 + 1] = -1.0f / ratio;
	vertices[3 * 5 + 2] = 0.f;

	//
	vertices[3 * 6 + 0] = -1.0f;
	vertices[3 * 6 + 1] = -1.0f / ratio;
	vertices[3 * 6 + 2] = 0.f;

	vertices[3 * 7 + 0] = -1.f;
	vertices[3 * 7 + 1] = 1.f / ratio;
	vertices[3 * 7 + 2] = 0.f;


	glGenVertexArrays(1, &mVAOs[1]);
	glGenBuffers(1, &mVBOs[1]);

	glBindVertexArray(mVAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, mVBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mRectangleVertexNum, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	delete[] vertices;
}

void Graphic::CreatePolygons(const GLint width, const GLint height)
{
	CreateCircle();
	CreateRectangle(width, height);
}
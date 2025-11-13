#include "Scene.h"
#include "Framework/SceneElements/Cube.h"
#include <AssetManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Scene::Scene(OpenGLWindow * window) :
	m_window(window)
{
	assert(window != nullptr);
}

Scene::~Scene() {}

bool Scene::init()
{
	try
	{
		// Shader laden
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram(
			"assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
		m_shader->use();

		// Tiefentest und Backface Culling
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);

		// Geometrie des Würfels laden
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);

		std::cout << "Scene initialization done\n";
		return true;
	}
	catch (std::exception& ex)
	{
		throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

// Hilfsfunktion zum Zeichnen eines Teils
void Scene::drawPart(const glm::mat4& parentMat, Transform& part)
{
	glm::mat4 model = parentMat * part.getTransformMatrix();
	m_shader->setUniform("model", model, false);
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Scene::render(float dt)
{
	// --------------------- Frame vorbereiten ---------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	float currentFrame = glfwGetTime();
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;


	// Kamera & Projektion
	glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 9), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 projection = glm::perspective(glm::radians(80.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	m_shader->use();
	m_shader->setUniform("view", view, false);
	m_shader->setUniform("projection", projection, false);

	// --------------------- Transforms zurücksetzen ---------------------
	rootTrans = Transform();
	robotTrans = Transform();
	bodyTrans = Transform();
	headTrans = Transform();
	leftUpperArmTrans = Transform();
	rightUpperArmTrans = Transform();
	leftLowerArmTrans = Transform();
	rightLowerArmTrans = Transform();
	leftLegTrans = Transform();
	rightLegTrans = Transform();

	// --------------------- ROOT ---------------------
	// Hier könnten wir die gesamte Szene oder den Roboter bewegen/rotieren
	rootTrans.rotate(glm::vec3(0.0f, currentFrame * 0.3f, 0.0f)); // langsame Rotation der ganzen Szene

	// --------------------- ROBOT (lokal) ---------------------
	robotTrans.translate(glm::vec3(0.0f, 0.0f, 0.0f)); // Position im Root-Koordinatensystem
	glm::mat4 robotMat = rootTrans.getTransformMatrix() * robotTrans.getTransformMatrix();

	// --------------------- RUMPF ---------------------
	bodyTrans.translate(glm::vec3(0.0f, 0.0f, 0.0f));    // Rumpf mittig im lokalen Robot-System
	bodyTrans.scale(glm::vec3(3.0f, 3.0f, 2.0f));        // nur Rumpf dicker / größer
	drawPart(robotMat, bodyTrans);

	// --------------------- KOPF ---------------------
	headTrans.translate(glm::vec3(0.0f, 2.5f, 0.0f));    // Kopf über dem Rumpf
	headTrans.scale(glm::vec3(1.0f, 1.0f, 1.0f));
	drawPart(robotMat, headTrans);

	// --------------------- LINKER OBERARM ---------------------
	// Linker Oberarm
	leftUpperArmTrans.translate(glm::vec3(-2.0f, 0.6f, 0.0f));  // Position des Oberarms (Schulter)
	leftUpperArmTrans.rotateAroundPoint(glm::vec3(0.0f, 0.6f, 0.0f),  // Rotation um die Schulter
		glm::vec3(glm::sin(currentFrame) * 0.25f, 0.0f, 0.0f));  // Rotation des Oberarms nach vorne
	leftUpperArmTrans.scale(glm::vec3(0.5f, 1.2f, 0.6f));  // Skalierung des Oberarms
	drawPart(robotMat, leftUpperArmTrans);

	// Linker Unterarm: Jetzt folgt der Unterarm der Oberarmrotation
	leftLowerArmTrans.translate(glm::vec3(0.0f, -1.0f, 0.0f));  // Position des Unterarms
	leftLowerArmTrans.rotateAroundPoint(glm::vec3(0.0f, -0.6f, 0.0f),  // Ellbogen
		glm::vec3(glm::sin(currentFrame) * 0.18f, 0.0f, 0.0f));  // Gleiche Rotation wie Oberarm
	leftLowerArmTrans.scale(glm::vec3(0.9f, 0.9f, 0.6f));  // Skalierung des Unterarms
	drawPart(robotMat * leftUpperArmTrans.getTransformMatrix(), leftLowerArmTrans);


	// --------------------- RECHTER OBERARM ---------------------
	rightUpperArmTrans.translate(glm::vec3(2.0f, 0.6f, 0.0f));  // Position des rechten Oberarms
	rightUpperArmTrans.rotateAroundPoint(glm::vec3(0.0f, 0.6f, 0.0f),  // Rotation um die Schulter
		glm::vec3(-glm::sin(currentFrame) * 0.25f, 0.0f, 0.0f));  // Rotation des rechten Oberarms
	rightUpperArmTrans.scale(glm::vec3(0.5f, 1.2f, 0.6f));  // Skalierung des Oberarms
	drawPart(robotMat, rightUpperArmTrans);


	rightLowerArmTrans.translate(glm::vec3(0.0f, -1.0f, 0.0f));  // Position des rechten Unterarms (relativ zum Oberarm)
	rightLowerArmTrans.rotateAroundPoint(glm::vec3(0.0f, -0.6f, 0.0f),  // Ellbogen (relativ zum Oberarm)
		glm::vec3(-glm::sin(currentFrame) * 0.18f, 0.0f, 0.0f));  // Rotation des Unterarms
	rightLowerArmTrans.scale(glm::vec3(0.9f, 0.9f, 0.6f));  // Skalierung des Unterarms
	drawPart(robotMat * rightUpperArmTrans.getTransformMatrix(), rightLowerArmTrans);


	// --------------------- LINKES BEIN ---------------------
	leftLegTrans.translate(glm::vec3(-1.0f, -2.2f, 0.0f));
	leftLegTrans.rotateAroundPoint(glm::vec3(-0.5f, -1.125f, 0.0f),
		glm::vec3(glm::sin(currentFrame) * 0.2f, 0.0f, 0.0f));
	leftLegTrans.scale(glm::vec3(0.5f, 2.2f, 0.9f));
	drawPart(robotMat, leftLegTrans);

	// --------------------- RECHTES BEIN ---------------------
	rightLegTrans.translate(glm::vec3(1.0f, -2.2f, 0.0f));
	rightLegTrans.rotateAroundPoint(glm::vec3(0.5f, -1.125f, 0.0f),
		glm::vec3(-glm::sin(currentFrame) * 0.2f, 0.0f, 0.0f));
	rightLegTrans.scale(glm::vec3(0.5f, 2.2f, 0.9f));
	drawPart(robotMat, rightLegTrans);
}


void Scene::update(float dt)
{

}

OpenGLWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{

}

void Scene::onMouseMove(MousePosition mouseposition)
{

}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

void Scene::onMouseScroll(double xscroll, double yscroll)
{

}

void Scene::onFrameBufferResize(int width, int height)
{

}
void Scene::shutdown()
{

}

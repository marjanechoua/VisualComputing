#include "Scene.h"
#include "Framework/SceneElements/Cube.h"
#include <AssetManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Scene::Scene(OpenGLWindow * window) :
	m_window(window)
{
	assert(window != nullptr);

}

Scene::~Scene()
{}

bool Scene::init()
{
	try
	{
		// Shader laden
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
		m_shader->use();

		// 2.1 - Tiefentest aktivieren
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);        // Tiefentest aktivieren
		glDepthFunc(GL_LESS);           // Standard: nächstes Pixel kleinerer Tiefe gewinnt
		glEnable(GL_CULL_FACE);         // Rückseiten ausblenden
		glCullFace(GL_BACK);            // Rückseite definieren
		glFrontFace(GL_CCW);            // Frontface definieren

		// 2.1 - Geometrie des Würfels laden
		// a) VBO erzeugen, binden und Daten hochladen
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);  // cubeVert aus Cube.h

		// b) VAO erzeugen und binden
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// c) Vertex-Attribute definieren und aktivieren
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);         // Position
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Farbe (R,G)
		glEnableVertexAttribArray(1);

		// d) Indexbuffer erzeugen und binden
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW); // cubeInd aus Cube.h

		// 2.2 - Transformationsobjekt für den Würfel erstellen
		// Rotations-Transformation für den Würfel
		cubeTrans = Transform();

		// 2.3.1 - Szenegraph: Initialsieren der Transformationsgruppen für den Roboter
		bodyTrans = Transform();
		headTrans = Transform();
		bodyPos=Transform();



		leftLegTrans = Transform();
		rightLegTrans = Transform();

		leftUpperArmTrans = Transform();
		rightUpperArmTrans = Transform();

		leftLowerArmTrans = Transform();
		rightLowerArmTrans = Transform();



        std::cout << "Scene initialization done\n";
        return true;
	}
	catch (std::exception& ex)
	{
	    throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

void Scene::render(float dt)
{
   // 2.2.3 Hintergrund löschen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Löscht Farbpuffer und Tiefenpuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Setzt die Hintergrundfarbe auf Schwarz

    // Berechnung der Zeitdifferenz zwischen den Frames
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // 2.2 - Kamera (view) und Projektion definieren
    glm::mat4 view = glm::lookAt(glm::vec3(8, 8,15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Kameraposition und Blickrichtung
    glm::mat4 projection = glm::perspective(glm::radians(80.0f), 800.0f / 600.0f, 0.1f, 100.0f); // Perspektivische Projektion

    // 2.2.1 Rotation des Würfels
    cubeTrans.rotate(glm::vec3(0, 0 , glm::radians(20.0f) * dt)); // Würfel kontinuierlich rotieren (dt berücksichtigen)

    // 2.2.2 Transformation in den Shader setzen
    m_shader->use(); // Den Shader einmal pro Frame verwenden
    m_shader->setUniform("view", view, false); // Kamera-Transformation
    m_shader->setUniform("projection", projection, false); // Projektion

    // 2.1 - Geometrie des Würfels zeichnen
    /*m_shader->setUniform("model", cubeTrans.getTransformMatrix(), false); // Setze Würfel-Transformation
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  // Zeichne den Würfel
    glBindVertexArray(0);*/


// 2.3 - Roboter erstellen

glm::mat4 robotTrans;

// ----- Rumpf -----
bodyTrans = Transform();
bodyTrans.translate(glm::vec3(0.0f, 2.0f, 0.0f));       // Mittig platziert
bodyTrans.scale(glm::vec3(2.0f, 2.2f, 1.0f));           // Hoch und rechteckig
robotTrans = bodyTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

// ----- Kopf -----
headTrans = Transform();
headTrans.translate(glm::vec3(0.0f, 2.0f, 0.0f));       // Direkt auf dem Körper
headTrans.scale(glm::vec3(1.0f, 0.6f, 1.0f));           // Etwas kleiner als der Körper
robotTrans = bodyTrans.getTransformMatrix() * headTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

// ----- Linker Oberarm -----
leftUpperArmTrans = Transform();
leftUpperArmTrans.translate(glm::vec3(-1.7f, 1.2f, 0.0f)); // direkt an der Schulter
leftUpperArmTrans.rotateAroundPoint(glm::vec3(0.0f), glm::vec3(glm::sin(currentFrame)*0.25f, 0.0f, 0.0f));
leftUpperArmTrans.scale(glm::vec3(0.5f, 0.8f, 0.5f));      // schlank, rechteckig
robotTrans = bodyTrans.getTransformMatrix() * leftUpperArmTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

// ----- Linker Unterarm -----
leftLowerArmTrans = Transform();
leftLowerArmTrans.translate(glm::vec3(0.0f, -1.5f, 0.0f)); // direkt anschließen
leftLowerArmTrans.rotate(glm::vec3(glm::sin(currentFrame*1.3f)*0.25f, 0.0f, 0.0f));
leftLowerArmTrans.scale(glm::vec3(0.5f, 0.8f, 0.5f));      // gleich groß wie Oberarm
robotTrans = bodyTrans.getTransformMatrix() * leftUpperArmTrans.getTransformMatrix() * leftLowerArmTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

// ----- Rechter Oberarm -----
rightUpperArmTrans = Transform();
rightUpperArmTrans.translate(glm::vec3(1.7f, 1.2f, 0.0f));
rightUpperArmTrans.rotateAroundPoint(glm::vec3(0.0f), glm::vec3(-glm::sin(currentFrame)*0.25f, 0.0f, 0.0f));
rightUpperArmTrans.scale(glm::vec3(0.5f, 0.8f, 0.5f));
robotTrans = bodyTrans.getTransformMatrix() * rightUpperArmTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

// ----- Rechter Unterarm -----
rightLowerArmTrans = Transform();
rightLowerArmTrans.translate(glm::vec3(0.0f, -1.5f, 0.0f)); // direkt anschließen
rightLowerArmTrans.rotate(glm::vec3(-glm::sin(currentFrame*1.3f)*0.25f, 0.0f, 0.0f));
rightLowerArmTrans.scale(glm::vec3(0.5f, 0.8f, 0.5f));      // gleich groß wie Oberarm
robotTrans = bodyTrans.getTransformMatrix() * rightUpperArmTrans.getTransformMatrix() * rightLowerArmTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

// ----- Linkes Bein -----
leftLegTrans = Transform();
leftLegTrans.translate(glm::vec3(-0.6f, -1.8f, 0.0f)); // eng unter Körper
leftLegTrans.rotateAroundPoint(glm::vec3(0.0f), glm::vec3(glm::sin(currentFrame)*0.2f, 0.0f, 0.0f));
leftLegTrans.scale(glm::vec3(0.6f, 2.0f, 0.6f));       // etwas länger als Arme
robotTrans = bodyTrans.getTransformMatrix() * leftLegTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

// ----- Rechtes Bein -----
rightLegTrans = Transform();
rightLegTrans.translate(glm::vec3(0.6f, -1.8f, 0.0f));
rightLegTrans.rotateAroundPoint(glm::vec3(0.0f), glm::vec3(-glm::sin(currentFrame)*0.2f, 0.0f, 0.0f));
rightLegTrans.scale(glm::vec3(0.6f, 2.0f, 0.6f));
robotTrans = bodyTrans.getTransformMatrix() * rightLegTrans.getTransformMatrix();
m_shader->setUniform("model", robotTrans, false);
glBindVertexArray(m_vao);
glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);




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

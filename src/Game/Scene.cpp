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
        // Hier wird der Shader aus den Dateien geladen und in der Asset-Verwaltung gespeichert
        m_assets.addShaderProgram("shader", AssetManager::createShaderProgram(
            "assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));

        // Das Shader-Programm wird abgerufen und aktiviert
        m_shader = m_assets.getShaderProgram("shader");
        m_shader->use(); // Shader für die folgenden Renderoperationen aktivieren

        // Tiefentest aktivieren, um sicherzustellen, dass nur die Vorderseite eines Objekts sichtbar ist
        glClearDepth(0.0f); // Setzt den Wert für den Tiefenpuffer
        glEnable(GL_DEPTH_TEST); // Tiefentest aktivieren
        glDepthFunc(GL_GREATER); // Nur Objekte, die näher sind, werden angezeigt
        glEnable(GL_CULL_FACE); // Culling aktivieren, um Rückseiten von Dreiecken zu ignorieren
        glCullFace(GL_BACK); // Rückseiten von Objekten werden entfernt
        glFrontFace(GL_CCW); // Legt fest, dass die Vorderseite eines Objekts im Uhrzeigersinn ist (CCW = Counter Clock Wise)

        // Geometrie des Würfels laden (Vertex-Daten)
        glGenBuffers(1, &m_vbo); // Ein Vertex-Buffer-Objekt (VBO) erzeugen
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo); // VBO an das OpenGL-Kontext binden
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW); // Daten für das VBO übergeben

        // Ein Vertex-Array-Objekt (VAO) erzeugen
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao); // VAO binden

        // Die Vertex-Daten an OpenGL übergeben (Position und Farbe)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position der Eckpunkte
        glEnableVertexAttribArray(0); // Aktiviert das Attribut für die Position

        // Farbwerte für die Eckpunkte an OpenGL übergeben
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1); // Aktiviert das Attribut für die Farbe

        // Indices für das Element-Array-Buffer (EBO) laden
        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo); // EBO binden
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW); // Indizes für die Würfelflächen

        std::cout << "Scene initialization done\n";
        return true; // Rückgabe, dass die Initialisierung erfolgreich war
    }
    catch (std::exception& ex) // Fehlerbehandlung
    {
        throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
    }
}

// Hilfsfunktion zum Zeichnen eines Teils der Szene
void Scene::drawPart(const glm::mat4& parentMat, Transform& part)
{
    // Berechne die endgültige Modellmatrix für dieses Teil
    glm::mat4 model = parentMat * part.getTransformMatrix();
    m_shader->setUniform("model", model, false); // Setze die Modellmatrix im Shader

    // Gebe das Modell zum Rendern an OpenGL weiter
    glBindVertexArray(m_vao); // VAO binden, um es für das Rendering zu verwenden
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Zeichne 36 Indizes des Würfels (als Dreiecke)
    glBindVertexArray(0); // Entbinde das VAO
}

// Funktion zum Rendern der Szene (wird pro Frame aufgerufen)
void Scene::render(float dt)
{
    // --------------------- Frame vorbereiten ---------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Bildschirm löschen (Farben + Tiefe)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Hintergrundfarbe festlegen (schwarz)

    // Zeit- und Delta-Time Berechnungen für Animationen
    float currentFrame = glfwGetTime(); // Hole die aktuelle Zeit (für Animationen)
    float deltaTime = currentFrame - lastFrame; // Berechne die Zeitdifferenz zum letzten Frame
    lastFrame = currentFrame; // Setze das letzte Frame auf die aktuelle Zeit



    // --------------------- Kamera & Projektion ---------------------
    //glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 9), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Blickwinkel
    //glm::mat4 projection = glm::perspective(glm::radians(80.0f), 800.0f / 600.0f, 0.1f, 100.0f); // Perspektive

    // Setze die Kamera- und Projektionsmatrix im Shader
    m_shader->use();
    //m_shader->setUniform("view", view, false);
    //m_shader->setUniform("projection", projection, false);
    // Setze die Zeit-Uniform im Shader für Farbanimationen

    m_shader->setUniform("time", currentFrame);  // Zeit an den Shader übergeben

    // --------------------- Transforms zurücksetzen ---------------------
    // Initialisieren der Transform-Objekte für den Roboter
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
    // Die gesamte Szene oder der Roboter wird hier rotiert (hier wird die Szene gedreht)
    rootTrans.rotate(glm::vec3(0.0f, currentFrame * 0.3f, 0.0f)); // Langsame Rotation der gesamten Szene um die y-Achse

    // --------------------- ROBOT (lokal) ---------------------
    // Der Roboter wird in das Root-Koordinatensystem bewegt
    robotTrans.translate(glm::vec3(0.0f, 0.0f, 0.0f)); // Position im Root-Koordinatensystem
    robotTrans.scale(glm::vec3(0.2f));
    glm::mat4 robotMat = rootTrans.getTransformMatrix() * robotTrans.getTransformMatrix();

    // --------------------- RUMPF ---------------------
    // Der Körper des Roboters wird mit einer Position und Skalierung versehen
    bodyTrans.translate(glm::vec3(0.0f, 0.0f, 0.0f)); // Rumpf in der Mitte
    bodyTrans.scale(glm::vec3(3.0f, 3.0f, 2.0f)); // Der Körper wird größer und dicker
    drawPart(robotMat, bodyTrans); // Zeichne den Körper des Roboters

    // --------------------- KOPF ---------------------
    // Kopf des Roboters wird über dem Körper positioniert
    headTrans.translate(glm::vec3(0.0f, 2.5f, 0.0f)); // Kopf über dem Rumpf
    headTrans.scale(glm::vec3(1.0f, 1.0f, 1.0f)); // Standard-Skalierung für den Kopf
    drawPart(robotMat, headTrans); // Zeichne den Kopf

    // --------------------- LINKER OBERARM ---------------------
    // Hier wird der linke Oberarm animiert (Rotationen und Position)
    leftUpperArmTrans.translate(glm::vec3(-2.0f, 0.6f, 0.0f)); // Position des Oberarms
    leftUpperArmTrans.rotateAroundPoint(glm::vec3(0.0f, 0.6f, 0.0f), glm::vec3(glm::sin(currentFrame) * 0.25f, 0.0f, 0.0f)); // Rotation des Oberarms
    leftUpperArmTrans.scale(glm::vec3(0.5f, 1.2f, 0.6f)); // Skalierung des Oberarms
    drawPart(robotMat, leftUpperArmTrans); // Zeichne den linken Oberarm

    // Linker Unterarm: Folgen der Oberarmrotation
    leftLowerArmTrans.translate(glm::vec3(0.0f, -1.0f, 0.0f)); // Unterarmposition
    leftLowerArmTrans.rotateAroundPoint(glm::vec3(0.0f, -0.6f, 0.0f), glm::vec3(glm::sin(currentFrame) * 0.18f, 0.0f, 0.0f)); // Gleiche Rotation wie Oberarm
    leftLowerArmTrans.scale(glm::vec3(0.9f, 0.9f, 0.6f)); // Skalierung des Unterarms
    drawPart(robotMat * leftUpperArmTrans.getTransformMatrix(), leftLowerArmTrans); // Zeichne den Unterarm

    // --------------------- RECHTER OBERARM ---------------------
    // Hier wird der rechte Oberarm positioniert und animiert
    rightUpperArmTrans.translate(glm::vec3(2.0f, 0.6f, 0.0f)); // Position des rechten Oberarms
    rightUpperArmTrans.rotateAroundPoint(glm::vec3(0.0f, 0.6f, 0.0f), glm::vec3(-glm::sin(currentFrame) * 0.25f, 0.0f, 0.0f)); // Rotation des rechten Oberarms
    rightUpperArmTrans.scale(glm::vec3(0.5f, 1.2f, 0.6f)); // Skalierung des Oberarms
    drawPart(robotMat, rightUpperArmTrans); // Zeichne den rechten Oberarm

    // rechter Unterarm: Der Unterarm folgt der Oberarmrotation
    rightLowerArmTrans.translate(glm::vec3(0.0f, -1.0f, 0.0f)); // Position des Unterarms
    rightLowerArmTrans.rotateAroundPoint(glm::vec3(0.0f, -0.6f, 0.0f), glm::vec3(-glm::sin(currentFrame) * 0.18f, 0.0f, 0.0f)); // Gleiche Rotation wie Oberarm
    rightLowerArmTrans.scale(glm::vec3(0.9f, 0.9f, 0.6f)); // Skalierung des Unterarms
    drawPart(robotMat * rightUpperArmTrans.getTransformMatrix(), rightLowerArmTrans); // Zeichne den rechten Unterarm

    // --------------------- LINKES BEIN ---------------------
    leftLegTrans.translate(glm::vec3(-1.0f, -2.2f, 0.0f)); // Position des linken Beins
    leftLegTrans.rotateAroundPoint(glm::vec3(-0.5f, -1.125f, 0.0f), glm::vec3(glm::sin(currentFrame) * 0.2f, 0.0f, 0.0f)); // Rotation des linken Beins
    leftLegTrans.scale(glm::vec3(0.5f, 2.2f, 0.9f)); // Skalierung des linken Beins
    drawPart(robotMat, leftLegTrans); // Zeichne das linke Bein

    // --------------------- RECHTES BEIN ---------------------
    rightLegTrans.translate(glm::vec3(1.0f, -2.2f, 0.0f)); // Position des rechten Beins
    rightLegTrans.rotateAroundPoint(glm::vec3(0.5f, -1.125f, 0.0f), glm::vec3(-glm::sin(currentFrame) * 0.2f, 0.0f, 0.0f)); // Rotation des rechten Beins
    rightLegTrans.scale(glm::vec3(0.5f, 2.2f, 0.9f)); // Skalierung des rechten Beins
    drawPart(robotMat, rightLegTrans); // Zeichne das rechte Bein
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

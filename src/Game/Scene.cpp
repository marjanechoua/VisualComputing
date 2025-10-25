#include "Scene.h"
#include <AssetManager.h>

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
		//Load shader
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");
        m_shader->use();

        float vertices[] = {-0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, 0.5, 0.0, 1.0, 0.0,
                            0.0, 1.0, 1.0, 0.0, 0.0,
                            -0.5, 0.5, 0.0, 1.0, 0.0};

        int indices[] = {0, 1, 2,
                         0, 2, 4,
                         4, 2, 3};

		/* Praktikum 1 */
		// 1. VAO erstellen und binden
		GLuint vaoID;
		glGenVertexArrays(1, &vaoID);
		glBindVertexArray(vaoID);
		m_vao = vaoID; // falls du es als Member speichern willst

		// 2. VBO erstellen, binden und Daten hochladen
		GLuint vboID;
		glGenBuffers(1, &vboID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		m_vbo = vboID; // Member speichern

		// 3. Vertex-Attribute definieren und aktivieren
		// Attribut 0 = Position (x, y, z)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Attribut 1 = Farbe (r, g)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// 4. Index Buffer Object (IBO/EBO) erstellen und binden
		GLuint iboID;
		glGenBuffers(1, &iboID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		m_ibo = iboID; // Member speichern

		// 5. Optional: Alles lösen, damit wir nichts versehentlich ändern
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




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

    /* Praktikum1 */
	// Shader aktivieren
	m_shader->use();

	// VAO binden
	glBindVertexArray(m_vao);

	// Elemente zeichnen (Index Array)
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

	// Optional: VAO lösen
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

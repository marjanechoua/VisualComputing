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

		// Jeder Vertex hat 5 Werte, 2 für Position x,y und 3 für Farbe r,g,b
        float vertices[] = {-0.5, -0.5, 0.0, 0.0, 1.0, // Vertex 0
                            0.5, -0.5, 0.0, 0.0, 1.0, // Vertex 1
                            0.5, 0.5, 0.0, 1.0, 0.0, // Vertex 2
                            0.0, 1.0, 1.0, 0.0, 0.0, // Vertex 3
                            -0.5, 0.5, 0.0, 1.0, 0.0}; //Vertex 4

        int indices[] = {0, 1, 2, // Erstes Dreieck: Vertex 0, 1, 2
                         0, 2, 4, // zweites Dreiecck: Vertex 0,2,4
                         4, 2, 3}; // drittes Dreieck: vertrex 4,2,3

		/* Praktikum 1 */
		// a) VBO erzeugen, binden und Daten hochladen ( VBO ist quasi ein Container mit Rohdaten)
		glGenBuffers(1, &m_vbo); // machen Speicher
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Rohdaten kommen auf die GPU

		// b) VAO erzeugen und binden ( sagt wie man die Rohdaten im VBO liest)
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// c) Vertex-Attribute definieren und aktivieren ( VertexAttribPointer sagt, welche Werte im Vertex für Position/Farbe(etc. sind)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// d) Indexbuffer erzeugen und binden ( IBO sagt welche Vertices zusammen ein Dreieck bilden)
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// e) Optional: alles lösen
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

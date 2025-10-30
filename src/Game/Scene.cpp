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

		/*// Teil 1 des Praktikums
		// Jeder Vertex hat 5 Werte, 2 für Position x,y und 3 für Farbe r,g,b
        float vertices[] = {-0.5, -0.5, 0.0, 0.0, 1.0, // Vertex 0
                            0.5, -0.5, 0.0, 0.0, 1.0, // Vertex 1
                            0.5, 0.5, 0.0, 1.0, 0.0, // Vertex 2
                            0.0, 1.0, 1.0, 0.0, 0.0, // Vertex 3
                            -0.5, 0.5, 0.0, 1.0, 0.0}; //Vertex 4

        int indices[] = {0, 1, 2, // Erstes Dreieck: Vertex 0, 1, 2
                         0, 2, 4, // zweites Dreiecck: Vertex 0,2,4
                         4, 2, 3}; // drittes Dreieck: vertrex 4,2,3



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

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// d) Indexbuffer erzeugen und binden ( IBO sagt welche Vertices zusammen ein Dreieck bilden)
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// e) Optional: alles lösen
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		*/
     // zweiter Teil von Praktikum 1

		float vertices[] = {
			// Buchstabe M (alle X-Werte um -1.0 verschoben)
			// Left vertical (CCW)
			-1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left 0
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left 1
			-0.9f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-right 2
			-0.9f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right 3

			// Left diagonal (CCW)
			-0.9f, 1.0f, 0.0f, 0.0f, 1.0f,  // bottom-left (from previous rectangle) 4
			-0.75f, 0.5f, 0.0f, 0.0f, 1.0f,  // top-left (middle peak) 5
			-0.65f, 0.5f, 0.0f, 0.0f, 1.0f,  // top-right 6
			-0.75f, 1.0f, 0.0f, 0.0f, 1.0f,  // bottom-right (repeat) 7

			// Right diagonal (CCW)
			-0.65f, 0.5f, 0.0f, 0.0f, 1.0f,  // bottom-left (middle peak) 8
			-0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left 9
			-0.4f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-right 10
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,  // bottom-right 11

			// Right vertical (CCW)
			-0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left 12
			-0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-left 13
			-0.4f, 1.0f, 0.0f, 0.0f, 1.0f,  // top-right 14
			-0.4f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right 15

			// Buchstabe C - Vertices verschoben um +0.8 auf der X-Achse
	// links vertikal
	0.2f, 0.0f, 0.0f, 0.0f, 1.0f,  //16
	0.2f, 0.7f, 0.0f, 0.0f, 1.0f,  //17
	0.4f, 0.7f, 0.0f, 0.0f, 1.0f,  //18
	0.4f, 0.0f, 0.0f, 0.0f, 1.0f,  //19

	// oben horizontal
	0.4f, 0.5f, 0.0f, 0.0f, 1.0f,  //20
	0.8f, 0.5f, 0.0f, 0.0f, 1.0f,  //21
	0.8f, 0.7f, 0.0f, 0.0f, 1.0f,  //22
	0.4f, 0.7f, 0.0f, 0.0f, 1.0f,  //23

	// unten horizontal
	0.4f, 0.0f, 0.0f, 0.0f, 1.0f,  //24
	0.8f, 0.0f, 0.0f, 0.0f, 1.0f,  //25
	0.8f, 0.1f, 0.0f, 0.0f, 1.0f,  //26
	0.4f, 0.1f, 0.0f, 0.0f, 1.0f   //27
		};


		 int indices[] = {
		 	 //Buchstabe M
		 	0, 2, 1, 0, 3, 2,  // Correct winding order for left block

// Left diagonal (CCW)
		 	// Left diagonal (CCW)
			 4, 5, 6,  // erstes Dreieck
			 6,7,4,   // zweites Dreieck
			   // Correct winding order for left diagonal block

// Right diagonal (CCW)
8, 9, 10, 8, 11, 9,  // Correct winding order for right diagonal block

// Right vertical (CCW)
12, 14, 13, 12, 15, 14 , //


		 	//Buchstabe C
		 	// C - Links vertikal (CCW)
	16, 18, 17, 16, 19, 18,  // links vertikal (2 Dreiecke)

	// C - Oben horizontal (CCW)
	20, 21, 22, 20, 22, 23,  // oben horizontal (2 Dreiecke)

	// C - Unten horizontal (CCW)
	24, 25, 26, 24, 26, 27   // unten horizontal (2 Dreiecke)
		};




        // a) VBO erzeugen, binden und Daten hochladen
        glGenBuffers(1, &m_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // b) VAO erzeugen und binden
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // c) Vertex-Attribute definieren und aktivieren
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);         // Position
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))); // Farbe (R,G)
        glEnableVertexAttribArray(1);

        // d) Indexbuffer erzeugen und binden
        glGenBuffers(1, &m_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// e) Backface culling aktivieren, OpenGL rendert nur die Vorderseiten der Dreiecke,
		// Vorderseite = Dreiecke, deren Vertices gegen den Uhrzeigersinn (CCW) definiert sind,
		// Rückseite = Dreiecke, deren Vertices im Uhrzeigersinn (CW) definiert sind, werden ausgeblendet

		glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        // Alles lösen
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

    /* Praktikum1 Teil 1
	// Shader aktivieren
	m_shader->use();

	// VAO binden
	glBindVertexArray(m_vao);

	// Elemente zeichnen (Index Array)
	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

	// Optional: VAO lösen
	glBindVertexArray(0); */

//  Praktikum 1 Teil 2
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 42*3, GL_UNSIGNED_INT, 0); // 42 Indizes insgesamt
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

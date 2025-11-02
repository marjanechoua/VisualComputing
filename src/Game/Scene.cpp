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
     /* zweiter Teil von Praktikum 1

		float vertices[] = {
			// x, y, r, g, b
			-0.74f, -0.50f,  0.0f, 0.0f, 1.0f, // A 0
			-0.67f, -0.50f,  0.0f, 0.0f, 1.0f, // B 1
			-0.75f,  0.50f,  0.0f, 0.0f, 1.0f, // C 2
			-0.67f,  0.50f,  0.0f, 0.0f, 1.0f, // D 3
			-0.33f, -0.50f,  0.0f, 0.0f, 1.0f, // E 4
			-0.25f, -0.50f,  0.0f, 0.0f, 1.0f, // F 5
			-0.55f,  0.16f,  0.0f, 0.0f, 1.0f, // G 6
			-0.45f,  0.16f,  0.0f, 0.0f, 1.0f, // H 7
			-0.58f,  0.50f,  0.0f, 0.0f, 1.0f, // I 8
			-0.50f,  0.27f,  0.0f, 0.0f, 1.0f, // J 9
			-0.33f,  0.50f,  0.0f, 0.0f, 1.0f, // K 10
			-0.25f,  0.50f,  0.0f, 0.0f, 1.0f, // L 11
			-0.40f,  0.50f,  0.0f, 0.0f, 1.0f , // M 12

			// Buchstabe C
	0.24f, -0.50f,  0.0f, 0.0f, 1.0f, // N 13
	0.34f, -0.50f,  0.0f, 0.0f, 1.0f, // O 14
	0.23f,  0.50f,  0.0f, 0.0f, 1.0f, // P 15
	0.34f,  0.50f,  0.0f, 0.0f, 1.0f, // Q 16
	0.34f, -0.40f,  0.0f, 0.0f, 1.0f, // R 17
	0.83f, -0.40f,  0.0f, 0.0f, 1.0f, // S 18
	0.83f, -0.50f, 0.0f, 0.0f, 1.0f, // T 19
	0.83f,  0.50f,  0.0f, 0.0f, 1.0f, // U 20
	0.83f,  0.40f,  0.0f, 0.0f, 1.0f, // V 21
	0.34f,  0.40f,  0.0f, 0.0f, 1.0f  // W 22
		};

		unsigned int indices[] = {
			2, 0, 1,  // C, A, B
			1, 3,2,  // B, D, C
			3, 6, 9,  // D, G, J
			3, 9, 8,  // J, D, I
			6, 7, 12, // G, H, M
			12, 7, 10,// M, H, K
			10, 4, 5, // K, E, F
			10, 5, 11, // K, F, L

			// --- Buchstabe C ---
	15, 13, 14, // P, N, O
	15, 14, 16, // P, O, Q
	16, 22, 20, // Q, W, U
	22, 21, 20, // W, V, U
	17, 14, 18, // R, O, S
	14, 19, 18  // O, T, S
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
*/

		//Praktikum Nr.2

		// Im Konstruktor der Scene-Klasse
		cubeTrans = new Transform();  // Initialisiert den Zeiger auf ein neues Transform-Objekt


		static const float cubeVert[] =  {0.5, -0.5, -0.5, 1, 0, 0, // rot 0
										  0.5, -0.5, 0.5, 0, 1, 0, //grün 1
										  -0.5, -0.5, 0.5, 0, 0, 1, //blau 2
										  -0.5, -0.5, -0.5, 1, 1, 0, //gelb 3
										  0.5, 0.5, -0.5, 1, 0, 1, // magenta 4
										  0.5, 0.5, 0.5, 0, 1, 1, //cyan 5
										  -0.5, 0.5, 0.5, 1, 1, 1, //weiß 6
										  -0.5, 0.5, -0.5, 0.5, 1, 0.5}; //hellgrün 7
		static const int cubeInd[] = {
			// Unterseite (Y = -0.5)
			0, 2, 1,
			0, 3, 2,
			// Oberseite (Y = 0.5)
			4, 6, 5,
			4, 7, 6,
			// Vorderseite (Z = 0.5)
			1, 5, 6,
			1, 6, 2,
			// Rückseite (Z = -0.5)
			7, 3, 0,
			0, 4, 7,
			// Rechts (X = 0.5)
			0, 4, 5,
			0, 5, 1,
			// Links (X = -0.5)
			3, 2, 6,
			3, 6, 7
		};





		// a) VBO erzeugen, binden und Daten hochladen
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVert), cubeVert, GL_STATIC_DRAW);

		// b) VAO erzeugen und binden
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);

		// c) Vertex-Attribute definieren und aktivieren
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);         // Position
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Farbe (R,G)
		glEnableVertexAttribArray(1);

		// d) Indexbuffer erzeugen und binden
		glGenBuffers(1, &m_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeInd), cubeInd, GL_STATIC_DRAW);

		// e) Backface culling aktivieren, OpenGL rendert nur die Vorderseiten der Dreiecke,
		// Vorderseite = Dreiecke, deren Vertices gegen den Uhrzeigersinn (CCW) definiert sind,
		// Rückseite = Dreiecke, deren Vertices im Uhrzeigersinn (CW) definiert sind, werden ausgeblendet

		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_CCW);
		//glCullFace(GL_BACK);

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

/* Praktikum 1 Teil 2
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 42*3, GL_UNSIGNED_INT, 0); // 42 Indizes insgesamt
	glBindVertexArray(0);
*/
	// Praktikum 2
	// Shader aktivieren
	m_shader->use();

	// Berechne die Rotation für dieses Frame
	float rotationSpeed = 30.0f;  // 30 Grad pro Sekunde
	float deltaRotation = glm::radians(rotationSpeed * dt);  // Berechne Rotation für das aktuelle Frame
	glm::quat deltaQuat = glm::quat(glm::vec3(deltaRotation, deltaRotation, deltaRotation));  // Rotation um X, Y, Z
	cubeTrans->rotate(deltaQuat);  // Wende Rotation auf den Würfel an

	// Setze die Transformationsmatrix in den Shader
	m_shader->setUniform("model", cubeTrans->getMatrix(), false);

	// VAO binden
	glBindVertexArray(m_vao);

	// Würfel zeichnen
	glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, 0);

	// VAO lösen
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

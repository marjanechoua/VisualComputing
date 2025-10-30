

# 🧱 Visual Computing – Praktikum 1

### Hochschule Darmstadt – WS 2025/2026

**Thema:** Zeichnen von  Haus mit VAO, VBO und IBO in OpenGL

---

## 🎯 Ziel des Praktikums

In diesem Praktikum wird ein einfaches Objekt (ein Haus) mit OpenGL gezeichnet.
Dabei werden folgende Grundkonzepte angewendet:

* **Vertex Buffer Object (VBO)** – speichert die Vertex-Daten (Position, Farbe etc.)
* **Vertex Array Object (VAO)** – kennt die Struktur der Daten im VBO
* **Index Buffer Object (IBO)** – bestimmt, in welcher Reihenfolge die Vertices verbunden werden
* **Shader** – verarbeiten Positionen und Farben auf der GPU
* **Render-Schleife** – zeichnet jedes Frame neu

Das Framework übernimmt die Fensterverwaltung, das Rendering-Loop und das Shader-Management.
Wir arbeiten ausschließlich in der **Scene-Klasse**.

---

## 🗂️ Projektstruktur (wichtigste Dateien)

```
├── assets/
│   └── shaders/
│       ├── vertex.glsl
│       └── fragment.glsl
├── src/
│   ├── Scene.cpp
│   ├── Scene.h
│   ├── Window.cpp
│   ├── Window.h
│   └── main.cpp
```

---

## ⚙️ 1. Scene-Klasse (Hauptarbeitsbereich)

### 📄 `Scene.h`

Hier werden die Member-Variablen für OpenGL-Objekte deklariert:

```cpp
private:
    OpenGLWindow* m_window;
    AssetManager m_assets;
    ShaderProgram* m_shader;
    GLuint vaoID, vboID, iboID;
```

---

### 📄 `Scene.cpp`

#### Methode `init()`

Diese Methode wird **einmal beim Start** aufgerufen.
Hier werden alle OpenGL-Objekte erzeugt und initialisiert.

```cpp
bool Scene::init()
{
    // Shader laden
    m_assets.addShaderProgram("shader",
        AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
    m_shader = m_assets.getShaderProgram("shader");
    m_shader->use();

    // Vertex-Daten (Position + Farbe)
    float vertices[] = {
        -0.5, -0.5, 0.0, 0.0, 1.0,
         0.5, -0.5, 0.0, 0.0, 1.0,
         0.5,  0.5, 0.0, 1.0, 0.0,
         0.0,  1.0, 1.0, 0.0, 0.0,
        -0.5,  0.5, 0.0, 1.0, 0.0
    };

    int indices[] = {
        0, 1, 2,
        0, 2, 4,
        4, 2, 3
    };

    // 🔹 VBO erzeugen und befüllen
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 🔹 VAO erzeugen
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    // 🔹 Attribute definieren (Position und Farbe)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 🔹 IBO erzeugen
    glGenBuffers(1, &iboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::cout << "Scene initialization done\n";
    return true;
}
```

#### Methode `render(float dt)`

Wird **in jedem Frame** aufgerufen → zeichnet die Szene:

```cpp
void Scene::render(float dt)
{
    m_shader->use();      // Shader aktivieren
    glBindVertexArray(vaoID);  // VAO aktivieren

    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0); // Optional: VAO wieder lösen
}
```

---

## 🪟 2. Window-Klasse

### 📄 `Window.h`

Die Klasse `Window` erbt von `OpenGLWindow` und enthält eine `Scene`.

```cpp
class Window : public OpenGLWindow
{
private:
    std::unique_ptr<Scene> m_scene;
};
```

### 📄 `Window.cpp`

Hier wird die Szene initialisiert und gerendert.

```cpp
#include "Window.h"

Window::Window() {}
Window::~Window() {}

void Window::init()
{
    m_scene = std::make_unique<Scene>(this);
    m_scene->init();
}

void Window::render(GLdouble dtime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_scene->render(dtime);
}

void Window::shutdown()
{
    m_scene->shutdown();
}
```

---

## 🚀 3. main.cpp

```cpp
#include "Window.h"

int main()
{
    Window window;
    window.run(); // startet das Framework und Render-Loop
    return 0;
}
```

---

## 🧩 4. Shader

### 📄 `vertex.glsl`

```glsl
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vertexColor;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = aColor;
}
```

### 📄 `fragment.glsl`

```glsl
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
```

---

## 💡 5. Erweiterungen

1. **Initialen zeichnen:**
   Ändere die `vertices` und `indices`, um deine Initialen darzustellen.
   (z. B. ein „M“ oder „S“ aus Dreiecken)

2. **Backface Culling aktivieren:**
   Am Ende von `Scene::init()`:

   ```cpp
   glEnable(GL_CULL_FACE);
   glFrontFace(GL_CCW);
   glCullFace(GL_BACK);
   ```

---

## ✅ Ergebnis

Wenn alles korrekt ist, öffnet sich ein Fenster mit einem **Haus aus Dreiecken**, das über **VAO, VBO und IBO** auf der GPU gezeichnet wird.

---

Möchtest du, dass ich diese README noch etwas **grafischer mit ASCII-Diagrammen** und einer kurzen **Erklärung der Pipeline (CPU → GPU → Shader)** aufbereite, damit sie wie eine Mini-Dokumentation für deinen Praktikumsbericht aussieht?

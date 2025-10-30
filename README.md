
---


# OpenGL Praktikum – Setup & Übersicht

Dieses Projekt dient als Grundlage für die OpenGL-Praktika im Modul **Visual Computing**.  
Hier ist dokumentiert, wie das Projekt eingerichtet wurde, welche Bibliotheken verwendet werden und wie das System funktioniert.


---

## ⚙️ Einrichtungsschritte (was gemacht wurde)

1. **GLFW, GLEW, GLM, stb** in `libs/` gelegt.  
   → Diese Libraries sorgen für Fensterverwaltung, OpenGL-Funktionen und Matheoperationen.

2. **CMakeLists.txt** angepasst, damit alle Include-Pfade korrekt sind:  
   ```cmake
   include_directories(
       ${PROJECT_SOURCE_DIR}/libs/glfw-3.4/include
       ${PROJECT_SOURCE_DIR}/libs/glew-2.1.0/include
       ${PROJECT_SOURCE_DIR}/libs/glm-master
       ${PROJECT_SOURCE_DIR}/libs/stb
   )


3. **Pfadprobleme behoben** (alte GLFW 3.2.1 gelöscht, jetzt GLFW 3.4 und überall in den CMakeLists files 3.25 als minimum angegeben).


4. **CMake Build neu generiert**
   → in CLion: *File → Reload CMake Project*
   → oder manuell: `rm -rf cmake-build-debug && cmake -S . -B cmake-build-debug`

5. **Projekt erfolgreich gebaut:**
   Schwarzes Fenster bestätigt, dass Rendering-Loop aktiv ist 🎉

---

## 🧠 Wie das System funktioniert

* **`main.cpp`** ist der Startpunkt:
  Hier wird das Fenster erstellt (`Window mg;`) und die Hauptschleife (`mg.run()`) gestartet.

* **`Window` / `OpenGLWindow`**:
  Verantwortlich für die OpenGL-Initialisierung, Kontext-Erstellung, Event-Loop.

* **`ShaderProgram`**:
  Lädt und kompiliert deine Shader (`vertex_shader.glsl`, `fragment_shader.glsl`).

* **`Scene`**:
  Enthält Objekte, die in einer Szene gezeichnet werden (z. B. Würfel, Modelle, etc.).
  Wird vom `Window` geladen und in jedem Frame gerendert.

* **Rendering Loop**:

  ```cpp
  while (window.isOpen()) {
      shader.use();
      scene.render();
      window.swapBuffers();
  }
  ```

---

## 🧰 Neues Praktikum hinzufügen

Wenn du ein neues Praktikum (z. B. **Praktikum2**) anlegen willst:

1. Erstelle Datei:

   ```
   src/Framework/SceneElements/Praktikum2.cpp
   assets/scenes/Praktikum2.h
   ```

2. In `main.cpp`:

   ```cpp
   #include "Framework/SceneElements/Praktikum2.cpp"

   int main() {
       Window mg;
       Praktikum2 scene;
       mg.setScene(&scene);
       mg.run();
   }
   ```

3. Falls du neue Shader brauchst:
   Lege sie unter `assets/shader/` ab und lade sie im Konstruktor deiner Szene.

---

## 🚀 Projekt starten

In **CLion**:

* „Build Project“ drücken (Hammer-Symbol 🛠️)
* Dann „Run“ ▶️
* Ein schwarzes Fenster = erfolgreiches Setup ✅
  (Bald wird hier dein erstes 3D-Objekt erscheinen!)

---

## 💡 Nächste Schritte

* Clear Color ändern (einfacher Farb-Output)
* Erstes Dreieck zeichnen
* Kamera/Transformations-Matrizen hinzufügen
* Licht und Texturen integrieren

---

## 🧾 Troubleshooting

| Problem                 | Lösung                                        |
| ----------------------- | --------------------------------------------- |
| `GLFW header not found` | Pfad in `libheaders.h` oder CMake prüfen      |
| `shader not found`      | Shader-Datei-Pfad in `ShaderProgram` prüfen   |
| Schwarzes Fenster       | Kein Render-Aufruf / Clear-Color noch schwarz |
| Linker-Fehler           | CMake „Reload Project“ + Clean Build          |

---

✍️ *Erstellt von Marjane Choua – Visual Computing (5. Semester)*
📅 *Letztes Update: Oktober 2025*

```


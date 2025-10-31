
# Praktikum 1 – OpenGL Grundlagen und Buchstaben-Rendering

## 1. Zielsetzung

Das Ziel dieses Praktikums war es, grundlegende OpenGL-Funktionen kennenzulernen und zu verwenden:

1. Vertex- und Index-Buffer zu erzeugen, um geometrische Formen darzustellen.
2. Einfache Farb- und Positionsattribute für Vertex-Daten zu definieren.
3. Ein kleines grafisches Objekt (Haus) zu rendern.
4. Eigene Initialen (M und C) aus Rechtecken/Dreiecken zu bauen.
5. Backface culling zu verstehen und korrekt anzuwenden.

---

## 2. Aufbau der Szene

### 2.1 Vertex Buffer Object (VBO)

* **Was es ist:** Speicher auf der GPU, der rohe Vertex-Daten enthält.
* **Zweck:** OpenGL kann diese Daten direkt vom VBO ablesen, um die Geometrie zu rendern.
* **Code-Beispiel:**

```cpp
glGenBuffers(1, &m_vbo);
glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
```

> ⚠️ Fehler beim ersten Versuch: Vertices lokal definiert, ohne Member-Variable → konnte nicht später in `render()` genutzt werden. Lösung: `m_vbo` als Member definieren.

---

### 2.2 Vertex Array Object (VAO)

* **Was es ist:** Beschreibt, wie Vertex-Daten im VBO interpretiert werden (z.B. welche Daten Position sind, welche Farbe).
* **Zweck:** OpenGL weiß, wie die Daten gelesen werden, ohne jedes Mal Attribute neu zu definieren.

```cpp
glGenVertexArrays(1, &m_vao);
glBindVertexArray(m_vao);
```

---

### 2.3 Vertex Attributes

* **Position (x, y, z)**
* **Farbe (r, g, b)**

```cpp
// Position
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// Farbe
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);
```

> ⚠️ **Fehler:** Anfangs falsches Offset für Farbe gesetzt (`3` statt `2`) → falsches Farbmuster.
> ✅ Korrektur: Offset = 2, weil in `vertices` die Farbe nach den ersten zwei (x,y) Werten kommt.

---

### 2.4 Index Buffer Object (IBO/EBO)

* **Was es ist:** Definiert, welche Vertices zusammen ein Dreieck bilden.
* **Zweck:** Ermöglicht Wiederverwendung von Vertices für mehrere Dreiecke.
* **Code:**

```cpp
glGenBuffers(1, &m_ibo);
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
```

> ⚠️ **Problem:** Bei den Initialen war die richtige Reihenfolge der Indices wichtig wegen **Backface Culling** (CCW).
> ✅ Lösung: Indices in **counter-clockwise** Reihenfolge schreiben.

---

### 2.5 Render-Methode

```cpp
void Scene::render(float dt)
{
    m_shader->use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
```

> Hinweis: Ohne Member-Variablen (`m_vao`, `m_ibo`) funktioniert `render()` nicht, da lokale Variablen nach `init()` gelöscht werden.

---

## 3. Haus-Beispiel

* **Vertices:** 5 Punkte mit Position und Farbe
* **Indices:** 3 Dreiecke zur Darstellung des Hauses
* **Problem:** Farben falsch, weil Offset im `glVertexAttribPointer` für Farbe falsch war.

```cpp
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
```

* **Lösung:** Offset korrekt setzen → Farben werden richtig angezeigt.

---

## 4. Initialen (M + C)

### 4.1 Vorgehensweise

1. Raster definieren (0.0 – 1.0 in X/Y)
2. Rechtecke als Bausteine verwenden
3. Jedes Rechteck = 2 Dreiecke → 6 Indices pro Rechteck
4. Reihenfolge der Vertices für **CCW** beachten, sonst werden Flächen bei **Backface Culling** nicht angezeigt.

### 4.2 M

* **Aufbau:** 4 Vierecke

    * 2 links und rechts vertikal
    * 2 diagonal nach oben (Spitze des M)

```cpp
float verticesM[] = { ... }; // siehe vorheriges Beispiel
unsigned int indicesM[] = { ... };
```

> ⚠️ **Herausforderung:** Richtige Indices für die Dreiecke erstellen → mehrmals getestet.

### 4.3 C

* **Aufbau:** 3 Vierecke

    * 2 horizontal (oben, unten)
    * 1 vertikal (mittig)

```cpp
float verticesC[] = { ... };
unsigned int indicesC[] = { ... };
```

---

## 5. Backface Culling

```cpp
glEnable(GL_CULL_FACE);
glFrontFace(GL_CCW);
glCullFace(GL_BACK);
```

> ⚠️ **Problem:** Wenn Indices nicht CCW, werden Dreiecke nicht angezeigt.
> ✅ **Lösung:** Reihenfolge der Vertices in den Indices anpassen.

---

## 6. Probleme und Learnings

| Problem                              | Ursache                                                        | Lösung                                                         |
| ------------------------------------ | -------------------------------------------------------------- | -------------------------------------------------------------- |
| Farben falsch im Haus                | falsches Offset im `glVertexAttribPointer` für Farbe           | Offset auf 2 korrigiert                                        |
| Render-Methode zeigte nichts         | VBO/VAO/IBO waren lokal, nicht als Member                      | Variablen als Member definiert (`m_vbo`, `m_vao`, `m_ibo`)     |
| Initialen M/C nicht sichtbar         | Falsche Reihenfolge der Indices → Backface Culling schlug fehl | Indices CCW anpassen                                           |
| Koordinaten unklar                   | X/Y-Raster falsch interpretiert                                | Einheitliches Raster festgelegt, Vierecke aufgeteilt           |
| Viele kleine Dreiecke für Buchstaben | Zu komplex                                                     | Rechtecke in 2 Dreiecke aufteilen, einfacher und übersichtlich |

---

## 7. Zusammenfassung

* Praktikum hat gezeigt, wie Vertex- und Index-Buffer zusammenarbeiten.
* Backface Culling erfordert korrekte Vertex-Reihenfolge.
* Offset in `glVertexAttribPointer` ist entscheidend für Attribute.
* Rechteck-basierte Buchstaben erleichtern Rendering im 2D-Raster.
* Member-Variablen für GPU-Objekte notwendig, um sie über mehrere Methoden hinweg zu nutzen.

## 8. Bild der Buchstaben 

![Bildbeschreibung](/Users/marjanechoua/Library/Mobile Documents/com~apple~CloudDocs/myLocal/5.Semester/Visual Computing/Praktika/OpenGL_Praktikum/Buchstaben.png.)

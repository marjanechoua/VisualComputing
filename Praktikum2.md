

# 📄 **Dokumentation – Aufgabe 2.3: Mein eigener Roboter**

## **1. Ziel der Aufgabe**

In dieser Aufgabe soll ein einfacher Roboter aus Würfeln erstellt werden, wobei alle Körperteile über einen **Szenegraphen** miteinander verbunden sind.
Der Roboter besteht aus:

* Rumpf
* Kopf
* zwei Oberarmen
* zwei Unterarmen
* zwei Beinen

Alle Teile verwenden **dieselbe Geometrie** (einen Würfel) und haben **jeweils eine eigene Transformationsmatrix**.
Zusätzlich sollen Bewegungen (Rotationen) umgesetzt werden, sodass einfache Animationen wie Schwenkbewegungen dargestellt werden können.

---

## **2. Szenegraph (Aufbau & Struktur)**

Der Roboter wird hierarchisch aufgebaut. Das bedeutet:
Transformationen eines übergeordneten Elements wirken sich automatisch auf alle Kinder aus.

Hier der verwendete Szenegraph:

```
Root
└── Robot-Transform
    ├── Body
    ├── Head
    ├── LeftUpperArm
    │    └── LeftLowerArm
    ├── RightUpperArm
    │    └── RightLowerArm
    ├── LeftLeg
    └── RightLeg
```

**Wichtige Besonderheiten:**

* Die **Unterarme** hängen an den **Oberarmen**, d. h. jede Rotation des Oberarms beeinflusst den Unterarm.
* Die Beine sollen an der Hüfte rotieren.
* Root steuert z. B. die Gesamtdrehung der Szene.

---

## **3. Hintergrund löschen**

Da der Roboter pro Frame animiert wird, muss der Hintergrund jedes Frame neu gelöscht werden:

```cpp
glClear(GL_COLOR_BUFFER_BIT);
glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
```

`glClearColor()` legt die Hintergrundfarbe fest.
`glClear()` löscht den Framebuffer, um "Schmier-Effekte" zu verhindern.

---

## **4. Tiefentest konfigurieren**

Damit Objekte richtig verdeckt werden, wurde der Tiefentest aktiviert.
Da in Aufgabe 2.3 **keine Kamera** vorhanden ist, befinden wir uns direkt im NDC-Space.
Deshalb muss der Tiefentest **in umgekehrter Richtung** erfolgen:

```cpp
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_GREATER);
glClearDepth(0.0f);
```

Grund:
Ohne Kamera schaut OpenGL entlang der **positiven Z-Achse**, daher muss `GL_GREATER` verwendet werden.
Dieser Spezialfall wird in Aufgabe 3.1 wieder rückgängig gemacht, sobald eine echte Kamera vorhanden ist.

Zusätzlich wird der Depth-Buffer pro Frame gelöscht:

```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
```

---

## **5. Transformationsgruppen pro Körperteil**

Für jedes Körperteil wurde ein eigenes `Transform`-Objekt erzeugt:

```cpp
Transform bodyTrans;
Transform headTrans;
Transform leftUpperArmTrans;
Transform leftLowerArmTrans;
...
```

Jedes Objekt kann:

* skaliert
* verschoben
* rotiert
* um einen Punkt gedreht werden (`rotateAroundPoint()`)

---

## **6. Platzierung und Skalierung der Körperteile**

### **Beispiele:**

### Rumpf

```cpp
bodyTrans.translate(glm::vec3(0.0f, 0.0f, 0.0f));
bodyTrans.scale(glm::vec3(3.0f, 3.0f, 2.0f));
```

### Kopf

```cpp
headTrans.translate(glm::vec3(0.0f, 2.5f, 0.0f));
headTrans.scale(glm::vec3(1.0f));
```

### Linker Oberarm mit Animation

```cpp
leftUpperArmTrans.translate(glm::vec3(-2.0f, 0.6f, 0.0f));
leftUpperArmTrans.rotateAroundPoint(
    glm::vec3(0.0f, 0.6f, 0.0f),
    glm::vec3(glm::sin(time) * 0.25f, 0.0f, 0.0f)
);
leftUpperArmTrans.scale(glm::vec3(0.5f, 1.2f, 0.6f));
```

### Linker Unterarm folgt automatisch dem Oberarm

```cpp
drawPart(robotMat * leftUpperArmTrans.getTransformMatrix(), leftLowerArmTrans);
```

---

## **7. Animationen**

Für die Animation wurde die Zeitvariable in den Shader geladen:

```cpp
m_shader->setUniform("time", currentFrame);
```

Diese Zeit wird in den Rotationen genutzt:

```cpp
glm::sin(currentFrame)
```

Dadurch bewegen sich Arme und Beine rhythmisch.

---

## **8. Rendering**

Zu jedem Frame:

* Hintergrund löschen
* Tiefenpuffer löschen
* Shader aktivieren
* Transformationsmatrizen zurücksetzen
* Szenegraph traversieren und jede Teilmatrix berechnen
* Mit `drawPart()` jedes Körperteil zeichnen

Die Knoten werden korrekt multipliziert:

```cpp
glm::mat4 model = parent * localTransform;
```

---

## **9. Ergebnis**

Der fertige Roboter:

* ist richtig skaliert und positioniert
* besitzt vollständige Hierarchie
* Arme und Beine können um sinnvolle Gelenkpunkte rotieren
* Animationen funktionieren (Schwenken der Extremitäten)
* wird korrekt verdeckt, da Tiefentest aktiv ist
* wird ohne Kamera korrekt in NDC dargestellt (wie in der Aufgabe gefordert)


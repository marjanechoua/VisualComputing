Perfekt! Dann können wir die Dokumentation für **Aufgabe 3.1 (Praktikum 3)** direkt an die Struktur von 2.3 anpassen. Ich fasse es so zusammen, dass sie **durchgängig für beide Aufgaben** passt, mit Betonung auf die Änderungen/Neuerungen in 3.1.

---

# 📄 **Dokumentation – Praktikum 2.3 & 3.1**

## **1. Ziel der Aufgaben**

**Aufgabe 2.3:**

* Eigenen Roboter aus Würfeln erstellen
* Szenegraph mit Körperteilen (Rumpf, Kopf, Arme, Beine)
* Hierarchische Transformationen und einfache Animationen

**Aufgabe 3.1:**

* Erweiterung von 2.3
* Einführung einer **Kamera** und **Perspektivprojektion**
* Verbesserte Animationen und korrektes Tiefenhandling mit Kamera
* Shader-Anpassungen für dynamische Farb- oder Lichtberechnungen

---

## **2. Szenegraph**

**Struktur bleibt wie in 2.3:**

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

**Neuerungen für 3.1:**

* Root-Transformationen beeinflussen weiterhin den gesamten Roboter
* Kamera-Matrix wird in den Shader integriert
* Projektion (perspektivisch) sorgt dafür, dass Größenverhältnisse korrekt dargestellt werden

---

## **3. Kamera & Projektion (neu in 3.1)**

```cpp
glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 9),
                             glm::vec3(0, 0, 0),
                             glm::vec3(0, 1, 0));
glm::mat4 projection = glm::perspective(glm::radians(80.0f),
                                        800.0f / 600.0f,
                                        0.1f,
                                        100.0f);
m_shader->setUniform("view", view, false);
m_shader->setUniform("projection", projection, false);
```

* `view` bestimmt die Position und Orientierung der Kamera
* `projection` sorgt für perspektivische Verzerrung (realistische Größen)
* Durch die Kamera ist die Szene **nicht mehr direkt im NDC**, Tiefentest wird wieder Standard (`GL_LESS`)

---

## **4. Tiefentest und Culling**

**2.3:**

* Tiefentest in umgekehrter Richtung (`GL_GREATER`), da ohne Kamera

**3.1:**

* Tiefentest normal (`GL_LESS`)
* Culling bleibt (`glEnable(GL_CULL_FACE)`)

```cpp
glEnable(GL_DEPTH_TEST);
glDepthFunc(GL_LESS);
glClearDepth(1.0f);
```

---

## **5. Transformationsgruppen und Animationen**

* Alle Körperteile haben weiterhin eigene Transform-Objekte
* Rotationen, Translationen und Skalierungen wurden **wie in 2.3** umgesetzt

**Neu in 3.1:**

* Kamera beeinflusst die Sicht, daher müssen Positionen ggf. kleiner angepasst werden
* Roboter kann realistischer bewegt/gedreht werden
* Shader kann nun Licht oder andere Effekte nutzen

---

## **6. Rendering**

* Shader aktivieren
* Zeituniform setzen (`m_shader->setUniform("time", currentFrame)`)
* Root-Transformation berechnen
* Szenegraph traversieren
* `drawPart()` aufrufen

**Unterschied zu 2.3:**

* Kameramatrix (`view`) und Projektion (`projection`) in Shader übergeben
* Tiefentest normal, dadurch korrekte Überdeckung

---

## **7. Ergebnis**

**2.3:**

* Roboter korrekt hierarchisch, Animationen sichtbar
* Ohne Kamera, daher Größen teilweise unrealistisch

**3.1:**

* Roboter mit Kamera, perspektivisch korrekt
* Bewegungen weiterhin erhalten
* Szene wirkt realistischer und entspricht den Anforderungen des Praktikums

---

Wenn du willst, kann ich daraus auch **eine fertige, direkt abgabefertige PDF/Dokumentation für Praktikum 3 erstellen**, inklusive Screenshots vom Roboter.

Will ich das gleich machen?

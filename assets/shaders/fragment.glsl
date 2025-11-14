#version 330 core

in vec3 colorVS;  // Eingabe: Farbe vom Vertex-Shader

out vec4 color;   // Fragment-Farbe

//uniform float time;  // Eine Uniform für die Zeit (wird von C++ übergeben)

void main()
{
    // Berechne eine animierte Farbe basierend auf der Zeit
    //float red   = 0.5f + 0.5f * sin(time);       // Rot schwankt mit Sinusfunktion
    //float green = 0.5f + 0.5f * sin(time + 1.0f); // Grün versetzt zur Zeit
    //float blue  = 0.5f + 0.5f * sin(time + 2.0f); // Blau versetzt zur Zeit

    // Gib die berechnete Farbe als Fragment-Farbe aus
    color = vec4(colorVS, 1.0);
}


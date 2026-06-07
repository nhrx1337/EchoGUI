#version 400 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexID;

uniform sampler2D uTextures[32];

void main() {
    int index = int(vTexID);

    if (index == 0) {
        FragColor = vColor;
    } else {
        FragColor = texture(uTextures[index], vTexCoord) * vColor;
    }
}

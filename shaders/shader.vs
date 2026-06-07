#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexID;

out vec4 vColor;
out vec2 vTexCoord;
out float vTexID;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos, 1.0);
    vColor = aColor;
    vTexCoord = aTexCoord;
    vTexID = aTexID;
}

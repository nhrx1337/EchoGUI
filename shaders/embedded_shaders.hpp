#ifndef ECHOGUI_EMBEDDED_SHADERS_HPP
#define ECHOGUI_EMBEDDED_SHADERS_HPP

static const char* const VERTEX_SHADER_SOURCE = R"GLSL(#version 400 core
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
)GLSL";

static const char* const FRAGMENT_SHADER_SOURCE = R"GLSL(#version 400 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vTexCoord;
in float vTexID;

uniform sampler2D uTextures[32];

void main() {
    int index = int(vTexID);

    if (index == 0) {
        // Negative alpha means border rendering
        if (vColor.a < 0.0) {
            float thickness = -vColor.a;

            if (vTexCoord.x > thickness && vTexCoord.x < (1.0 - thickness) &&
                vTexCoord.y > thickness && vTexCoord.y < (1.0 - thickness)) {
                discard;
            }

            FragColor = vec4(vColor.rgb, 1.0f);
        } else {
            FragColor = vColor;
        }
    } else {
        FragColor = texture(uTextures[index], vTexCoord) * vColor;
    }
}
)GLSL";

#endif

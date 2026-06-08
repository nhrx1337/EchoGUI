#version 400 core

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

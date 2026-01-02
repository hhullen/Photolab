#version 440

layout(location = 0) in vec2 v_texCoord;

layout(location = 0) out vec4 fragColor;

layout(binding = 0) uniform sampler2D initPicture;
layout(binding = 1) uniform sampler2D editedPicture;

layout(binding = 2, std140) uniform buf {
    float cutter;
};

void main() {
    if (v_texCoord.x <= cutter) {
        fragColor = texture(initPicture, v_texCoord);
    } else {
        fragColor = texture(editedPicture, v_texCoord);
    }
}

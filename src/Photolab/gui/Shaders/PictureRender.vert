#version 440

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

layout(location = 0) out vec2 v_texCoord;

// В Qt 6.9 для SPIR-V (Vulkan/Metal/DX12) это стандартный блок
out gl_PerVertex {
    vec4 gl_Position;
};

void main()
{
    v_texCoord = texCoord;
    gl_Position = vec4(position, 0.0, 1.0);
}

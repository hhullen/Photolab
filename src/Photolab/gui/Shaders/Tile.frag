#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D src;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float tileSize;
    float areaWidth;
    float areaHeight;
} ubuf;

void main() {
  vec2 scale = vec2(ubuf.areaWidth, ubuf.areaHeight) / ubuf.tileSize;
  vec2 uv = fract(qt_TexCoord0 * scale);

  fragColor = texture(src, uv) * ubuf.qt_Opacity;
}

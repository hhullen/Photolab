#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D src;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float blurRadius;
    bool isHorizontal;
    float width;
    float height;
    float brightness;
} ubuf;

const int weightsAmount = 9;
const float weights[weightsAmount] = float[](0.154615, 0.128180, 0.093778, 0.060548, 0.034500, 0.017348, 0.007698, 0.003015, 0.000318);

void main() {
  const vec4 initPix = texture(src, qt_TexCoord0);

  vec3 sum = initPix.rgb * weights[0] * 2;

  float texelSize = 1.0 / ubuf.height;
  vec2 offsetFactor = vec2(0.0, 1.0);

  if (ubuf.isHorizontal) {
    texelSize = 1.0 / ubuf.width;
    offsetFactor = vec2(1.0, 0.0);
  }

  const float texelHalf = texelSize / 2;

  for (int i = 1; i < weightsAmount; i++) {
    const float offset = texelSize * i * ubuf.blurRadius + texelHalf;
    sum += texture(src, qt_TexCoord0 + offset * offsetFactor).rgb * weights[i];
    sum += texture(src, qt_TexCoord0 - offset * offsetFactor).rgb * weights[i];
  }

  fragColor = vec4(sum * ubuf.brightness, ubuf.qt_Opacity);
}

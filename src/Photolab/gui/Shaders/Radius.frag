#version 440

layout(location = 0) in vec2 qt_TexCoord0;
layout(location = 0) out vec4 fragColor;

layout(binding = 1) uniform sampler2D src;

layout(std140, binding = 0) uniform buf {
    mat4 qt_Matrix;
    float qt_Opacity;
    float width;
    float height;
    float radius;
} ubuf;

const float edgeWidth = 2.0;

float calcDistance(vec2 uv) {
    vec2 positionInQuadrant = abs(uv * 2.0 - 1.0);
    vec2 extend = vec2(ubuf.width, ubuf.height) / 2.0;
    vec2 coords = positionInQuadrant * (extend + ubuf.radius);
    vec2 delta = max(coords - extend, 0.0);
    return length(delta);
}

float calcMask(float radius, float dist) {
  if (radius > 0) {
    return smoothstep(radius, radius - edgeWidth, dist);
  }
  return 1.0;
}

void main() {
  float dist = calcDistance(qt_TexCoord0);

  if (dist > ubuf.radius) {
      fragColor = vec4(0.0, 0.0, 0.0, 0.0);
      return;
  }

  fragColor = texture(src, qt_TexCoord0) * ubuf.qt_Opacity * calcMask(ubuf.radius, dist);
}

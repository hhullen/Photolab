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
    float borderRradius;
} ubuf;

vec2 resolution = vec2(ubuf.areaWidth, ubuf.areaHeight);

float calcDistance(vec2 uv) {
    vec2 positionInQuadrant = abs(uv * 2.0 - 1.0);
    vec2 extend = resolution / 2.0;
    vec2 coords = positionInQuadrant * (extend + ubuf.borderRradius);
    vec2 delta = max(coords - extend, 0.0);
    return length(delta);
}

void main() {
  float dist = calcDistance(qt_TexCoord0);

  const float edgeWidth = 2.0;
  float mask = smoothstep(ubuf.borderRradius, ubuf.borderRradius-edgeWidth, dist);

  if (dist > ubuf.borderRradius) {
      fragColor = vec4(0.0, 0.0, 0.0, 0.0);
      return;
  }

  vec2 scale = resolution / ubuf.tileSize;
  vec2 uv = fract(qt_TexCoord0 * scale);
  fragColor = texture(src, uv) * ubuf.qt_Opacity* mask;

}

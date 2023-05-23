#version 330

in vec2 vUv;

out vec4 fragColor;

uniform sampler2D uMap;
uniform sampler2D uMask;
uniform float uScale;
uniform vec2 uOffset;
uniform float uFlip;

void main() {
    vec4 mask = texture(uMask, vUv);
    vec2 uv = vUv;
    uv += uOffset * vec2(uFlip, 1.0);
    uv -= vec2(0.5);
    uv /= uScale;
    uv += vec2(0.5);
    fragColor = texture(uMap, uv);
    fragColor.a *= mask.r * 2.0;
}
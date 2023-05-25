//#version 330
//
//in vec2 vUv;
//
//out vec4 fragColor;
//
//uniform sampler2D uMap;
//uniform sampler2D uMask;
//uniform float uScale;
//uniform vec2 uOffset;
//uniform float uFlip;
//
//void main() {
//    vec4 mask = texture(uMask, vUv);
//    vec2 uv = vUv;
//    uv += uOffset * vec2(uFlip, 1.0);
//    uv -= vec2(0.5);
//    uv /= uScale;
//    uv += vec2(0.5);
//    fragColor = texture(uMap, uv);
//    fragColor.a *= mask.r * 2.0;
//}
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{             
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(vec3(depth), 1.0);
}
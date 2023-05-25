#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;
layout(location = 5) in ivec4 boneIds; 
layout(location = 6) in vec4 weights;

out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    mat3 TBN;
} vs_out;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 finalBonesMatrices[MAX_BONES];

uniform vec3 viewPos;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
    {
        if(boneIds[i] == -1) 
            continue;
        if(boneIds[i] >=MAX_BONES) 
        {
            totalPosition = vec4(position,1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(position,1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * normal;
    }

	gl_Position = projection * view * model * totalPosition;

    mat3 viewModel = mat3(view * model);

    vs_out.FragPos = vec3(model * totalPosition);
    vs_out.TexCoords = texCoords;


    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    vs_out.TBN = transpose(mat3(T, B, N));
    vs_out.TangentViewPos  = vs_out.TBN * viewPos;
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos;


}
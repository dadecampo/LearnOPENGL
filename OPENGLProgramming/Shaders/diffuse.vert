#version 330
layout (location = 0) in vec3 VertexPosition; // Stream of vertex positions 
layout (location = 1) in vec3 VertexNormal; // Stream of vertex normals 
layout (location = 2) in vec2 TexCoord;
layout (location = 3) in ivec4 BoneIDs; // Stream of vertex bone IDs
layout (location = 4) in vec4 Weights; // Stream of vertex weights

out vec2 TexCoord0;
out vec3 Normal0; // Transformed normal
out vec3 LocalPos0; // Vertex position in eye coords
flat out ivec4 BoneIDs0;
out vec4 Weights0;

uniform mat4 gWVP;

uniform mat3 NormalMatrix; // Normal matrix 
uniform mat4 M; // Model matrix 
uniform mat4 V; // View matrix 
uniform mat4 P; // Projection matrix 

const int MAX_BONES = 200;

uniform mat4 gBones[MAX_BONES];

void main()
{
	mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
    BoneTransform     += gBones[BoneIDs[3]] * Weights[3];

    vec4 PosL = BoneTransform * vec4(VertexPosition, 1.0);
	gl_Position = (P * V * M) * PosL;
	TexCoord0 = TexCoord;
	Normal0=VertexNormal;
	LocalPos0=VertexPosition;
	BoneIDs0=BoneIDs;
	Weights0=Weights;

}

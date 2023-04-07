#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);

	//Semplicemente si ribalta la coordinata X della tessitura ( sappiamo che va da x e y appartengono all'intervallo [0,1] )
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0-TexCoord.x, TexCoord.y)), 0.2);
}
#version 330

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 LocalPos0;
flat in ivec4 BoneIDs0;
in vec4 Weights0;

uniform vec3 lightPos;
uniform vec3 lightIntensity;
uniform vec3 Kd;            // Diffuse reflectivity
uniform vec3 Ka;	// Ambient reflectivity 
uniform vec3 Ks;	// Specular reflectivity
uniform float specularShininess;	// Specular shininess 



out vec4 FragColor;

uniform sampler2D gSampler;

vec3 ads()
{
	vec3 n = normalize(Normal0);
		
	vec3 l = normalize( vec3(lightPos) - LocalPos0);

	float df = max( dot(l,n), 0.0);
	float sf = 0.0f;

	if (df > 0.0f) 
	{
		vec3 v = normalize(vec3(LocalPos0));
		vec3 r = reflect(l, n);
		sf = max(dot(r,v), 0.0f);
	}

	return lightIntensity * ( Ka + Kd * df  + Ks * pow( sf, specularShininess ));
}

void main()
{
    FragColor = texture(gSampler, TexCoord0.xy) + vec4(ads(),1);
}
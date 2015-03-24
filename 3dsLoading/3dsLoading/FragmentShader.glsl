#version 330 core
in vec2 UV;
in vec3 normal;
in vec3 fragmentColor;

out vec3 color;

uniform sampler2D textureSampler;

uniform vec4 lightDirection;
uniform vec4 lightColor;

void main()
{
	vec3 outColor = clamp(texture(textureSampler, UV).rgb, 0.0f, 1.0f);
	vec4 lightDir = lightDirection;
	lightDir.y = -lightDir.y;
	float lightPower = clamp(dot((lightDir).xyz, normal), 0.0f, 1.0f);
	vec3 diffuse = lightPower * lightColor.xyz;

	color = clamp(diffuse * outColor + fragmentColor, 0.0f, 1.0f);
}
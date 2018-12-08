#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	//FragColor = texture(texture1, vec2(0.5f, 0.5f));
	FragColor = texture(texture1, TexCoord);
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
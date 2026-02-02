# version 330 core

out vec4 FragColor;

// Texture to be passed
uniform sampler2D tex0;

// Should receive the texCoord from the vertex shader 
in vec2 texCoord;

void main()
{
	//			      r     g    b    a             ranges from 0.f -> 1.0f
	// FragColor = vec4(0.0f, 0.f, 1.f, 1.f);

	// Assign the texture color using the function
	FragColor = texture(tex0, texCoord);
}
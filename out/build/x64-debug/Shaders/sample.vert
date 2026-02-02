# version 330 core

// Gets the data at Attribute Position 0
// Converts it and stores it into a vec3 variable called aPos
layout(location = 0) in vec3 aPos;

// Projection matrix
uniform mat4 projection;

// uniform float x;
// uniform float y;

// Create a transformation matrix

uniform mat4 transform;

// View / camera mat
uniform mat4 view;

// The tex coord / UV is at 0
// Accesses the UV and assigns it to aTex

layout(location = 2) in vec2 aTex;

// Pass the tex coord to the fragment shader
out vec2 texCoord;

void main()
{
	//vec3 newPos = vec3(aPos.x + aPos.y, aPos.z);
	gl_Position =  // Shutter
	projection * // Apply the lens
	view * // Position the camera
	transform * vec4(aPos, 1.0); // Position model

	texCoord = aTex;
}


in vec3 a_position;
in vec3 a_color;
out vec3 color;
uniform mat4 modelViewProjectionMatrix;

void main()
{
	color = a_color * 2.5f;
	gl_Position = modelViewProjectionMatrix * vec4(a_position, 1);
}
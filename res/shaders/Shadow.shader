#shader vertex
#version 330 core

in vec3 vtx_position;

uniform mat4 light_space_matrix;
uniform mat4 model_matrix;

void main()
{
	gl_Position = light_space_matrix * model_matrix * vec4(vtx_position, 1.0);
};

#shader fragment
#version 330 core

void main()
{
};

#version 130

in vec3 position;
in vec3 colorIn;
in vec3 translate;

out vec3 colorOut;

mat4 projection = mat4(
	vec4( 2.0, 0.0, 0.0, 0.0 ),
	vec4( 0.0, 2.0, 0.0, 0.0 ),
	vec4( 0.0, 0.0, 1.0, 0.0 ),
	vec4(-1.0,-1.0, 0.0, 1.0 ));

mat4 m_translate = mat4(
	vec4(1.0, 0.0, 0.0, 0.0),
	vec4(0.0, 1.0, 0.0, 0.0),
	vec4(0.0, 0.0, 1.0, 0.0),
	vec4(0.0, 0.0, 0.0, 1.0));

void main()
{
	m_translate[3] = vec4(translate, 1.0);
	gl_Position = projection * m_translate * vec4(position, 1.0);
	colorOut = colorIn;
}

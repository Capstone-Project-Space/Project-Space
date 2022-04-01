#version 330 core

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in int a_Texture;
layout(location = 3) in vec3 a_Color;
layout(location = 4) in mat4 a_Transform;

out vec2 v_UV;
flat out int v_Texture;
out vec3 v_Color;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main() {
	v_UV = a_UV;
	v_Texture = a_Texture;
	v_Color = a_Color;

	gl_Position = u_Projection * u_View * a_Transform * vec4(a_Vertex, 1.0f);
}

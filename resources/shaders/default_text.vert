#version 330 core

layout(location = 0) in vec3 a_Vertex;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in int a_Texture;
layout(location = 4) in mat4 a_Transform;

out vec2 v_UV;
out vec3 v_Color;
flat out int v_Texture;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main() {
	v_UV = a_UV;
	v_Color = a_Color;
	v_Texture = a_Texture;

	gl_Position = u_Projection * u_View * a_Transform * vec4(a_Vertex, 1.0f);
}

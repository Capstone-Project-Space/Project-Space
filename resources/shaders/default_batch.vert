#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_UV;
layout(location = 2) in int a_Texture;
layout(location = 3) in mat4 a_Transform;

out vec2 v_UV;
out flat int v_Texture;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main() {
	v_UV = a_UV;
	v_Texture = a_Texture;
	gl_Position = u_Projection * u_View * a_Transform * vec4(a_Position, 0, 1.0f);
}

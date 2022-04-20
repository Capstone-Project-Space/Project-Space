#version 330 core

layout(location = 0) in vec3 a_Vertex; 
layout(location = 1) in vec2 a_UV; 
layout(location = 2) in vec3 a_Normal; 
layout(location = 3) in int  a_Texture;
layout(location = 4) in vec3 a_Ambient;
layout(location = 5) in vec3 a_Diffuse;
layout(location = 6) in vec3 a_Specular;
layout(location = 7) in float a_SpecExponent;

out vec2	  v_UV;
flat out int  v_Texture;
out vec3	  v_Ambient;
out vec3	  v_Diffuse;
out vec3	  v_Specular;
out float	  v_SpecExponent;
flat out vec3 v_Normal;
flat out vec3 v_ECPosition;
flat out vec3 v_EyePos;
flat out int  v_InstanceID;


// vec3 EyeLightPos = vec3(0, 10, 0);


struct UniformData {
	mat4 transform;
	vec4 color;
};

layout(std140) uniform Matrices {
	mat4 u_ProjectionMatrix;
	mat4 u_ViewMatrix;
	UniformData u_ModelMatrices[512];
};


void main() {
	v_UV = a_UV;
	v_Texture = a_Texture;
	v_Ambient = a_Ambient;
	v_Diffuse = a_Diffuse;
	v_Specular = a_Specular;
	v_SpecExponent = a_SpecExponent;
	v_InstanceID = gl_InstanceID;

	mat3 normalMatrix = mat3(transpose(inverse(u_ModelMatrices[gl_InstanceID].transform)));
	v_Normal = normalize((normalMatrix * a_Normal));
	
	vec4 ECPosition = u_ModelMatrices[gl_InstanceID].transform * vec4(a_Vertex.x, a_Vertex.y, a_Vertex.z, 1.0);
	v_ECPosition = ECPosition.xyz;

	v_EyePos = vec3(0, 0, 0) - ECPosition.xyz;

	gl_Position = u_ProjectionMatrix * u_ViewMatrix * ECPosition;
}

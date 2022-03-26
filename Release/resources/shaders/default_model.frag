#version 330 core

in vec2		  v_UV;
flat in int   v_Texture;
in vec3		  v_Ambient;
in vec3		  v_Diffuse;
in vec3		  v_Specular;
in float	  v_SpecExponent;
flat in vec3  v_Normal;
flat in vec3  v_ECPosition;
flat in vec3  v_EyePos;

out vec4 color;

uniform sampler2D u_Texture0;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform sampler2D u_Texture3;
uniform sampler2D u_Texture4;
uniform sampler2D u_Texture5;
uniform sampler2D u_Texture6;
uniform sampler2D u_Texture7;
uniform sampler2D u_Texture8;
uniform sampler2D u_Texture9;
uniform sampler2D u_Texture10;
uniform sampler2D u_Texture11;
uniform sampler2D u_Texture12;
uniform sampler2D u_Texture13;
uniform sampler2D u_Texture14;
uniform sampler2D u_Texture15;
uniform sampler2D u_Texture16;
uniform sampler2D u_Texture17;
uniform sampler2D u_Texture18;
uniform sampler2D u_Texture19;
uniform sampler2D u_Texture20;
uniform sampler2D u_Texture21;
uniform sampler2D u_Texture22;
uniform sampler2D u_Texture23;
uniform sampler2D u_Texture24;
uniform sampler2D u_Texture25;
uniform sampler2D u_Texture26;
uniform sampler2D u_Texture27;
uniform sampler2D u_Texture28;
uniform sampler2D u_Texture29;
uniform sampler2D u_Texture30;
uniform sampler2D u_Texture31;


struct LightData {
	vec4 position;
};

layout(std140) uniform Lights {
	LightData u_Lights[64];
	int u_Count;
};

void main() {
	switch (v_Texture) {
		case 0:
			color = texture(u_Texture0, v_UV);
			break;
		case 1:
			color = texture(u_Texture1, v_UV);
			break;
		case 2:
			color = texture(u_Texture2, v_UV);
			break;
		case 3:
			color = texture(u_Texture3, v_UV);
			break;
		case 4:
			color = texture(u_Texture4, v_UV);
			break;
		case 5:
			color = texture(u_Texture5, v_UV);
			break;
		case 6:
			color = texture(u_Texture6, v_UV);
			break;
		case 7:
			color = texture(u_Texture7, v_UV);
			break;
		case 8:
			color = texture(u_Texture8, v_UV);
			break;
		case 9:
			color = texture(u_Texture9, v_UV);
			break;
		case 10:
			color = texture(u_Texture10, v_UV);
			break;
		case 11:
			color = texture(u_Texture11, v_UV);
			break;
		case 12:
			color = texture(u_Texture12, v_UV);
			break;
		case 13:
			color = texture(u_Texture13, v_UV);
			break;
		case 14:
			color = texture(u_Texture14, v_UV);
			break;
		case 15:
			color = texture(u_Texture15, v_UV);
			break;
		case 16:
			color = texture(u_Texture16, v_UV);
			break;
		case 17:
			color = texture(u_Texture17, v_UV);
			break;
		case 18:
			color = texture(u_Texture18, v_UV);
			break;
		case 19:
			color = texture(u_Texture19, v_UV);
			break;
		case 20:
			color = texture(u_Texture20, v_UV);
			break;
		case 21:
			color = texture(u_Texture21, v_UV);
			break;
		case 22:
			color = texture(u_Texture22, v_UV);
			break;
		case 23:
			color = texture(u_Texture23, v_UV);
			break;
		case 24:
			color = texture(u_Texture24, v_UV);
			break;
		case 25:
			color = texture(u_Texture25, v_UV);
			break;
		case 26:
			color = texture(u_Texture26, v_UV);
			break;
		case 27:
			color = texture(u_Texture27, v_UV);
			break;
		case 28:
			color = texture(u_Texture28, v_UV);
			break;
		case 29:
			color = texture(u_Texture29, v_UV);
			break;
		case 30:
			color = texture(u_Texture30, v_UV);
			break;
		case 31:
			color = texture(u_Texture31, v_UV);
			break;
		default:
			color = texture(u_Texture0, v_UV);
			break;
	}

	for (int i = 0; i < u_Count; i++) {
		vec3 normal = normalize(v_Normal);
		vec3 light  = normalize(u_Lights[i].position.xyz - v_ECPosition);
		vec3 eye    = normalize(v_EyePos);

		vec3 ambient = .5 * color.rgb;
	
		float d = max(dot(normal, light), 0);
		vec3 diffuse = .4 * d * v_Diffuse;

		float s = 0;
		if (dot(normal, light) > 0.) {
			vec3 ref = normalize(2 * normal * dot(normal, light) - light);
			s = pow(max(dot(eye, ref), 0), v_SpecExponent);
		}
		vec3 specular = .1 * s * v_Specular;

		color = vec4(ambient + diffuse + specular, 1.0);
	}
}

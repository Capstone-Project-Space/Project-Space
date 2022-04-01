#version 330 core

in vec2 v_UV;
flat in int v_Texture;
in vec3 v_Color;

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

void main() {
	switch (v_Texture) {
		case 0:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture0, v_UV).r);
			break;
		case 1:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture1, v_UV).r);
			break;
		case 2:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture2, v_UV).r);
			break;
		case 3:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture3, v_UV).r);
			break;
		case 4:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture4, v_UV).r);
			break;
		case 5:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture5, v_UV).r);
			break;
		case 6:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture6, v_UV).r);
			break;
		case 7:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture7, v_UV).r);
			break;
		case 8:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture8, v_UV).r);
			break;
		case 9:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture9, v_UV).r);
			break;
		case 10:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture10, v_UV).r);
			break;
		case 11:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture11, v_UV).r);
			break;
		case 12:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture12, v_UV).r);
			break;
		case 13:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture13, v_UV).r);
			break;
		case 14:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture14, v_UV).r);
			break;
		case 15:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture15, v_UV).r);
			break;
		case 16:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture16, v_UV).r);
			break;
		case 17:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture17, v_UV).r);
			break;
		case 18:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture18, v_UV).r);
			break;
		case 19:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture19, v_UV).r);
			break;
		case 20:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture20, v_UV).r);
			break;
		case 21:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture21, v_UV).r);
			break;
		case 22:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture22, v_UV).r);
			break;
		case 23:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture23, v_UV).r);
			break;
		case 24:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture24, v_UV).r);
			break;
		case 25:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture25, v_UV).r);
			break;
		case 26:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture26, v_UV).r);
			break;
		case 27:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture27, v_UV).r);
			break;
		case 28:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture28, v_UV).r);
			break;
		case 29:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture29, v_UV).r);
			break;
		case 30:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture30, v_UV).r);
			break;
		case 31:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture31, v_UV).r);
			break;
		default:
			color = vec4(1.0f, 1.0f, 1.0f, texture(u_Texture0, v_UV).r);
			break;
	}

	color = vec4(v_Color, 1.0) * color;
}

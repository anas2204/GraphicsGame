#version 330 core

// input vertex data, different for all executions of this shader.
layout( location = 0 ) in vec3 vertexPos;
layout( location = 1 ) in vec3 texturePos;
layout( location = 2 ) in vec3 normalPos;
layout( location = 3 ) in vec3 Ka;
layout( location = 4 ) in vec3 Kd;
layout( location = 5 ) in vec3 Ks;
layout( location = 6 ) in float Ns;
layout( location = 7 ) in float textureIndex;
layout( location = 8 ) in vec3 boundingVertex;
layout( location = 9 ) in vec2 alphabetUV;
layout( location = 10) in vec2 alphabetVertex;

uniform mat4 MVP;
uniform mat4 View;
uniform mat4 Model;

uniform highp int drawBounding;
uniform highp int drawAlphabets;

uniform vec3 LightPos;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform vec3 CameraPos;

out vec3 normalPosOut;
out vec3 vertexPosOut;
out vec3 LightPosOut;
out vec3 VectorV;
out vec3 KdOut;
out vec3 KsOut;
out vec3 KaOut;
out float NsOut;
out vec2 alphabetUVOut;

//out float textureIndexOut;

out vec2 texturePosOut;

void main() {

	if(drawAlphabets == 1)
	{
		vec2 vertexPosition_homoneneousspace = alphabetVertex - vec2(400,300);
		vertexPosition_homoneneousspace /= vec2(400,300);

		gl_Position = vec4(vertexPosition_homoneneousspace,0,1);

		alphabetUVOut = vec2(alphabetUV.x,-alphabetUV.y);
	}	

	else if (drawBounding == 1)
	{
		gl_Position = MVP * vec4(boundingVertex,1);
	}

	else
	{
		gl_Position = MVP * vec4(vertexPos,1);
	}

	
	vertexPosOut = (View * Model * vec4(vertexPos,1)).xyz;

	KdOut = Kd;
	KsOut = Ks;
	KaOut = Ka;
	NsOut = Ns;

	//textureIndexOut = textureIndex;
	texturePosOut = vec2(texturePos.x,1.0-texturePos.y);

	vec3 temp = (View * Model * vec4(normalPos,0)).xyz;
	normalPosOut = normalize (temp);

	LightPosOut = (View * vec4(LightPos,1)).xyz;
	LightPosOut = LightPosOut - vertexPosOut;

	VectorV = CameraPos - vertexPosOut;
}
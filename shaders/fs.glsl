#version 330 core

in vec3 fragmentColor;
in vec3 normalPosOut;
in vec3 vertexPosOut;

in vec3 LightPosOut;
in vec3 VectorV;

in vec3 KdOut;
in vec3 KsOut;
in vec3 KaOut;

in float NsOut;

//in textureIndexOut;
in vec2 texturePosOut;

in vec2 alphabetUVOut;

uniform sampler2D texture1;
//uniform sampler2D texture2;

uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;

uniform highp int drawBounding;
uniform highp int drawAlphabets;

// ouput data
out vec3 color;

void main() {

	if(drawAlphabets == 1)
	{
		color = texture2D(texture1, alphabetUVOut).rgb;
	}

	else if(drawBounding == 1)
	{
		color = vec3 (1.0,1.0,1.0);
	}

	else
	{	
		color = (La * KaOut);

		//Diffuse
		float dotp = clamp (dot(normalPosOut,normalize(LightPosOut)) , 0 ,1);
		color = color + (Ld * KdOut * dotp);

		//Specular
		vec3 temp = normalize(LightPosOut + VectorV);
		dotp = clamp ( dot(normalPosOut,temp), 0 , 1);
		color = color + (Ls * KsOut * pow(dotp,NsOut));

		//color = texture2D(texture1,texturePosOut).rgb * color;
	}
}
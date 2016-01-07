#pragma once
#include <windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include "extra.h"

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alut.h"

using namespace std;

//#define COMPATIBILITY
#define MODERN

// include GLEW
#include "glew/glew.h"

// include GLFW
#include "glfw/glfw3.h"
GLFWwindow* window;

// include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"

#include "SOIL.h"

//OpenGL Global Variables
ShaderInfo info;
GLuint programId = NULL;
GLuint vertexArray = NULL;
GLuint LightPosId = NULL;
GLuint CameraId = NULL;
GLuint ViewId = NULL;
GLuint LaId = NULL;
GLuint LdId = NULL;
GLuint LsId = NULL;
GLuint MVPId = NULL;
GLuint ModelId = NULL;
GLuint drawBoundingId = NULL;

GLuint textShaderId = NULL;
GLuint drawAlphabetsId = NULL;
GLuint alphabetVertexBufferId = NULL;
GLuint alphabetUVBufferId = NULL;

glm::vec3 Camera(0,0,3);
glm::vec3 LookAt(0,0,0);

glm::vec3 LightPos(0,5,0);
glm::vec3 La(0.5f,0.5f,0.5f);
glm::vec3 Ld(1.0f,1.0f,1.0f);
glm::vec3 Ls(1.0f,1.0f,1.0f);

int texWidth, texHeight;

float TransX = 0, TransY = 0, TransZ = 0;
float ScaleXYZ = 1;
float RotateDegreeX = 0;
float RotateDegreeY = 0;

float GlobalWidth = 720.0f, GlobalHeight = 720.0f;

glm::mat4 TranslationMatrix;

glm::mat4 RotationMatrix1;
glm::mat4 RotationMatrix2;
glm::mat4 RotationMatrix;

glm::mat4 ScalingMatrix;

glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
glm::mat4 MVP;

//bool displayBounding = false;

int GlobalCounter = 0;
int GlobalCounter2 = 0;
int MaxGlobalCounter=0;
int TempMaxGlobalCounter=0;

glm::mat4 RotateAboutX =  glm::rotate(45.0f, glm::vec3 (1,0,0)); 
glm::mat4 RotateAboutY =  glm::rotate(-45.0f, glm::vec3 (0,1,0));

int tempQbertX,tempQbertY;

int PlayerScore = 0;
int Level = 0;
int Lives = 2;

double lastTimeGameOver;
double lastTimeCollided;
double lastTime;
double lastTimeLevelChange;

float GlobalScale = 0.9f;

bool GameOver = false;
bool Enemy1Spawned = false;
bool Enemy2Spawned = false;
bool Enemy3Spawned = false;

bool Collided = false;
bool LevelChanged = false;

bool DiskLeft = true;
bool DiskRight = true;

ALuint ALbuffer, ALsource;
ALuint ALstate;

ALuint ALbuffer2, ALsource2;
ALuint ALstate2;

vector<glm::vec2> alphabetVertices;
vector<glm::vec2> alphabetUV;

GLuint LoadShaders( const char* vertex_file_path, const char* fragment_file_path ){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	GLuint FragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

void init(CubeObject& obj) 
{
	
	drawBoundingId = glGetUniformLocation(programId, "drawBounding");
	drawAlphabetsId = glGetUniformLocation(programId, "drawAlphabets");

	glGenBuffers( 1, &obj.singleCube.info.vertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.vertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.VertexBuffer.size()) * sizeof(glm::vec3), &obj.singleCube.VertexBuffer[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.textureBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.textureBuffer );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.TextureBuffer.size()) * sizeof(glm::vec3), &obj.singleCube.TextureBuffer[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.normalBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.normalBuffer );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.NormalBuffer.size()) * sizeof(glm::vec3), &obj.singleCube.NormalBuffer[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.BufferKa );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferKa );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.VectorKa.size()) * sizeof(glm::vec3), &obj.singleCube.VectorKa[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.BufferKd );
	//glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferKd );
	//glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.VectorKd.size()) * sizeof(glm::vec3), &obj.singleCube.VectorKd[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.BufferKs );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferKs );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.VectorKs.size()) * sizeof(glm::vec3), &obj.singleCube.VectorKs[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.NsId );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.NsId );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.VectorNs.size()) * sizeof(float), &obj.singleCube.VectorNs[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.BufferIndex );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferIndex );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.TextureIndices.size()) * sizeof(float), &obj.singleCube.TextureIndices[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &obj.singleCube.info.boundingBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.boundingBuffer );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.BoundingVertices.size()) * sizeof(glm::vec3), &obj.singleCube.BoundingVertices[0], GL_STATIC_DRAW );

	glGenBuffers( 1, &alphabetVertexBufferId);
	glGenBuffers( 1, &alphabetUVBufferId);
}

void AssignBoundingToObject(CubeObject &obj)
{
	obj.singleCube.BoundingVertices[0] = glm::vec3 (obj.singleCube.minX,obj.singleCube.minY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[1] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.minY,obj.singleCube.minZ);

	obj.singleCube.BoundingVertices[2] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.minY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[3] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.maxY,obj.singleCube.minZ);

	obj.singleCube.BoundingVertices[4] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.maxY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[5] = glm::vec3 (obj.singleCube.minX,obj.singleCube.maxY,obj.singleCube.minZ);

	obj.singleCube.BoundingVertices[6] = glm::vec3 (obj.singleCube.minX,obj.singleCube.maxY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[7] = glm::vec3 (obj.singleCube.minX,obj.singleCube.minY,obj.singleCube.minZ);

	obj.singleCube.BoundingVertices[8] = glm::vec3 (obj.singleCube.minX,obj.singleCube.minY,obj.singleCube.maxZ);
	obj.singleCube.BoundingVertices[9] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.minY,obj.singleCube.maxZ);

	obj.singleCube.BoundingVertices[10] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.minY,obj.singleCube.maxZ);
	obj.singleCube.BoundingVertices[11] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.maxY,obj.singleCube.maxZ);

	obj.singleCube.BoundingVertices[12] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.maxY,obj.singleCube.maxZ);
	obj.singleCube.BoundingVertices[13] = glm::vec3 (obj.singleCube.minX,obj.singleCube.maxY,obj.singleCube.maxZ);

	obj.singleCube.BoundingVertices[14] = glm::vec3 (obj.singleCube.minX,obj.singleCube.maxY,obj.singleCube.maxZ);
	obj.singleCube.BoundingVertices[15] = glm::vec3 (obj.singleCube.minX,obj.singleCube.minY,obj.singleCube.maxZ);

	obj.singleCube.BoundingVertices[16] = glm::vec3 (obj.singleCube.minX,obj.singleCube.minY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[17] = glm::vec3 (obj.singleCube.minX,obj.singleCube.minY,obj.singleCube.maxZ);

	obj.singleCube.BoundingVertices[18] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.minY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[19] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.minY,obj.singleCube.maxZ);

	obj.singleCube.BoundingVertices[20] = glm::vec3 (obj.singleCube.minX,obj.singleCube.maxY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[21] = glm::vec3 (obj.singleCube.minX,obj.singleCube.maxY,obj.singleCube.maxZ);

	obj.singleCube.BoundingVertices[22] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.maxY,obj.singleCube.minZ);
	obj.singleCube.BoundingVertices[23] = glm::vec3 (obj.singleCube.maxX,obj.singleCube.maxY,obj.singleCube.maxZ);
}

void BeforeInit()
{
	glGenVertexArrays( 1, &vertexArray );
	glBindVertexArray( vertexArray );

	// create and compile our GLSL program from the shaders
	programId = LoadShaders( "shaders/vs.glsl", "shaders/fs.glsl" );

	// Use our shader
	glUseProgram( programId );

	ViewId = glGetUniformLocation(programId, "View");
	glUniformMatrix4fv(ViewId, 1, GL_FALSE, &View[0][0]);

	LaId = glGetUniformLocation(programId, "La");
	glUniform3f(LaId, La.x, La.y, La.z);

	LightPosId = glGetUniformLocation(programId, "LightPos");
	glUniform3f(LightPosId, LightPos.x, LightPos.y, LightPos.z);

	LdId = glGetUniformLocation(programId, "Ld");
	glUniform3f(LdId, Ld.x, Ld.y, Ld.z);

	LsId = glGetUniformLocation(programId, "Ls");
	glUniform3f(LsId, Ls.x, Ls.y, Ls.z);

	CameraId = glGetUniformLocation(programId, "CameraPos");
	glUniform3f(CameraId, Camera.x, Camera.y, Camera.z);

	MVPId = glGetUniformLocation(programId, "MVP");
	glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);
	
	ModelId = glGetUniformLocation(programId, "Model");
	glUniformMatrix4fv(ModelId, 1, GL_FALSE, &Model[0][0]);

	//LOADING FONT TEXTURES
	glGenTextures(1, &textShaderId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textShaderId);

	unsigned char *texture = SOIL_load_image("models//font.bmp", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//TEXTURE INFORMATION
	textShaderId = glGetUniformLocation(programId, "texture1");
	glUniform1i(textShaderId,0);

	//Initializing Object Buffers
	for(int i=0; i<Cubes.size(); i++)
		init(Cubes[i]);

	init(qbert);

	init(enemy1);

	init(enemy2);

	init(enemy3);

	init(Disks[0]);

	init(Disks[1]);

	//Referenced from => https://neppramod.wordpress.com/2012/08/04/openal-tutorial-using-alut/
	//~~~~~~~~~~~~~~SOUND~~~~~~~~~~~~~~~~~~~~~~~
	// Initialize the environment
    alutInit(0, NULL);

    // Capture errors
    alGetError();

    // Load pcm data into buffer
    ALbuffer = alutCreateBufferFromFile("models\\gameover.wav");

    // Create sound source (use buffer to fill source)
    alGenSources(1, &ALsource);
    alSourcei(ALsource, AL_BUFFER, ALbuffer);

	ALbuffer2 = alutCreateBufferFromFile("models\\explosion.wav");

    // Create sound source (use buffer to fill source)
    alGenSources(1, &ALsource2);
    alSourcei(ALsource2, AL_BUFFER, ALbuffer2);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Finding MAXIMUM number of children in a Tree
	/*for(int i=0; i<DisplayObjects.size(); i++)
	{
		TempMaxGlobalCounter = 0;
		FindMaxGlobalCounter(DisplayObjects[i]);
		
		if(TempMaxGlobalCounter > MaxGlobalCounter)
			MaxGlobalCounter = TempMaxGlobalCounter;
	}*/
}

void render3D( CubeObject& obj , int type) {
	
	//Calculating Model of CURRENT Object
	switch(type)
	{
		default: break;

		case 1:		//CUBES
			Model =  glm::translate(0.0f,0.3f,0.0f) *
					 RotateAboutX * 
					 RotateAboutY * 
					 obj.singleCube.transMatrix;
		break;

		case 2:		//QBERT
			Model = glm::translate(0.0f, 1*WorldScale, -2*WorldScale) *
					glm::translate(0.0f,0.3f,0.0f) *
				    RotateAboutX *
					RotateAboutY *
					obj.singleCube.transMatrix	*
					glm::scale(0.5f,0.5f,0.5f);					
		break;

		case 3:		//Enemy1
			Model = glm::translate(0.0f, 1*WorldScale, -2*WorldScale) *
					glm::translate(0.0f,0.3f,0.0f) *
				    RotateAboutX *
					RotateAboutY *
					obj.singleCube.transMatrix	*
					glm::scale(0.5f,0.5f,0.5f);					
		break;

		case 4:		//Transition
			Model = glm::translate(0.0f, 1*WorldScale, -2*WorldScale) *
					glm::translate(0.0f,0.3f,0.0f) *
				    RotateAboutX *
					RotateAboutY *
					obj.singleCube.transMatrix	*
					glm::scale(GlobalScale,GlobalScale,GlobalScale);					
		break;

	}
	
	//Model =  Transform * obj.singleCube.transMatrix;
	MVP = Projection * View * Model;	

	glUniform1i(drawBoundingId,0);
	glUniform1i(drawAlphabetsId,0);

	glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(ViewId, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(ModelId, 1, GL_FALSE, &Model[0][0]);

	// 1st attribute buffer : vertices
	glEnableVertexAttribArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.vertexBuffer );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);

	//2nd attribute Texture Coords
	glEnableVertexAttribArray( 1 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.textureBuffer );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);

	//3rd attribute Normals
	glEnableVertexAttribArray( 2 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.normalBuffer );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);	

	//4th attribute Ka
	glEnableVertexAttribArray( 3 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferKa );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);

	//5th attribute Kd
	glEnableVertexAttribArray( 4 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferKd );
	glBufferData( GL_ARRAY_BUFFER, (obj.singleCube.VectorKd.size()) * sizeof(glm::vec3), &obj.singleCube.VectorKd[0], GL_STATIC_DRAW );
	glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);

	//6th attribute Ks
	glEnableVertexAttribArray( 5 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferKs );
	glVertexAttribPointer( 5, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);

	//7th NS Values (Specular)
	glEnableVertexAttribArray( 6 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.NsId );
	glVertexAttribPointer( 6, 1, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);

	//8th Texture Index Value
	glEnableVertexAttribArray( 7 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.BufferIndex );
	glVertexAttribPointer( 7, 1, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);

	//9th Texture Bounding BUffer
	glEnableVertexAttribArray( 8 );
	glBindBuffer( GL_ARRAY_BUFFER, obj.singleCube.info.boundingBuffer );
	glVertexAttribPointer( 8, 3, GL_FLOAT, GL_FALSE, 0, ( void* ) 0	);
	
	// draw the triangles
	glDrawArrays( GL_TRIANGLES, 0, obj.singleCube.VertexBuffer.size());


	//if(displayBounding && obj.singleCube.ShowBounding)
	//{
	//	glUniform1i(drawBoundingId,1);

	//	//Changing MVP as Bounding Vertices are already multiplied with Model Transformations
	//	MVP = Projection * View * obj.singleCube.transMatrix;
	//	glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);

	//	glDrawArrays( GL_LINES, 0, obj.singleCube.BoundingVertices.size());
	//}

	glDisableVertexAttribArray( 0 );
	glDisableVertexAttribArray( 1 );
	glDisableVertexAttribArray( 2 );
	glDisableVertexAttribArray( 3 );
	glDisableVertexAttribArray( 4 );
	glDisableVertexAttribArray( 5 );
	glDisableVertexAttribArray( 6 );
	glDisableVertexAttribArray( 7 );
	glDisableVertexAttribArray( 8 );
}

//DISPLAYING TEXT ON SCREEN
void displayText(const char* text, int x, int y, int size)
{
	unsigned int textLength = strlen(text);

	for ( unsigned int i=0 ; i < textLength ; i++ )
	{		
		glm::vec2 vertexTopLeft = glm::vec2( x+i*size, y+size);
		glm::vec2 vertexBotLeft = glm::vec2( x+i*size, y);
		
		glm::vec2 vertexTopRight = glm::vec2( x+i*size+size, y+size );
		glm::vec2 vertexBotRight = glm::vec2( x+i*size+size, y);
		
		alphabetVertices.push_back(vertexTopLeft);
		alphabetVertices.push_back(vertexBotLeft );
		alphabetVertices.push_back(vertexTopRight);
		alphabetVertices.push_back(vertexBotRight);
		alphabetVertices.push_back(vertexTopRight);
		alphabetVertices.push_back(vertexBotLeft);

		char character = text[i];
		float uv_x = (character%16)/16.0f;
		float uv_y = (character/16 + 1)/16.0f;
		uv_y = 1.0 - uv_y;
		
		glm::vec2 upLeft = glm::vec2(uv_x, uv_y + (1.0f/16.0f)); 
		glm::vec2 downLeft = glm::vec2(uv_x, uv_y);
		glm::vec2 upRight = glm::vec2(uv_x + (1.0f/16.0f), uv_y + (1.0f/16.0f));
		glm::vec2 downRight = glm::vec2(uv_x + (1.0f/16.0f), uv_y);
		
		alphabetUV.push_back(upLeft);
		alphabetUV.push_back(downLeft);
		alphabetUV.push_back(upRight);
		alphabetUV.push_back(downRight);
		alphabetUV.push_back(upRight);
		alphabetUV.push_back(downLeft);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textShaderId);

		glEnableVertexAttribArray( 9 );
		glBindBuffer( GL_ARRAY_BUFFER, alphabetUVBufferId);
		glBufferData( GL_ARRAY_BUFFER, alphabetUV.size()*sizeof(glm::vec2), &alphabetUV[0], GL_STATIC_DRAW);
		
		glBindBuffer( GL_ARRAY_BUFFER, alphabetVertexBufferId);
		glBufferData( GL_ARRAY_BUFFER, alphabetVertices.size()*sizeof(glm::vec2), &alphabetVertices[0], GL_STATIC_DRAW);
		
		glBindBuffer( GL_ARRAY_BUFFER, alphabetUVBufferId);
		glVertexAttribPointer( 9, 2, GL_FLOAT,GL_FALSE, 0, ( void* ) 0);
		
		glEnableVertexAttribArray( 10 );
		glBindBuffer( GL_ARRAY_BUFFER, alphabetVertexBufferId);
		glVertexAttribPointer( 10, 2, GL_FLOAT,GL_FALSE, 0, ( void* ) 0);
		
		glUniform1i(glGetUniformLocation(programId,"drawAlphabets"),1);

		glDrawArrays(GL_TRIANGLES, 0, alphabetVertices.size());
		
		glDisableVertexAttribArray( 9 );
		glDisableVertexAttribArray( 10 );
		
		alphabetVertices.clear();
		alphabetUV.clear();
	}	
}

void ResetGame(bool trueReset)
{
	if(trueReset)
	{
		qbert.Xval = qbert.Yval = 0;
		qbert.singleCube.transMatrix = Cubes[0].singleCube.transMatrix;

		DiskLeft = DiskRight = true;

		//Reset Game according to CURRENT Level
		PlayerScore = Level*28;

		//CHANGING TOUCHED-CUBE States
		for(int i=0; i<Cubes.size(); i++)
		{
			Cubes[i].state = false;

			//Changing Cube Color to Original
			for(int j=0; j<Cubes[i].singleCube.VectorKd.size(); j++)
				Cubes[i].singleCube.VectorKd[j] = glm::vec3(1.0f,0.0f,0.0f);
		}
	}
	
	else
	{
		alSourcePlay(ALsource2);

		qbert.Xval = tempQbertX;
		qbert.Yval = tempQbertY;

		for(int i=0; i<Cubes.size(); i++)
		{
			if(qbert.Xval==Cubes[i].Xval && qbert.Yval==Cubes[i].Yval)
			{
				qbert.singleCube.transMatrix = Cubes[i].singleCube.transMatrix;
				break;
			}
		}
	}	
	
	//RESET Enemies
	Enemy1Spawned = false;
	Enemy2Spawned = false;
	Enemy3Spawned = false;

	//Reset TIME to Re-Spawn Enemies
	lastTime = glfwGetTime();
}

void NextLevel()
{
	Level++;
	ResetGame(true);

	LevelChanged = true;
	lastTimeLevelChange = glfwGetTime();
}

void FinishGame()
{
	alSourcePlay(ALsource);

	Lives = 2;
	Level = 0;
	
	ResetGame(true);

	GameOver = true;

	//Starting Game Over Timer to DISPLAY for 3 seconds
	lastTimeGameOver = glfwGetTime();
}

void QbertInTrouble()
{
	Lives--;

	if(Lives < 0)
	{
		FinishGame();
		return;
	}

	ResetGame(false);

	Collided = true;
	lastTimeCollided = glfwGetTime();
}

void ManageQbert()
{
	cout<<"\nQbert=> X: "<<qbert.Xval<<", Y: "<<qbert.Yval;

	//Left Disk
	if(qbert.Xval == -1 && qbert.Yval == 4 && DiskLeft)
	{
		qbert.Xval = qbert.Yval = 0;
		qbert.singleCube.transMatrix = Cubes[0].singleCube.transMatrix;
		DiskLeft = false;
		return;
	}

	//Right Disk
	if(qbert.Xval == 4 && qbert.Yval == -1 && DiskRight)
	{
		qbert.Xval = qbert.Yval = 0;
		qbert.singleCube.transMatrix = Cubes[0].singleCube.transMatrix;
		DiskRight = false;
		return;
	}

	//Qbert FALLING
	if(qbert.Xval<0 || qbert.Xval>6 || qbert.Yval<0 || qbert.Yval>6 || (qbert.Xval+qbert.Yval > 6))
	{
		QbertInTrouble();
		return;
	}

	//Qbert on a VALID Cube
	for(int i=0; i<Cubes.size(); i++)
	{
		if(qbert.Xval==Cubes[i].Xval && qbert.Yval==Cubes[i].Yval)
		{
			//qbert.singleCube.transMatrix = glm::translate(0.0f,2*WorldScale,0.0f) * Cubes[i].singleCube.transMatrix;
			//CubeOnTop = Cubes[i].singleCube.transMatrix;
			qbert.singleCube.transMatrix = Cubes[i].singleCube.transMatrix;
			
			//CUBE HASN'T BEEN VISITED
			if (Cubes[i].state == false)
			{
				Cubes[i].state = true;
				PlayerScore++;

				if(PlayerScore == (Level+1)*28)
				{
					NextLevel();
					return;
				}

				//Changing Cube Color on Touching
				for(int j=0; j<Cubes[i].singleCube.VectorKd.size(); j++)
					Cubes[i].singleCube.VectorKd[j] = glm::vec3(0.0f,0.0f,1.0f);
			}			
			break;
		}
	}	
}

void SpawnEnemy1()
{
	int temp = rand()%3;
	enemy1.singleCube.transMatrix = Cubes[temp].singleCube.transMatrix;
	enemy1.Xval = Cubes[temp].Xval;
	enemy1.Yval = Cubes[temp].Yval;

	if(temp == 1)
	{
		enemy1.Xdirection = 0;
		enemy1.Ydirection = 1;
	}
	else if (temp == 2)
	{
		enemy1.Xdirection = 1;
		enemy1.Ydirection = 0;
	}
	else if(temp == 0)
	{
		int temp2 = rand()%2;

		if(temp2 == 0)
		{
			enemy1.Xdirection = 0;
			enemy1.Ydirection = 1;
		}
		else
		{
			enemy1.Xdirection = 1;
			enemy1.Ydirection = 0;
		}
	}
}

void SimulateEnemy1()
{
	enemy1.Yval += enemy1.Ydirection;
	enemy1.Xval += enemy1.Xdirection;

	if(enemy1.Ydirection == 1 && enemy1.Yval>6)
	{
		enemy1.Yval = 5;
		enemy1.Ydirection = -1;
	}

	else if(enemy1.Ydirection == -1 && enemy1.Yval<0)
	{
		enemy1.Xval = 1;
		enemy1.Yval = 0;
		enemy1.Ydirection = 0;
		enemy1.Xdirection = 1;
	}

	else if (enemy1.Xdirection == 1 && enemy1.Xval>6)
	{
		enemy1.Xval = 5;
		enemy1.Xdirection = -1;
	}
	
	else if (enemy1.Xdirection == -1 && enemy1.Xval<0)
	{
		enemy1.Yval = 1;
		enemy1.Xval = 0;
		enemy1.Xdirection = 0;
		enemy1.Ydirection = 1;
	}

	for(int i=0; i<Cubes.size(); i++)
	{
		if(enemy1.Xval==Cubes[i].Xval && enemy1.Yval==Cubes[i].Yval)
		{
			enemy1.singleCube.transMatrix = Cubes[i].singleCube.transMatrix;
			break;
		}
	}
}

void SpawnEnemy2()
{
	enemy2.Xval = enemy2.Yval = 0;
	enemy2.singleCube.transMatrix = Cubes[0].singleCube.transMatrix;
}

void SimulateEnemy2()
{
	int temp = rand()%2;

	if(temp == 1)		//Move Down
	{
		enemy2.Yval++;
	}
	else				//Move Right
	{
		enemy2.Xval++;
	}

	//IF Enemy2 Fell Off moving Down/Right
	if(enemy2.Xval>6 || enemy2.Yval>6 || (enemy2.Xval+enemy2.Yval > 6))
	{
		Enemy2Spawned = false;
		lastTime = glfwGetTime();
		return;
	}

	//Finding Place for Enemy2
	for(int i=0; i<Cubes.size(); i++)
	{
		if(enemy2.Xval==Cubes[i].Xval && enemy2.Yval==Cubes[i].Yval)
		{
			enemy2.singleCube.transMatrix = Cubes[i].singleCube.transMatrix;
			break;
		}
	}
}

void SpawnEnemy3()
{
	enemy3.Xval = enemy2.Yval = 2;
	enemy3.singleCube.transMatrix = Cubes[12].singleCube.transMatrix;
}

void SimulateEnemy3()
{
	if(Level == 1)
	{
		int Xdifference = qbert.Xval - enemy3.Xval;
		int Ydifference = qbert.Yval - enemy3.Yval;

		if(Xdifference == 0)
		{
			if(Ydifference<0)
				enemy3.Yval--;
			else
				enemy3.Yval++;
		}
		else if (Xdifference < 0)
		{
			enemy3.Xval--;
		}
		else
		{
			if(enemy3.Xval + enemy3.Yval == 6)
				enemy3.Yval--;
			else
				enemy3.Xval++;
		}

		//IF Enemy3 Fell Off moving Down/Right
		if(enemy3.Xval<0 || enemy3.Xval>6 || enemy3.Yval<0 || enemy3.Yval>6 || (enemy3.Xval+enemy3.Yval > 6))
		{
			Enemy3Spawned = false;
			lastTime = glfwGetTime();
			return;
		}
	}

	else if (Level > 1)
	{
		int tempX = enemy3.Xval;
		int tempY = enemy3.Yval;

		while(1)
		{
			int rand1 = rand()%2;
			int rand2 = rand()%2;

			if(rand1 == 0)
			{
				if (rand2 == 0)
					tempX--;
				else
					tempX++;
			}
			else
			{
				if(rand2 == 0)
					tempY--;
				else
					tempY++;
			}
			
			if(tempX<0 || tempX>6 || tempY<0 || tempY>6 || (tempX+tempY > 6))
				continue;
			
			else
			{
				enemy3.Xval = tempX;
				enemy3.Yval = tempY;
				break;
			}				
		}
	}

	//Finding Place for Enemy3
	for(int i=0; i<Cubes.size(); i++)
	{
		if(enemy3.Xval==Cubes[i].Xval && enemy3.Yval==Cubes[i].Yval)
		{
			enemy3.singleCube.transMatrix = Cubes[i].singleCube.transMatrix;
			break;
		}
	}
}

void DetectCollisions()
{
	//Collision with Enemy1
	if(Enemy1Spawned && qbert.Xval == enemy1.Xval && qbert.Yval == enemy1.Yval)
	{
		tempQbertX = qbert.Xval;
		tempQbertY = qbert.Yval;

		QbertInTrouble();
		return;
	}

	//Collision with Enemy2
	if(Enemy2Spawned && qbert.Xval == enemy2.Xval && qbert.Yval == enemy2.Yval)
	{
		tempQbertX = qbert.Xval;
		tempQbertY = qbert.Yval;

		QbertInTrouble();
		return;
	}

	//Collision with Enemy3
	if(Enemy3Spawned && qbert.Xval == enemy3.Xval && qbert.Yval == enemy3.Yval)
	{
		tempQbertX = qbert.Xval;
		tempQbertY = qbert.Yval;

		QbertInTrouble();
		return;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if(GameOver || Collided)
		return;

	tempQbertX = qbert.Xval;
	tempQbertY = qbert.Yval;

		switch (key)
		{
			default: break;
			
			case GLFW_KEY_UP: 
				if(action == GLFW_PRESS)
				{
					qbert.Yval--;
					ManageQbert();
				}
				break;

			case GLFW_KEY_DOWN:
				if(action == GLFW_PRESS)
				{
					qbert.Yval++;
					ManageQbert();
				}
				break;

			case GLFW_KEY_LEFT: 
				if(action == GLFW_PRESS)
				{
					qbert.Xval--;
					ManageQbert();
				}

				break;

			case GLFW_KEY_RIGHT:
				if(action == GLFW_PRESS)
				{
					qbert.Xval++;
					ManageQbert();
				}
				break;
		}	
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	GlobalWidth = width;
	GlobalHeight = height;
    glViewport(0, 0, width, height);
}

int main( int argc, char** argv )
{
	srand(time(NULL));

	// initialize GLFW
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW!\n" );
		return -1;
	}

	glfwWindowHint( GLFW_SAMPLES, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	// open a window and create its OpenGL context
	window = glfwCreateWindow( 720, 720, "CSC561 - Game", NULL, NULL );
	if( window == NULL ) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent( window );

	// initialize GLEW
	glewExperimental = true; // Needed for core profile
	if( glewInit() != GLEW_OK ) {
		fprintf( stderr, "Failed to initialize GLEW\n" );
		return -1;
	}

	// ensure we can capture the escape key being pressed below
	glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE );

	//Referenced from => http://www.glfw.org/docs/latest/input.html#input_keyboard
	glfwSetKeyCallback(window, key_callback);

	//Referenced from => http://www.glfw.org/docs/latest/window.html#window_events
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// set the GL clear color
	glClearColor( 0.f, 0.f, 0.f, 0.f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glfwSwapBuffers( window );
	
	//MAIN PARSING
	ParseCubes();
	ParseQbert();

	ParseEnemy1();
	ParseEnemy2();
	ParseEnemy3();
	
	ParseDisks();

	//MAIN INITIALIZATION
	BeforeInit();

	//FPS Reference From: http://www.opengl-tutorial.org/miscellaneous/an-fps-counter/
	lastTime = glfwGetTime();

	//Setting View and Projection Matrices
	View = glm::mat4(1.0f);
	Projection = glm::mat4(1.0f);

	//Initializing Timer Counters
	double lastTimeEnemy1 = glfwGetTime();
	double lastTimeEnemy2 = glfwGetTime();
	double lastTimeEnemy3 = glfwGetTime();

	lastTimeLevelChange = glfwGetTime();
	lastTimeCollided = glfwGetTime();

	//int nbFrames = 0;

	string displayString;

	do {
		// clear the screen
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		//ANIMATION FOR LEVEL CHANGE
		double currentTimeLevelChanged = glfwGetTime();
		if (LevelChanged && (currentTimeLevelChanged - lastTimeLevelChange <= 2.0))
		{
			for(int i=0; i<Cubes.size(); i++)
			{
				render3D(Cubes[i],4);
			}

			GlobalScale -= 0.05f;

			lastTime = glfwGetTime();
		}

		if (LevelChanged && (currentTimeLevelChanged - lastTimeLevelChange > 2.0))
		{
			LevelChanged = false;
			GlobalScale = 0.9f;
			lastTime = glfwGetTime();
		}

		//WHEN COLLIDING OR FALLING DOWN
		double currentTimeCollided = glfwGetTime();
		if (Collided && (currentTimeCollided - lastTimeCollided <= 2.0))
		{
			displayText("RE-SPAWN!",250,450,40);
			lastTime = glfwGetTime();
		}

		if (Collided && (currentTimeCollided - lastTimeCollided > 2.0))
		{
			Collided = false;
			lastTime = glfwGetTime();
		}

		//FOR GAME OVER
		double currentTimeGameOver = glfwGetTime();
		if(GameOver && (currentTimeGameOver - lastTimeGameOver <= 3.0))
		{
			displayText("GAME OVER!",250,450,40);			

			lastTime = glfwGetTime();
		}

		if(GameOver && (currentTimeGameOver - lastTimeGameOver > 3.0))
		{
			GameOver = false;
			lastTime = glfwGetTime();
		}

		//DRAW STUFF
		render3D(qbert,2);

		if(Enemy1Spawned)
			render3D(enemy1,3);

		if(Enemy2Spawned)
			render3D(enemy2,3);

		if(Enemy3Spawned)
			render3D(enemy3,3);

		if(DiskLeft)
			render3D(Disks[0],1);

		if(DiskRight)
			render3D(Disks[1],1);

		for(int i=0; i<Cubes.size(); i++)
		{
			render3D(Cubes[i],1);
		}
		//RENDERING ENDS

		//DETECTING COLLISIONS
		DetectCollisions();
		
		if(!GameOver)
		{
			//Display SCORE
			displayString = "Score:"+to_string(PlayerScore);
			displayText(displayString.c_str(),300,450,30);

			//Display LEVEL
			displayString = "Level:"+to_string(Level);
			displayText(displayString.c_str(),300,500,30);

			//Display LIVES
			if (Lives>0)
			displayString = "Lives Remaining:"+to_string(Lives);
			else
			displayString = "Lives Remaining: NONE!";

			displayText(displayString.c_str(),120,550,30);
		}
		
		// swap buffers
		glfwSwapBuffers( window );
		glfwPollEvents();

		double currentTime = glfwGetTime();

       if ( currentTime - lastTime >= 4.0 )
	   {
          //printf("%f ms/frame\n", 1000.0/double(nbFrames));
          //nbFrames = 0;
		   if(!Enemy1Spawned)
		   {
			   cout<<"SPAWN Enemy1";
			   SpawnEnemy1();
			   Enemy1Spawned = true;
			   lastTimeEnemy1 = glfwGetTime();
		   }

		   if(!Enemy2Spawned)
		   {
			   cout<<"Spawn Enemy2";
			   SpawnEnemy2();
			   Enemy2Spawned = true;
			   lastTimeEnemy2 = glfwGetTime();
		   }

		   if(!Enemy3Spawned && Level>0)
		   {
			   cout<<"Spawn Enemy3";
			   SpawnEnemy3();
			   Enemy3Spawned = true;
			   lastTimeEnemy3 = glfwGetTime();
		   }

          lastTime += 4.0;
	   }

	   double currentTimeEnemy1 = glfwGetTime();
	   if ((currentTimeEnemy1 - lastTimeEnemy1 >= 1.0))
	   {
		   if(Enemy1Spawned)
				SimulateEnemy1();

		   lastTimeEnemy1 += 1.0;
	   }

	   double currentTimeEnemy2 = glfwGetTime();
	   if ((currentTimeEnemy2 - lastTimeEnemy2 >= 1.0))
	   {
		   if(Enemy2Spawned)
				SimulateEnemy2();

		   lastTimeEnemy2 += 1.0;
	   }

	   double currentTimeEnemy3 = glfwGetTime();
	   if ((currentTimeEnemy3 - lastTimeEnemy3 >= 2.0))
	   {
		   if(Enemy3Spawned)
				SimulateEnemy3();

		   lastTimeEnemy3 += 2.0;
	   }
		
	  } // check if the ESC key was pressed or the window was closed
	while( glfwGetKey( window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose( window ) == 0 );

	// close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

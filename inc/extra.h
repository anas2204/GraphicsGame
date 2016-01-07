#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <ctime>

#include "glm\glm.hpp"

#include "glew/glew.h"
#include "glfw/glfw3.h"

using namespace std;

struct Vertex
{
    float x;
    float y;
    float z;

	void CopyVertex(glm::vec3 vertex)
	{
		x = vertex.x;
		y = vertex.y;
		z = vertex.z;
	}
};

struct Mtlobject
{
	glm::vec3 ka;
    glm::vec3 kd;
    glm::vec3 ks;
    float ns;
	string textureFile;

	Mtlobject()
	{
		ka = glm::vec3(0.1f,0.1f,0.1f);
		ks = glm::vec3(0.2f,0.2f,0.2f);
		ns=0.0f;
	}
};

struct Face
{
	glm::vec3 triangle[3];			//Vertices
	glm::vec3 vertexNormals[3];		//Vertex Normals
	glm::vec3 textureVertices[3];	//Texture Vertices

	glm::vec3 flag;					//Shows if Vertex Normals and Texture Vertices are Enabled=> X = Vertices, Y = Texture, Z = Normals

	Face()
	{
		flag.x = flag.y = flag.z = 0;		
	}

	
	void fillVertices(Vertex v1, Vertex v2, Vertex v3)
	{
		flag.x = 1;
		triangle[0].x = v1.x;	triangle[0].y = v1.y;	triangle[0].z = v1.z;
		triangle[1].x = v2.x;	triangle[1].y = v2.y;	triangle[1].z = v2.z;
		triangle[2].x = v3.x;	triangle[2].y = v3.y;	triangle[2].z = v3.z;
	}

	void fillTextureVertices(Vertex vt1, Vertex vt2, Vertex vt3)
	{
		flag.y = 1;
		textureVertices[0].x = vt1.x;	textureVertices[0].y = vt1.y;	textureVertices[0].z = vt1.z;
		textureVertices[1].x = vt2.x;	textureVertices[1].y = vt2.y;	textureVertices[1].z = vt2.z;
		textureVertices[2].x = vt3.x;	textureVertices[2].y = vt3.y;	textureVertices[2].z = vt3.z;
	}

	void fillNormals(Vertex vn1, Vertex vn2, Vertex vn3)
	{
		flag.z = 1;
		vertexNormals[0].x = vn1.x;	vertexNormals[0].y = vn1.y;	vertexNormals[0].z = vn1.z;
		vertexNormals[1].x = vn2.x;	vertexNormals[1].y = vn2.y;	vertexNormals[1].z = vn2.z;
		vertexNormals[2].x = vn3.x;	vertexNormals[2].y = vn3.y;	vertexNormals[2].z = vn3.z;
	}
};

struct Group
{
    string mtlName;
    vector<Face> faces;
};


struct ShaderInfo {
	GLuint vertexBuffer;
	GLuint textureBuffer;
	GLuint normalBuffer;
	GLuint boundingBuffer;

	GLuint BufferKa;
	GLuint BufferKd;
	GLuint BufferKs;
	GLuint BufferNs;

	GLuint NsId;

	GLuint Shader1;
	//GLuint Shader2;

	GLuint BufferIndex;
};

struct ObjectHierarchy
{
	string fileName;
	glm::mat4 transMatrix;
	int numberOfChildren;

	float minX,minY,minZ;
	float maxX,maxY,maxZ;

	ShaderInfo info;

	//Object Related Information
	vector<glm::vec3> VertexBuffer;
	vector<glm::vec3> TextureBuffer;
	vector<glm::vec3> NormalBuffer;
	vector<glm::vec3> VectorKa;
	vector<glm::vec3> VectorKd;
	vector<glm::vec3> VectorKs;
	vector<float> VectorNs;

	unordered_map <string,int> TextureFiles;
	vector<int> TextureIndices;

	vector<ObjectHierarchy> ChildrenObjects;

	//Bounding Box Hierarchy
	vector <glm::vec3> BoundingVertices;
	bool ShowBounding;

	ObjectHierarchy()
	{
		BoundingVertices.assign(24,glm::vec3(0,0,0));
		ShowBounding = false;

		numberOfChildren = 0;
		info.vertexBuffer = NULL;
		info.textureBuffer = NULL;
		info.normalBuffer = NULL;
		info.boundingBuffer = NULL;

		info.BufferKa = NULL;
	    info.BufferKd = NULL;
	    info.BufferKs = NULL;
	    info.BufferNs = NULL;

		info.NsId = NULL;
		info.Shader1 = NULL;
	}
};


struct CubeObject
{
	ObjectHierarchy singleCube;	//ALSO CONTAINS TRANSFORMATION For Cube
	float Xval, Yval;	//Cube Positions	
	bool state;	//If stepped upon or not

	CubeObject()
	{
		Xval = 0.0f;
		Yval = 0.0f;
		state = false;	//Not stepped Upon
	}
};

struct Qbert:public CubeObject
{
	Qbert()
	{
	}
};

struct Enemy1: public CubeObject
{
	int Xdirection;
	int Ydirection;

	Enemy1()
	{
		Xdirection = Ydirection = -2;
	}

};

struct LightSource
{
	glm::vec3 position;
	glm::vec3 la;
	glm::vec3 ld;
	glm::vec3 ls;
};


extern vector <CubeObject> Cubes;
extern Qbert qbert;
extern Enemy1 enemy1;
extern Enemy1 enemy2;
extern Enemy1 enemy3;

extern CubeObject Disks[2];

extern vector <ObjectHierarchy> DisplayObjects;
extern string HierarchyFileName;

extern vector <Vertex> Vertices;
extern vector <Vertex> TextureVertices;
extern vector <Vertex> VertexNormals;

extern vector<glm::vec3> VertexBuffer;
extern vector<glm::vec3> TextureBuffer;
extern vector<glm::vec3> NormalBuffer;
extern vector<glm::vec3> VectorKa;
extern vector<glm::vec3> VectorKd;
extern vector<glm::vec3> VectorKs;
extern vector<float> VectorNs; 

extern unordered_map <string,int> TextureFiles;
extern vector<int> TextureIndices;

extern unordered_map <string,Group> Groups;
extern unordered_map <string,Mtlobject> MtlArray;

extern vector <LightSource> Lights;

extern float Width;
extern float Height;
extern float Xmin;
extern float Xmax;
extern float Ymin;
extern float Ymax;

void ParseHierarchy (vector <ObjectHierarchy> &Objects, int);
void StartParsing();
void ParseCubes();
void ParseQbert();
void ParseEnemy1();
void ParseEnemy2();
void ParseEnemy3();

void ParseDisks();

extern float WorldScale;
extern float QbertScale;

void ParseFunc(string);
void ParseLight(string);
void ParseWindow(string);
void ParseProject(string);

void printVec3 (glm::vec3 &vect);
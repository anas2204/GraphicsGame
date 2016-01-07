#include <vector>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unordered_map>
#include <algorithm>
#include <string>
#include "extra.h"
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\type_ptr.hpp"

using namespace std;

string HierarchyFileName;
ifstream HierarchyFile;
string Hierarchyline;
string dir = "models\\";

vector <Vertex> Vertices;
vector <Vertex> TextureVertices;
vector <Vertex> VertexNormals;

vector<glm::vec3> VertexBuffer;
vector<glm::vec3> TextureBuffer;
vector<glm::vec3> NormalBuffer;
vector<glm::vec3> VectorKa;
vector<glm::vec3> VectorKd;
vector<glm::vec3> VectorKs;
vector<float> VectorNs;

vector<int> TextureIndices;
unordered_map <string,int> TextureFiles;

unordered_map <string,Group> Groups;
unordered_map <string,Mtlobject> MtlArray;

//Creating Hierarchy Objects
vector <ObjectHierarchy> DisplayObjects;

//Creating Cubes
vector <CubeObject> Cubes;

//Qbert Object
Qbert qbert;

//Enemies
Enemy1 enemy1;
Enemy1 enemy2;
Enemy1 enemy3;

//Spinning Disks
CubeObject Disks[2];

int GlobalTextureIndex=0;

float WorldScale=0.07;
float QbertScale = 0.05;

vector <LightSource> Lights;

float Width;
float Height;
float Xmin;
float Xmax;
float Ymin;
float Ymax;

//Prints a glm::vec3
void printVec3 (glm::vec3 &vect)
{
	cout<<vect.x<<", "<<vect.y<<", "<<vect.z<<endl;
}

//ParseFloat and ParseFaces functions have been referenced from use of strtok() function => http://www.cplusplus.com/reference/string/string/c_str/
void ParseFloat(float arr[], string line)
{
    char *tempFloat,*temp;
    temp = new char[line.length()+1];

    strcpy(temp,line.c_str());

    tempFloat = strtok(temp," ");           //Removes the 1st Word => v,f,Ka,Kd,Ks

    tempFloat = strtok(NULL," ");           //Gets the 1st value of the Line

    int k=0;
    while(tempFloat)
    {
        arr[k++] = atof(tempFloat);
        tempFloat = strtok(NULL," ");
    }
    delete []temp;
}

inline void ParseInt(int arr[], string line, const char* delim)
{
    char *tempInt,*temp;
    temp = new char[line.length()+1];

    strcpy(temp,line.c_str());

    //tempInt = strtok(temp,delim);           //Removes the 1st Word => v,f,Ka,Kd,Ks

    tempInt = strtok(temp,delim);           //Gets the 1st value of the Line

    int k=0;
    while(tempInt)
    {
        arr[k++] = atof(tempInt);
        tempInt = strtok(NULL,delim);
    }
    delete []temp;
}

inline void PopulateFaces(string s1, int &v, int &vt, int &vn)
{
	//f=>1/1/1 2/2/2 3/3/3
	if(s1.find_first_of("/") != s1.find_last_of("/"))
	{
		int arr[3];
		ParseInt(arr,s1,"/");
		v = arr[0];	
		vt = arr[1];
		vn = arr[2];
	}

	if(s1.length() == 1)
	{
		v = atoi(s1.c_str());
		vt = vn = -1;
	}
}

void AdjustModel()
{
	float maximumX=INT_MIN, minimumX=INT_MAX, maximumY=INT_MIN, minimumY=INT_MAX, maximumZ=INT_MIN, minimumZ=INT_MAX;

	for(int i=0; i<VertexBuffer.size(); i++)
	{
		maximumX = (VertexBuffer[i].x > maximumX)? VertexBuffer[i].x : maximumX;
		minimumX = (VertexBuffer[i].x < minimumX)? VertexBuffer[i].x : minimumX;
		maximumY = (VertexBuffer[i].y > maximumY)? VertexBuffer[i].y : maximumY;
		minimumY = (VertexBuffer[i].y < minimumY)? VertexBuffer[i].y : minimumY;
		maximumZ = (VertexBuffer[i].z > maximumZ)? VertexBuffer[i].z : maximumZ;
		minimumZ = (VertexBuffer[i].z < minimumZ)? VertexBuffer[i].z : minimumZ;
	}

	float Xorigin = (maximumX + minimumX)/2.0f;
	float Yorigin = (maximumY + minimumY)/2.0f;
	float Zorigin = (maximumZ + minimumZ)/2.0f;


	float Xdifference = maximumX - minimumX;
	float Ydifference = maximumY - minimumY;
	float Zdifference = maximumZ - minimumZ;

	float scale = max (Xdifference, max(Ydifference,Zdifference));

	cout<<endl<<minimumX<<","<<maximumX<<endl;
	cout<<minimumY<<","<<maximumY<<endl;
	cout<<minimumZ<<","<<maximumZ<<endl;

	cout<<scale<<endl;

	for(int i=0; i<VertexBuffer.size(); i++)
	{
		VertexBuffer[i].x -= Xorigin;
		VertexBuffer[i].y -= Yorigin;
		VertexBuffer[i].z -= Zorigin;
	}

	for(int i=0; i<VertexBuffer.size(); i++)
	{
		VertexBuffer[i].x *= (2.0f/scale);
		VertexBuffer[i].y *= (2.0f/scale);
		VertexBuffer[i].z *= (2.0f/scale);
	}
}

void MTLParse (string fileName)
{
    //Loading MTL File
    ifstream mtlFile(fileName.c_str());

    if(mtlFile.fail())
        cout<<"MTL File Failed to Load";

    string line;

    while(getline(mtlFile,line))
    {
        if(line.find("newmtl")!=string::npos)
        {
            Mtlobject mtlobj;
            string name = line.substr(line.find(" ")+1);        //Extracting Name of MTL Group
            float temp[3];

            while(getline(mtlFile,line))
            {
                if(line.empty())
                    break;

                else if(line.find("Ka")!=string::npos)
                {
                    ParseFloat(temp,line);
					mtlobj.ka.r = temp[0];
					mtlobj.ka.g = temp[1];
					mtlobj.ka.b = temp[2];
					continue;
                }
                else if(line.find("Kd")!=string::npos && line[0] == 'K')
                {
                    ParseFloat(temp,line);
					mtlobj.kd.r = temp[0];
					mtlobj.kd.g = temp[1];
					mtlobj.kd.b = temp[2];
					continue;
                }
                else if(line.find("Ks")!=string::npos)
                {
                    ParseFloat(temp,line);
					mtlobj.ks.r = temp[0];
					mtlobj.ks.g = temp[1];
					mtlobj.ks.b = temp[2];
					continue;
                }
                else if (line.find("Ns")!=string::npos)
                {
                    string tempstr = line.substr(line.find(" ")+1);
                    mtlobj.ns = (float)atof(tempstr.c_str());
					continue;
                }
                else if (line[0] == 'N' && line[1] == ' ')       //N is given
                {
                    string tempstr = line.substr(line.find(" ")+1);
                    mtlobj.ns = ((float)atof(tempstr.c_str())*128)/1000;
                }
				else if (line.find("map_Kd")!=string::npos)
                {
                    string tempstr = line.substr(line.find(" ")+1);
					mtlobj.textureFile = tempstr;
					TextureFiles[tempstr] = GlobalTextureIndex;
					cout<<"\nGlobal Index: "<<GlobalTextureIndex<<endl;
					GlobalTextureIndex++;
                }
            }
            MtlArray.insert(unordered_map<string,Mtlobject>::value_type(name,mtlobj));
        }
    }
    mtlFile.close();
}

//***********************MAIN PARSE FUNCTION*******************
void ParseFunc(string fileName)
{
    string line;

    //MAIN OBJ FILE
    ifstream objectFile(fileName);

    if(objectFile.fail())
        cout<<"OBJ File Failed to Load";

    while(getline(objectFile,line))                 //Main Loop iterating OBJ File
    {
        if(line.find("mtllib")!=string::npos)       //Calling Function to Parse MTL File and store in map with Key as Group Name
        {
            string file = line.substr(line.find(" ")+1);
			file = "models\\"+file;
            MTLParse(file);
			continue;
        }

		else if(line[0]=='v' && line[1]==' ')                   //Storing in vertices vector
        {
            Vertex v;

            float arr[3];
            ParseFloat(arr,line);

            v.x =  arr[0];
            v.y =  arr[1];
            v.z =  arr[2];

            Vertices.push_back(v);
        }

		else if(line[0]=='v' && line[1]=='n')                   //Storing in verticex Normals
        {
            Vertex v;

            float arr[3];
            ParseFloat(arr,line);

            v.x =  arr[0];
            v.y =  arr[1];
            v.z =  arr[2];

			VertexNormals.push_back(v);
        }

		else if(line[0]=='v' && line[1]=='t')                   //Storing in texture vertices
        {
            Vertex v;

            float arr[3];
            ParseFloat(arr,line);

            v.x =  arr[0];		//u
            v.y =  arr[1];		//v
            v.z =  0.0f;

			TextureVertices.push_back(v);
        }

        else if (line[0] == 'g')                //A new or Old group encountered
        {
            Group g;
            string groupname = line.substr(line.find(" ")+1);	//Extracting Groupname

            if(Groups.find(groupname) != Groups.end())      //Taking out Group Object if Group ALREADY EXISTS, like in cube.obj file
                g = Groups[groupname];

            while(getline(objectFile,line))         
            {
                if(line.empty())				//Loop runs till Group Finishes => Empty string is encountered
                    break;

                if(line[0] == 'f')                  //Storing Faces
                {
					string fTemp[3];
					Face f1;

					line = line.substr(line.find(" ")+1);

					//Parsing The Face string into 3 different parts: 1/1/1/ 2/2/2 3/3/3 => (1/1/1),(2/2/2),(3/3/3)
					fTemp[0] = line.substr(0,line.find(" "));
					line.erase(0,line.find(" ")+1);
					fTemp[1] = line.substr(0,line.find(" "));
					line.erase(0,line.find(" ")+1);
					fTemp[2] = line;

					int tempv[3], tempvt[3], tempvn[3];
					int ctrNormals = 0, ctrTexture = 0;

					for(int i=0; i<3; i++)
					{
						PopulateFaces(fTemp[i],tempv[i],tempvt[i],tempvn[i]);
					}
					//cout<<endl<<tempv[0]<<","<<tempvt[0]<<","<<tempvn[0]<<" "<<tempv[1]<<","<<tempvt[1]<<","<<tempvn[1]<<" "<<tempv[2]<<","<<tempvt[2]<<","<<tempvn[2]<<endl;

					if(tempvt[0] == -1 || tempvt[1] == -1 || tempvt[2] == -1)
						ctrTexture = 0;
					else
						ctrTexture = 1;

					if(tempvn[0] == -1 || tempvn[1] == -1 || tempvn[2] == -1)
						ctrNormals = 0;
					else
						ctrNormals = 1;

					f1.fillVertices(Vertices[tempv[0]-1],Vertices[tempv[1]-1],Vertices[tempv[2]-1]);

					if(ctrNormals)
						f1.fillNormals (VertexNormals[tempvn[0]-1],VertexNormals[tempvn[1]-1],VertexNormals[tempvn[2]-1]);
					
					if(ctrTexture)
						f1.fillTextureVertices(TextureVertices[tempvt[0]-1],TextureVertices[tempvt[1]-1],TextureVertices[tempvt[2]-1]);					

					g.faces.push_back(f1);
                }

                else if (line[0] == 'u')            //Storing MTL file information
                {
                    g.mtlName = line.substr(line.find(" ")+1);
                }
            }
            Groups[groupname] = g;					//Either insert a NEW group or UPDATE the previous one with added information
        }
    }

    objectFile.close();

	//******************POPULATING Triangle Vector*******************
	
	cout<<"\nV : "<<Vertices.size();
	cout<<"\nVN : "<<VertexNormals.size();
	cout<<"\nVT : "<<TextureVertices.size();

	Vertices.clear();
	TextureVertices.clear();
	VertexNormals.clear();

	for(unordered_map <string,Group>::iterator it = Groups.begin(); it!=Groups.end(); it++)
    {
        //cout<<"MTL Name:"<<it->second.mtlName<<endl;
		//cout<<endl<<it->first<<", ";
		//cout<<"Faces: "<<it->second.faces.size();

		//Iterating through Different Faces, Populating Different Buffers
        for(vector <Face>::iterator it2 = it->second.faces.begin(); it2!=it->second.faces.end(); it2++)
        {
			for(int i=0; i<3; i++)
			{
				VertexBuffer.push_back((*it2).triangle[i]);
						
				TextureBuffer.push_back((*it2).textureVertices[i]);
			
				NormalBuffer.push_back((*it2).vertexNormals[i]);

				VectorKa.push_back(MtlArray[it->second.mtlName].ka);

				VectorKd.push_back(MtlArray[it->second.mtlName].kd);

				VectorKs.push_back(MtlArray[it->second.mtlName].ks);

				VectorNs.push_back(MtlArray[it->second.mtlName].ns);

				TextureIndices.push_back(TextureFiles[MtlArray[it->second.mtlName].textureFile]);
			}
        }
    }
	
	//Shifting of Origin and Scaling
	AdjustModel();

    //***********************PARSING ENDS*******************

    //DISPLAYING DETAILS
    
	cout<<"\n\nMTL Size: "<<MtlArray.size()<<endl;
	for(unordered_map <string,Mtlobject>::iterator it = MtlArray.begin(); it!= MtlArray.end(); it++)
    {
        //Mtlobject mtltemp;

        cout<<"Group: "<<it->first<<endl;
		cout<<it->second.ka.r<<" "<<it->second.ka.g<<" "<<it->second.ka.b<<endl;
		cout<<it->second.kd.r<<" "<<it->second.kd.g<<" "<<it->second.kd.b<<endl;
		cout<<it->second.ks.r<<" "<<it->second.ks.g<<" "<<it->second.ks.b<<endl;
        cout<<it->second.ns<<endl;
    }
  //  cout<<"\nMTL File Ends "<<endl<<endl;
	
    cout<<"\n\nGROUP SIZE: "<<Groups.size()<<endl<<endl;

	//cout<<"\n\TRIANGLES SIZE: "<<Triangles.size()<<endl<<endl;

	//De-allocating Memory from "Groups" unordered_map
	Groups.clear();
	MtlArray.clear();
}

int VisitedObjects = 0;

void ParseCubes()
{
	//Common file for ALL cubes
	string cubeFile = dir + "cube.obj";

	for(int i=0; i<7; i++)
	{
		int tempValX = 0, tempValY = i;

		for(int j=0; j<=i; j++)
		{
			CubeObject tempObj;

			ParseFunc(cubeFile);

			//Scaling Cube down
			for(int i=0; i<VertexBuffer.size(); i++)
			{
				VertexBuffer[i].x *= WorldScale;
				VertexBuffer[i].y *= WorldScale;
				VertexBuffer[i].z *= WorldScale;
			}

			//Copying into Respective Object
			tempObj.singleCube.VertexBuffer = VertexBuffer;
			tempObj.singleCube.TextureBuffer = TextureBuffer;
			tempObj.singleCube.NormalBuffer = NormalBuffer;
			tempObj.singleCube.VectorKa = VectorKa;
			tempObj.singleCube.VectorKd = VectorKd;
			tempObj.singleCube.VectorKs = VectorKs;
			tempObj.singleCube.VectorNs = VectorNs;
			tempObj.singleCube.TextureFiles = TextureFiles;
			tempObj.singleCube.TextureIndices = TextureIndices;

			//Clearing Temporary Global Buffers
			VertexBuffer.clear();
			TextureBuffer.clear();
			NormalBuffer.clear();
			VectorKa.clear();
			VectorKd.clear();
			VectorKs.clear();
			VectorNs.clear();
			TextureFiles.clear();
			TextureIndices.clear();

			//Assigning Cube Coords
			tempObj.Xval = tempValX;
			tempObj.Yval = tempValY;

			tempValX++;
			tempValY--;

			//Calculating Cube Translation
			tempObj.singleCube.transMatrix = glm::translate(2*WorldScale*tempObj.Xval,-2*WorldScale*(tempObj.Xval+tempObj.Yval),2*WorldScale*tempObj.Yval);

			//Adding to Cubes Vector
			Cubes.push_back(tempObj);
		}
	}
}

void ParseQbert()
{
	string qbertFile = dir + "cube2.obj";

	ParseFunc(qbertFile);

	//Scaling Qbert down
	for(int i=0; i<VertexBuffer.size(); i++)
	{
		VertexBuffer[i].x *= WorldScale;
		VertexBuffer[i].y *= WorldScale;
		VertexBuffer[i].z *= WorldScale;
	}

	//Copying into Respective Object
	qbert.singleCube.VertexBuffer = VertexBuffer;
	qbert.singleCube.TextureBuffer = TextureBuffer;
	qbert.singleCube.NormalBuffer = NormalBuffer;
	qbert.singleCube.VectorKa = VectorKa;
	qbert.singleCube.VectorKd = VectorKd;
	qbert.singleCube.VectorKs = VectorKs;
	qbert.singleCube.VectorNs = VectorNs;
	qbert.singleCube.TextureFiles = TextureFiles;
	qbert.singleCube.TextureIndices = TextureIndices;

	//Clearing Temporary Global Buffers
	VertexBuffer.clear();
	TextureBuffer.clear();
	NormalBuffer.clear();
	VectorKa.clear();
	VectorKd.clear();
	VectorKs.clear();
	VectorNs.clear();
	TextureFiles.clear();
	TextureIndices.clear();

	//Putting qbert on Top of 1st Cube 
	qbert.singleCube.transMatrix = glm::mat4(1.0f);
}

void ParseEnemy1()
{
	string enemyfile = dir + "cube3.obj";

	ParseFunc(enemyfile);

	//Scaling Qbert down
	for(int i=0; i<VertexBuffer.size(); i++)
	{
		VertexBuffer[i].x *= WorldScale;
		VertexBuffer[i].y *= WorldScale;
		VertexBuffer[i].z *= WorldScale;
	}

	//Copying into Respective Object
	enemy1.singleCube.VertexBuffer = VertexBuffer;
	enemy1.singleCube.TextureBuffer = TextureBuffer;
	enemy1.singleCube.NormalBuffer = NormalBuffer;
	enemy1.singleCube.VectorKa = VectorKa;
	enemy1.singleCube.VectorKd = VectorKd;
	enemy1.singleCube.VectorKs = VectorKs;
	enemy1.singleCube.VectorNs = VectorNs;
	enemy1.singleCube.TextureFiles = TextureFiles;
	enemy1.singleCube.TextureIndices = TextureIndices;

	//Clearing Temporary Global Buffers
	VertexBuffer.clear();
	TextureBuffer.clear();
	NormalBuffer.clear();
	VectorKa.clear();
	VectorKd.clear();
	VectorKs.clear();
	VectorNs.clear();
	TextureFiles.clear();
	TextureIndices.clear();

	//Putting enemy1 on Top of 1st Cube 
	enemy1.singleCube.transMatrix = glm::mat4(1.0f);
}

void ParseEnemy2()
{
	string enemyfile = dir + "cube4.obj";

	ParseFunc(enemyfile);

	//Scaling Qbert down
	for(int i=0; i<VertexBuffer.size(); i++)
	{
		VertexBuffer[i].x *= WorldScale;
		VertexBuffer[i].y *= WorldScale;
		VertexBuffer[i].z *= WorldScale;
	}

	//Copying into Respective Object
	enemy2.singleCube.VertexBuffer = VertexBuffer;
	enemy2.singleCube.TextureBuffer = TextureBuffer;
	enemy2.singleCube.NormalBuffer = NormalBuffer;
	enemy2.singleCube.VectorKa = VectorKa;
	enemy2.singleCube.VectorKd = VectorKd;
	enemy2.singleCube.VectorKs = VectorKs;
	enemy2.singleCube.VectorNs = VectorNs;
	enemy2.singleCube.TextureFiles = TextureFiles;
	enemy2.singleCube.TextureIndices = TextureIndices;

	//Clearing Temporary Global Buffers
	VertexBuffer.clear();
	TextureBuffer.clear();
	NormalBuffer.clear();
	VectorKa.clear();
	VectorKd.clear();
	VectorKs.clear();
	VectorNs.clear();
	TextureFiles.clear();
	TextureIndices.clear();

	//Putting enemy2 on Top of 1st Cube 
	enemy2.singleCube.transMatrix = glm::mat4(1.0f);
}

void ParseEnemy3()
{
	string enemyfile = dir + "cube5.obj";

	ParseFunc(enemyfile);

	//Scaling Qbert down
	for(int i=0; i<VertexBuffer.size(); i++)
	{
		VertexBuffer[i].x *= WorldScale;
		VertexBuffer[i].y *= WorldScale;
		VertexBuffer[i].z *= WorldScale;
	}

	//Copying into Respective Object
	enemy3.singleCube.VertexBuffer = VertexBuffer;
	enemy3.singleCube.TextureBuffer = TextureBuffer;
	enemy3.singleCube.NormalBuffer = NormalBuffer;
	enemy3.singleCube.VectorKa = VectorKa;
	enemy3.singleCube.VectorKd = VectorKd;
	enemy3.singleCube.VectorKs = VectorKs;
	enemy3.singleCube.VectorNs = VectorNs;
	enemy3.singleCube.TextureFiles = TextureFiles;
	enemy3.singleCube.TextureIndices = TextureIndices;

	//Clearing Temporary Global Buffers
	VertexBuffer.clear();
	TextureBuffer.clear();
	NormalBuffer.clear();
	VectorKa.clear();
	VectorKd.clear();
	VectorKs.clear();
	VectorNs.clear();
	TextureFiles.clear();
	TextureIndices.clear();

	//Putting enemy3 on Top of 1st Cube 
	enemy3.singleCube.transMatrix = glm::mat4(1.0f);
}

void ParseDisks()
{
	string diskFile = dir + "cube6.obj";

	ParseFunc(diskFile);

	//Scaling Disks[0] down
	for(int i=0; i<VertexBuffer.size(); i++)
	{
		VertexBuffer[i].x *= WorldScale;
		VertexBuffer[i].y *= WorldScale;
		VertexBuffer[i].z *= WorldScale;
	}

	//Copying into Respective Object
	Disks[0].singleCube.VertexBuffer = VertexBuffer;
	Disks[0].singleCube.TextureBuffer = TextureBuffer;
	Disks[0].singleCube.NormalBuffer = NormalBuffer;
	Disks[0].singleCube.VectorKa = VectorKa;
	Disks[0].singleCube.VectorKd = VectorKd;
	Disks[0].singleCube.VectorKs = VectorKs;
	Disks[0].singleCube.VectorNs = VectorNs;
	Disks[0].singleCube.TextureFiles = TextureFiles;
	Disks[0].singleCube.TextureIndices = TextureIndices;

	//Putting Disks[0] on Top of 1st Cube 
	Disks[0].singleCube.transMatrix = glm::translate(2*WorldScale*-1,-2*WorldScale*3,2*WorldScale*4);

	//Copying into Respective Object
	Disks[1].singleCube.VertexBuffer = VertexBuffer;
	Disks[1].singleCube.TextureBuffer = TextureBuffer;
	Disks[1].singleCube.NormalBuffer = NormalBuffer;
	Disks[1].singleCube.VectorKa = VectorKa;
	Disks[1].singleCube.VectorKd = VectorKd;
	Disks[1].singleCube.VectorKs = VectorKs;
	Disks[1].singleCube.VectorNs = VectorNs;
	Disks[1].singleCube.TextureFiles = TextureFiles;
	Disks[1].singleCube.TextureIndices = TextureIndices;

	Disks[1].singleCube.transMatrix = glm::translate(2*WorldScale*4,-2*WorldScale*3,2*WorldScale*-1);

	//Clearing Temporary Global Buffers
	VertexBuffer.clear();
	TextureBuffer.clear();
	NormalBuffer.clear();
	VectorKa.clear();
	VectorKd.clear();
	VectorKs.clear();
	VectorNs.clear();
	TextureFiles.clear();
	TextureIndices.clear();
}
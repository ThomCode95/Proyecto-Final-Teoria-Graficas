/*
Semestre 2020-1
Proyecto Final : Casa navideña
Alumno: Arias Pelayo Thomas

*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#pragma comment(lib, "irrKlang.lib")

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//para el sonido  
#include <irrKlang.h>

#include "Window.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"




const float toRadians = 3.14159265f / 180.0f;


// variables para Key Frame
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

//void my_input(GLFWwindow *window);
void inputAnimateAngel(bool* keys);
//void my_input(GLFWwindow *window);
void inputAnimateTrineo1(bool* keys);
//void my_input(GLFWwindow *window);
void inputAnimateTrineo2(bool* keys);
//void my_input(GLFWwindow *window);
void inputAnimateGato(bool* keys);

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

//Variables para la camaras
Camera camera1;//Camara libre
Camera camera2;//Camara que realiza el recorrido
Camera camera3;// Camara estatica sobre el nacimiento
Camera camera4;
Camera camera5;
Camera camera6;

//variables para las texturas
Texture plainTexture;

//Variable para el sonido
using namespace irrklang;

// variables para los materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//Modelos implementados en navidad 
Model ArbolAfuera;
Model ArbolNavidad;
Model Casa;
Model Regalo;
Model CoronaAdviento;
Model Ornamento1;
Model MuñecoNavideño;
Model Campana;
Model Nacimiento;
Model Estrella;
Model CajaAdornos;
Model Pinata;
Model Angel;
Model Nochebuena;
Model Trineo1;
Model Trineo2;
Model Gato;



//Prototipos de las funciones 
void selectCamera();
void skyBoxCamera(glm::mat4 projection, GLuint time = 0);

//Variables para los skybox 
Skybox skybox1;
Skybox skybox2;
Skybox skybox3;

//Variables para el tiempo 
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformView2 = 0, uniformEyePosition = 0, uniformEyePosition2 = 0,
uniformSpecularIntensity = 0, uniformShininess = 0;

//Recorrido de la camara
float recorridoEnX = 0.0f;
float recorridoEnZ = 0.0f;
//float  recorridoEnY = 0.33f;
//float  recorridoEnZ = 0.66f;


//Variables de laAnimacion del ángel
bool animacionAngel = false;
float movKit_x_Angel = 10.0f;
float movKit_z_Angel = 270.0f;
int estadoAngel = 0;//estado del ángel

//Variables de laAnimacion del Trineo 1
bool animacionTrineo1 = false;
float movKit_x_Trineo1 = 270.0f;
float movKit_z_Trineo1 = 280.0f;
int estadoTrineo1 = 0;//estado del trineo1

//Variables de laAnimacion del Trineo2
bool animacionTrineo2 = false;
float movKit_x_Trineo2 = 300.0f;
float movKit_z_Trineo2 = 180.0f;
int estadoTrineo2 = 0;//estado del trineo1

//Variables de laAnimacion del gato
bool animacionGato = false;
float movKit_x_Gato = 50.0f;
float movKit_y_Gato = 110.0f;
float movKit_z_Gato = 380.0f;
int estadoGato = 0;//estado del gato

//Variables de laAnimacion del muñeco navideño
bool animacionMuñeco = false;
float movKit_x_Muñeco = 270.0f;
float movKit_z_Muñeco = 280.0f;
int estadoMuñeco = 0;//estado del múñeco navideño

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() 
{
	unsigned int indices[] = {		
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//	x      y      z			u	  v			nx	  ny    nz
		-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = 
	{
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};
	//Ejercicio 1: reemplazar con sus dados de 6 caras texturizados, agregar normales
// average normals
	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};
	
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

//NEW// Keyframes
float posXavion = 11.0, posYavion = 0.0, posZavion = 255;
float	movAvion_x = 11.0f, movAvion_y = 0.0f;
float giroAvion = 0;

#define MAX_FRAMES 9
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float giroAvion;
	float giroAvionInc;
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 9;			//introducir maximo número de avion
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);


	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].giroAvion;

	FrameIndex++;
}

void resetElements(void)
{

	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	giroAvion = KeyFrame[0].giroAvion;
}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].giroAvionInc = (KeyFrame[playIndex + 1].giroAvion - KeyFrame[playIndex].giroAvion) / i_max_steps;

}
//Animación por key frames

void animate(void)
{
	//Movimiento del objeto
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de la anamicación entre frames
		{
			playIndex++;
			//printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//fin de la animación??
			{
				printf("Frame index= %d\n", FrameIndex);
				printf("termina anim\n");
				playIndex = 0;//se resetea el índice
				play = false;//la animación esta en pausa de nuevo
			}
			else //Si no ha finalizado la animación hace la siguiente interpolación
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Resetea counter
				//Llama a la función de interpolación
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			giroAvion += KeyFrame[playIndex].giroAvionInc;
			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if (reproduciranimacion < 1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
				printf("\nPresiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}

	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}

	if (keys[GLFW_KEY_L])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("\nmovAvion_x es: %f\n", movAvion_x);
			//printf("movAvion_y es: %f\n", movAvion_y);
			printf("\npresiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
		}
	}


	if (keys[GLFW_KEY_1])
	{
		if (ciclo < 1)
		{
			//printf("movAvion_x es: %f\n", movAvion_x);
			movAvion_x += 1.0f;
			printf("\nmovAvion_x es: %f\n", movAvion_x);
			ciclo++;
			ciclo2 = 0;
			printf("\nreinicia con 2\n");
		}

	}
	if (keys[GLFW_KEY_2])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
		}
	}

}



//Animacion del ángel en el cuarto arriba a la derecha. El ángel se mueve en la trayectoria de un cuadrado
void animateAngel(void)
{
	if (animacionAngel)//Si se presiona espacio entra a la animación
	{
		switch (estadoAngel) {
		case 0://En este estado el angel avanza en línea recta
			movKit_z_Angel += 0.2f;
			if (movKit_z_Angel >= 330)//Cuando ha recorrido 60 unidades en z cambia de estado
				estadoAngel = 1;
			break;
		case 1://En este estado el angel se mueve en x
			movKit_x_Angel += 0.2f;
			if (movKit_x_Angel >= 55)//Cuando se ha movido 45 unidades en x cambia de estado
				estadoAngel = 2;
			break;
		case 2://En este estado el angel avanza en línea recta en direccion opuesta al estado 0
			movKit_z_Angel -= 0.2f;
			if (movKit_z_Angel <= 270)//Cuando ha recorrido 60 unidades en z cambia de estado
				estadoAngel = 3;
			break;
		case 3:// En este estado el angel vuelve a su posicion inicial
			movKit_x_Angel -= 0.2f;
			if (movKit_x_Angel <= 10.0)//Cuando llega a 0.0 pasa al siguiente estado
			{
				animacionAngel = false;
				movKit_x_Angel = 10.0f;
				movKit_z_Angel = 270.0f;
				estadoAngel = 0;
			}

			break;
		default:
			break;
		}
	}
}


//Animacion del muñec en el ático, parteizquierda de la casa
void animateMuñeco(void)
{
	if (animacionMuñeco)//Si se presiona espacio entra a la animación
	{
		switch (estadoMuñeco) {
		case 0://En este estado el avanza en línea recta
			movKit_z_Muñeco += 0.2f;
			if (movKit_z_Muñeco >= 330)//Cuando ha recorrido 60 unidades en z cambia de estado
				estadoMuñeco = 1;
			break;
		case 1://En este estado el  se mueve en x
			movKit_x_Muñeco += 0.2f;
			if (movKit_x_Muñeco >= 300)//Cuando se ha movido 45 unidades en x cambia de estado
				estadoMuñeco = 2;
			break;
		case 2://En este estado el avanza en línea recta en direccion opuesta al estado 0
			movKit_z_Muñeco -= 0.2f;
			if (movKit_z_Muñeco <= 270)//Cuando ha recorrido 60 unidades en z cambia de estado
				estadoMuñeco = 3;
			break;
		case 3:// En este estado el vuelve a su posicion inicial
			movKit_x_Muñeco -= 0.2f;
			if (movKit_x_Muñeco <= 240.0)//Cuando llega a 0.0 pasa al siguiente estado
			{
				animacionMuñeco = false;
				movKit_x_Muñeco = 270.0f;
				movKit_z_Muñeco = 280.0f;
				estadoMuñeco = 0;
			}

			break;
		default:
			break;
		}
	}
}

void animateTrineo1(void)
{
	if (animacionTrineo1)//Si se presiona espacio entra a la animación
	{
		switch (estadoTrineo1) {
		case 0://En este estado el avanza en línea recta
			movKit_z_Trineo1 -= 0.5f;
			if (movKit_z_Trineo1 <= -100)//Cuando ha recorrido 60 unidades en z cambia de estado
			{
				animacionTrineo1 = false;
				movKit_x_Trineo1 = 270.0f;
				movKit_z_Trineo1 = 180.0f;
				estadoMuñeco = 0;
			}
			break;
		default:
			break;
		}
	}
}


void animateTrineo2(void)
{
	if (animacionTrineo2)//Si se presiona espacio entra a la animación
	{
		switch (estadoTrineo2) {
		case 0://En este estado el avanza en línea recta
			movKit_z_Trineo2 -= 0.3f;
			if (movKit_z_Trineo2 <= 30)//Cuando ha recorrido 60 unidades en z cambia de estado
				estadoTrineo2 = 1;
			break;
		case 1:
			movKit_x_Trineo2 -= 0.3f;
			if (movKit_x_Trineo2 <= 270)//Cuando se ha movido 45 unidades en x cambia de estado
				estadoMuñeco = 2;
			break;
		case 2:
			movKit_z_Trineo2 += 0.2f;
			if (movKit_z_Trineo2 >= 180)//Cuando ha recorrido 60 unidades en z cambia de estado
			{
				animacionTrineo2 = false;
			//	movKit_x_Trineo2 = 270.0f;
				//movKit_z_Trineo2 = 180.0f;
				//estadoMuñeco = 0;
			}
			break;
		default:
			break;
		}
	}
}

void animateGato(void)
{
	if (animacionGato)//Si se presiona espacio entra a la animación
	{
		switch (estadoGato) {
		case 0://En este estado el avanza en línea recta
			movKit_y_Gato += 1.5f;
			if (movKit_y_Gato >= 900)//Cuando ha recorrido 60 unidades en z cambia de estado
			{
				animacionGato = false;
				movKit_x_Gato = 0.0f;
				movKit_z_Gato = 450.0f;
				estadoGato = 0;
			}
			break;
		default:
			break;
		}
	}
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void inputAnimateAngel(bool* keys)
{
	if (keys[GLFW_KEY_M])
	{
		animacionAngel = true;
	}
}

void inputAnimateMuñeco(bool* keys)
{
	if (keys[GLFW_KEY_N])
	{
		animacionMuñeco = true;
	}
}

void inputAnimateTrineo1(bool* keys)
{
	if (keys[GLFW_KEY_B])
	{
		animacionTrineo1 = true;
	}
}

void inputAnimateTrineo2(bool* keys)
{
	if (keys[GLFW_KEY_V])
	{
		animacionTrineo2 = true;
	}
}

void inputAnimateGato(bool* keys)
{
	if (keys[GLFW_KEY_T])
	{
		animacionGato = true;
	}
}


int main() 
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();
	
	camera1 = Camera(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);// Camara I: Cámara movil
	camera2 = Camera(glm::vec3(15.0f, 0.0f, 325.0f), glm::vec3(0.0f, 1.0f, 0.0f), -105.0f, 10.0f, 5.0f, 0.5f); // Camara O : Camára recorrido
	//camera2 = Camera(glm::vec3(45.0f, 29.50f, 325.0f), glm::vec3(recorridoEnX, recorridoEnY, recorridoEnZ), 45.0f, -90.0f, 5.0f,0.5f); // Camara O : Camára recorrido
	camera3 = Camera(glm::vec3(55.0f, -25.50f, 200.0f), glm::vec3(0.0f, 1.0f, 0.0f), 105.0f, 10.0f, 5.0f, 0.5f); // Camara P: Camara nacimiento
	camera4 = Camera(glm::vec3(300.0f, 15.50f, 200.0f), glm::vec3(0.0f, 1.0f, 0.0f), -105.0f, 10.0f, 5.0f, 0.5f);
	camera5 = Camera(glm::vec3(0.0f, 50.50f, 400.0f), glm::vec3(0.0f, 1.0f, 0.0f), 100.0f, 10.0f, 5.0f, 0.5f);
	//camera6 = Camera(glm::vec3(270.0f, 40.50f, 280.0f), glm::vec3(0.0f, 1.0f, 0.0f), -105.0f, 10.0f, 5.0f, 0.5f);
	camera6 = Camera(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, 10.0f, 5.0f, 0.5f); // Camara P: Camara nacimiento
	plainTexture = Texture("Textures/Piso/Pasto.png");
	plainTexture.LoadTextureA();

	//Proyecto 
	ArbolAfuera = Model();
	ArbolAfuera.LoadModel("Models/Arbol/Tree.obj");

	ArbolNavidad = Model();
	ArbolNavidad.LoadModel("Models/ArbolNavidad/ArbolNavidad.obj");


	Casa = Model();
	Casa.LoadModel("Models/Casa/Casa.obj");

	Regalo = Model();
	Regalo.LoadModel("Models/Regalo/11563_gift_box_V3.obj");

	CoronaAdviento = Model();
	CoronaAdviento.LoadModel("Models/CoronaAdviento/CoronaAdviento.obj");

	Ornamento1 = Model();
	Ornamento1.LoadModel("Models/Ornamento1/Ornamento1.obj");

	MuñecoNavideño = Model();
	MuñecoNavideño.LoadModel("Models/Ornamento2/Ornamento2.obj");

	Campana = Model();
	Campana.LoadModel("Models/Campana/Campana.obj");

	Nacimiento = Model();
	Nacimiento.LoadModel("Models/Nacimiento/Nacimiento.obj");


	Estrella = Model();
	Estrella.LoadModel("Models/Estrella/Estrella.obj");

	
	CajaAdornos = Model();
	CajaAdornos.LoadModel("Models/CajaAdornos/CajaAdornos.obj");

	Pinata = Model();
	Pinata.LoadModel("Models/Pinata/pinata.obj");

	Angel = Model();
	Angel.LoadModel("Models/Angel/Angel.obj");

	Nochebuena = Model();
	Nochebuena.LoadModel("Models/Nochebuena/flor.obj");

	Trineo1 = Model();
	Trineo1.LoadModel("Models/Trineo/SLITTA BABBO.obj");

	Trineo2 = Model();
	Trineo2.LoadModel("Models/Trineo2/santasledge_low.fbx");

	Gato = Model();
	Gato.LoadModel("Models/Cat/12221_Cat_v1_l3.obj");


	///////////////////////////////////////////////////////////////////////////////
	
	//Configuracion para el sonido 
	//Arranca el motor de sonido con los parámetros por defecto para 2D

	//Audio 3d
	// start the sound engine with default parameters
	ISoundEngine* engine1 = createIrrKlangDevice();
	ISoundEngine* engine2 = createIrrKlangDevice();
	ISoundEngine* engine3 = createIrrKlangDevice();

	//Para comprobar que el audio se pudo abrir 
	if (!engine1)
	{
		printf("Falló inicializar irrKlang.");
		engine1->drop();

	}

	if (!engine2)
	{
		printf("Falló inicializar irrKlang.");
		engine2->drop();

	}
	if (!engine3)
	{
		printf("Falló inicializar irrKlang.");
		engine3->drop();

	}

	irrklang::vec3df position(0, 0, 0);

	//Sonido ambiental de feria 2D
	engine1->play3D("Audio/la-brisa_1.mp3", position, true);
	engine2->play3D("Audio/000758970_prev.mp3", position, true);
	engine3->play3D("Audio/Campanas-de-trineo.mp3", position, true);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.3f, 0.3f,
								0.0f, 0.0f, -1.0f);

	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,//Colores R,G,B
		0.0f, 1.0f,//INTENSIDAD,DENSIDAD
		8.2f, -4.50f, 140.0f,//Posicion X,Y,Z
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	pointLights[1] = PointLight(1.0f, 0.8f, 0.0f,
		1.0f, 0.0f,
		8.2f, -10.50f, 142.0f,
		0.5f, 0.5f, 0.5f);
	pointLightCount++;

	pointLights[2] = PointLight(0.0f, 0.3f, 1.0f,
		1.0f, 0.0f,
		9.3f, -20.0f, 148.0f,
		0.5f, 0.5f, 0.5f);
	pointLightCount++;

	//pointLights[3] = PointLight(1.0f, 1.0f, 1.0f,//Colores R,G,B
		//1.0f, 0.0f,//INTENSIDAD,DENSIDAD
		//0.0f, 10.0f, 25.0f,//Posicion X,Y,Z
		//0.3f, 0.2f, 0.1f);
	//pointLightCount++;
	
	unsigned int spotLightCount = 0;

	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	////Luz interior 1
	spotLights[1] = SpotLight(0.0f, 1.0f, 1.0f,
		1.0f, 3.0f,
		35.0f, 29.50f, 200.0f, // Pos
		0.0f, -1.0f, 0.0f,
		0.1f, 0.1f, 0.0f,
		80.0f);
	spotLightCount++;

	std::vector<std::string> skybox1Faces;
	std::vector<std::string> skybox2Faces;
	std::vector<std::string> skybox3Faces;

	/*Día*/
	skybox1Faces.push_back("Textures/Skybox/desertsky_rt.tga");
	skybox1Faces.push_back("Textures/Skybox/desertsky_lf.tga");
	skybox1Faces.push_back("Textures/Skybox/desertsky_dn.tga");
	skybox1Faces.push_back("Textures/Skybox/desertsky_up.tga");
	skybox1Faces.push_back("Textures/Skybox/desertsky_bk.tga");
	skybox1Faces.push_back("Textures/Skybox/desertsky_ft.tga");

	/*Noche*/
	skybox2Faces.push_back("Textures/Skybox/desertsky_rt_night.tga");
	skybox2Faces.push_back("Textures/Skybox/desertsky_lf_night.tga");
	skybox2Faces.push_back("Textures/Skybox/desertsky_dn_night.tga");
	skybox2Faces.push_back("Textures/Skybox/desertsky_up_night.tga");
	skybox2Faces.push_back("Textures/Skybox/desertsky_bk_night.tga");
	skybox2Faces.push_back("Textures/Skybox/desertsky_ft_night.tga");

	/*Medio*/
	skybox3Faces.push_back("Textures/Skybox/desertsky_rt_mid.tga");
	skybox3Faces.push_back("Textures/Skybox/desertsky_lf_mid.tga");
	skybox3Faces.push_back("Textures/Skybox/desertsky_dn_mid.tga");
	skybox3Faces.push_back("Textures/Skybox/desertsky_up_mid.tga");
	skybox3Faces.push_back("Textures/Skybox/desertsky_bk_mid.tga");
	skybox3Faces.push_back("Textures/Skybox/desertsky_ft_mid.tga");


	skybox1 = Skybox(skybox1Faces);
	skybox2 = Skybox(skybox2Faces);
	skybox3 = Skybox(skybox3Faces);


	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 600.0f);


	float time = 0.0;


	irrklang::vec3df lookDirection(10, 0, 10); // the direction the listener looks into
	glm::vec3 posblackhawk = glm::vec3(11.0f, 0.0f, 255.0f); // vector de posicion

// aFUERA del while
	//KeyFrames iniciales
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].giroAvion = 0;


	KeyFrame[1].movAvion_x = 1.0f;
	KeyFrame[1].movAvion_y = 0.0f;
	KeyFrame[1].giroAvion = 0;


	KeyFrame[2].movAvion_x = 3.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].giroAvion = 0;


	KeyFrame[3].movAvion_x = 5.0f;
	KeyFrame[3].movAvion_y = 0.0f;
	KeyFrame[3].giroAvion = 0;

	KeyFrame[4].movAvion_x = 7.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].giroAvion = 180.0f;

	KeyFrame[5].movAvion_x = 5.0f;
	KeyFrame[5].movAvion_y = 0.0f;
	KeyFrame[5].giroAvion = 180.0f;

	KeyFrame[6].movAvion_x = 3.0f;
	KeyFrame[6].movAvion_y = 0.0f;
	KeyFrame[6].giroAvion = 180.0f;

	KeyFrame[7].movAvion_x = 1.0f;
	KeyFrame[7].movAvion_y = 0.0f;
	KeyFrame[7].giroAvion = 180.0f;

	KeyFrame[8].movAvion_x = 0.0f;
	KeyFrame[8].movAvion_y = 0.0f;
	KeyFrame[8].giroAvion = 180.0f;

	KeyFrame[9].movAvion_x = 0.0f;
	KeyFrame[9].movAvion_y = 0.0f;
	KeyFrame[9].giroAvion = 180.0f;

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())

	{

		/************************************Audio3D************************************/
		engine2->setListenerPosition(vec3df(0 - camera1.getCameraPosition().x, 0 - camera1.getCameraPosition().y, 5 - camera1.getCameraPosition().z), lookDirection );

		engine3->setListenerPosition(vec3df(30 - camera1.getCameraPosition().x, 0 - camera1.getCameraPosition().y, -5 - camera1.getCameraPosition().z), lookDirection);


		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();

		//Para la animacion del ángel
		inputAnimateAngel(mainWindow.getsKeys());
		animateAngel();
		//Para la animación del muñeco navieño
		inputAnimateMuñeco(mainWindow.getsKeys());
		animateMuñeco();

		inputAnimateTrineo1(mainWindow.getsKeys());
		animateTrineo1();

		inputAnimateTrineo2(mainWindow.getsKeys());
		animateTrineo2();

		inputAnimateGato(mainWindow.getsKeys());
		animateGato();
		
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//asignarle una camara al skyBox
		time += 1 * deltaTime;


		if (time <= 10.0) 
		{
			skyBoxCamera(projection);

		}
		else if (time >= 10.0 && time <= 20.0) 
		{
			skyBoxCamera(projection, 1);

		}
		else if (time >= 20.0) 
		{
			skyBoxCamera(projection, 2);

		}
		if (time >= 30.0) 
		{
			time = 0.0;
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

	
		glm::vec3 lowerLight = camera1.getCameraPosition(); // se le asina la posición a la spot light que s etiene enfrente de la pantalla
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera1.getCameraDirection());
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		/************************************Luces Spotlight************************************/
			//////////////////////////////////////// Prender y apagar las luces /////////////////////////////////
		if (mainWindow.getLuces() == 0)
		{
			shaderList[0].SetPointLights(pointLights, 0);
			shaderList[0].SetSpotLights(spotLights, 0);
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		if (mainWindow.getLuces() == 1)
		{
			shaderList[0].SetSpotLights(spotLights, 1);
		}
		if (mainWindow.getLuces() == 2)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}




		//llamada a la funcion de seleccion de camaras
		selectCamera();

		glm::mat4 model(1.0);

		/***********************************************Recorrido de la camara *******************************************/

		//recorridoEnX += 0.1f;
		//recorridoEnY += 0.1f;
		//recorridoEnZ += 0.1f;


		//Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -50.0f, -5.0f));
		model = glm::scale(model, glm::vec3(700.0f, 1.0f, 700.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		
		/************************************Zona arboles************************************/
	
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(95.0f, -49.0f, 35.0f));
		model = glm::scale(model, glm::vec3(27.0f, 27.0f, 27.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArbolAfuera.RenderModel();

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(65.0f, -49.0f, -42.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArbolAfuera.RenderModel();
		

		//Casa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -33.50f, 650.0f));
		model = glm::rotate(model, (-180 * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(25.0f, 25.0f, 25.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Casa.RenderModel();

		//Arbol Navidad
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, -38.50f, 135.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArbolNavidad.RenderModel();
		
		/************************************Regalos Navidad************************************/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(30.0f, -38.50f, 130.0f));
		model = glm::rotate(model,(-90 * toRadians), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Regalo.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(17.0f, -38.50f, 137.0f));
		model = glm::rotate(model, (-45 * toRadians), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Regalo.RenderModel();
	   

		//Caja de adornos 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(51.0f, -38.50f, 130.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CajaAdornos.RenderModel();
		

		//Corona de adviento 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f, 2.50f, 120.0f));
		model = glm::rotate(model, (90 * toRadians), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, (180 * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CoronaAdviento.RenderModel();

		//Ornamiento Uno 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(85.0f, -12.50f, 117.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Ornamento1.RenderModel();

		//Muñeco Navideño 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movKit_x_Muñeco, 40.50f, movKit_z_Muñeco));
		model = glm::rotate(model, (180 * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MuñecoNavideño.RenderModel();
	    
		//Campana
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(70.0f, 5.50f, 200.0f));
		model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Campana.RenderModel();

		//Nacimiento
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, -25.50f, 240.0f));
		model = glm::scale(model, glm::vec3(6.0f, 6.0f, 6.0f));
		model = glm::rotate(model, (180 * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Nacimiento.RenderModel();
		

		//Angel
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movKit_x_Angel, 50.50f, movKit_z_Angel));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Angel.RenderModel();
		
		//Piñata
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 15.50f, 185.0f));
		model = glm::scale(model, glm::vec3(2.75f, 2.75f, 2.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pinata.RenderModel();

		//Estrella nacimiento
		model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXavion + movAvion_x, posYavion + movAvion_y, posZavion);
		model = glm::translate(model, posblackhawk);
		model = glm::rotate(model, (-45* toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Estrella.RenderModel();


		//El trineo 1 esta En la cochera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movKit_x_Trineo1, -30.0f, movKit_z_Trineo1));
		model = glm::rotate(model, (90 * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Trineo1.RenderModel();

		//Trineo 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movKit_x_Trineo2, -30.0f, movKit_z_Trineo2));
		model = glm::rotate(model, (90 * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Trineo2.RenderModel();

		//Gato
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movKit_x_Gato,15.0f +  movKit_y_Gato, movKit_z_Gato));
		model = glm::rotate(model, (90 * toRadians), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gato.RenderModel();



		//////////////////////////////////////////////////////////////////////////////////////

		glDisable(GL_BLEND);
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	engine1->drop(); // delete engine
	// don't forget to release the resources as explained above.
	engine2->drop(); // delete engine
	// don't forget to release the resources as explained above.
	engine3->drop(); // delete engine
	// don't forget to release the resources as explained above.

	return 0;
}


void skyBoxCamera(glm::mat4 projection, GLuint time) 
{
	if (time == 0) 
	{
		if (mainWindow.GetSelectCamara() == 1)
		{
			camera1.keyControl(mainWindow.getsKeys(), deltaTime);
			camera1.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			skybox1.DrawSkybox(camera1.calculateViewMatrix(), projection);
		}
		else if (mainWindow.GetSelectCamara() == 2) {
			skybox1.DrawSkybox(camera2.calculateViewMatrix(), projection);
			camera2.recorridoControl(recorridoEnX);
		}

		else if (mainWindow.GetSelectCamara() == 3)
			skybox1.DrawSkybox(camera3.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 4)
			skybox1.DrawSkybox(camera4.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 5)
			skybox1.DrawSkybox(camera5.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 6)
			skybox1.DrawSkybox(camera6.calculateViewMatrix(), projection);
	
	}
	else if (time == 2) 
	{
		if (mainWindow.GetSelectCamara() == 1)
		{
			camera1.keyControl(mainWindow.getsKeys(), deltaTime);
			camera1.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			skybox2.DrawSkybox(camera1.calculateViewMatrix(), projection);
		}
		else if (mainWindow.GetSelectCamara() == 2)
			skybox2.DrawSkybox(camera2.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 3)
			skybox2.DrawSkybox(camera3.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 4)
			skybox2.DrawSkybox(camera4.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 5)
			skybox2.DrawSkybox(camera5.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 6)
			skybox2.DrawSkybox(camera6.calculateViewMatrix(), projection);
		
	}
	else if (time == 1) 
	{
		if (mainWindow.GetSelectCamara() == 1)
		{
			camera1.keyControl(mainWindow.getsKeys(), deltaTime);
			camera1.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
			skybox3.DrawSkybox(camera1.calculateViewMatrix(), projection);
		}
		else if (mainWindow.GetSelectCamara() == 2)
			skybox3.DrawSkybox(camera2.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 3)
			skybox3.DrawSkybox(camera3.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 4)
			skybox3.DrawSkybox(camera4.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 5)
			skybox3.DrawSkybox(camera5.calculateViewMatrix(), projection);
		else if (mainWindow.GetSelectCamara() == 6)
			skybox3.DrawSkybox(camera6.calculateViewMatrix(), projection);

	}


}

void selectCamera()
{

	if (mainWindow.GetSelectCamara() == 1)
	{
	   
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera1.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera1.getCameraPosition().x, camera1.getCameraPosition().y, camera1.getCameraPosition().z);
	}
	else if (mainWindow.GetSelectCamara() == 2)
	{
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera2.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera2.getCameraPosition().x, camera2.getCameraPosition().y, camera2.getCameraPosition().z);
	}
	else if (mainWindow.GetSelectCamara() == 3)
	{
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera3.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera3.getCameraPosition().x, camera3.getCameraPosition().y, camera3.getCameraPosition().z);
	}
	else if (mainWindow.GetSelectCamara() == 4)
	{
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera4.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera4.getCameraPosition().x, camera4.getCameraPosition().y, camera4.getCameraPosition().z);
	}
	else if (mainWindow.GetSelectCamara() == 5)
	{
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera5.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera5.getCameraPosition().x, camera5.getCameraPosition().y, camera5.getCameraPosition().z);
	}
	else if (mainWindow.GetSelectCamara() == 6)
	{
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera6.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera6.getCameraPosition().x, camera6.getCameraPosition().y, camera6.getCameraPosition().z);
	}
}


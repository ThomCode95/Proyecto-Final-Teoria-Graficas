#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		printf("se presiono la tecla: %s\n",key_name);
	}
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_A, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_W, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_S, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}
	//Camara numero uno
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		theWindow->SetSelectCamara(1);
		const char* key_name = glfwGetKeyName(GLFW_KEY_I, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}
	//Camara numero dos
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		theWindow->SetSelectCamara(2);
		const char* key_name = glfwGetKeyName(GLFW_KEY_O, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}
	//Camara numero tres
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		theWindow->SetSelectCamara(3);
		const char* key_name = glfwGetKeyName(GLFW_KEY_P, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}
	//Camara numero cuatro
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		theWindow->SetSelectCamara(4);
		const char* key_name = glfwGetKeyName(GLFW_KEY_U, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		theWindow->SetSelectCamara(5);
		const char* key_name = glfwGetKeyName(GLFW_KEY_J, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		theWindow->SetSelectCamara(6);
		const char* key_name = glfwGetKeyName(GLFW_KEY_K, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		theWindow->Luces = 0;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		theWindow->Luces = 1;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		theWindow->Luces = 2;
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}

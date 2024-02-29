#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

#include "Headers/AnimationUtils.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;

std::shared_ptr<FirstPersonCamera> camera(new FirstPersonCamera());

Sphere skyboxSphere(20, 20);
Box boxCesped;
Box boxWalls;
Box boxHighway;
Box boxLandingPad;
Sphere esfera1(10, 10);
// Models complex instances
Model modelRock;
Model modelAircraft;
Model modelEclipseChasis;
Model modelEclipseRearWheels;
Model modelEclipseFrontalWheels;
Model modelHeliChasis;
Model modelHeliHeli;
Model modelHeliRear;
Model modelLambo;
Model modelLamboLeftDor;
Model modelLamboRightDor;
Model modelLamboFrontLeftWheel;
Model modelLamboFrontRightWheel;
Model modelLamboRearLeftWheel;
Model modelLamboRearRightWheel;
//Dos objetos propios
Model modelPokeball;
Model modelHealingMachine;

// Dart lego
Model modelDartLegoBody;
Model modelDartLegoHead;
Model modelDartLegoMask;
Model modelDartLegoLeftArm;
Model modelDartLegoRightArm;
Model modelDartLegoLeftHand;
Model modelDartLegoRightHand;
Model modelDartLegoLeftLeg;
Model modelDartLegoRightLeg;

// Buzz lightyear
Model modelBuzzBody;
Model modelBuzzHead;
Model modelBuzzHip;
Model modelBuzzLeftArm;
Model modelBuzzRightArm;
Model modelBuzzLeftForearm;
Model modelBuzzRightForearm;
Model modelBuzzLeftHand;
Model modelBuzzRightHand;
Model modelBuzzLeftLeg;
Model modelBuzzRightLeg;
Model modelBuzzLeftCalf;
Model modelBuzzRightCalf;
Model modelBuzzLeftFoot;
Model modelBuzzRightFoot;
Model modelBuzzLeftWing1;
Model modelBuzzRightWing1;
Model modelBuzzLeftWing2;
Model modelBuzzRightWing2;

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID, texturePokeballID, textureHealingMachineBodyID;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
		"../Textures/mp_bloodvalley/blood-valley_up.tga",
		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 modelMatrixEclipse = glm::mat4(1.0f);
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 modelMatrixHeli = glm::mat4(1.0f);
glm::mat4 modelMatrixLambo = glm::mat4(1.0);
glm::mat4 modelMatrixAircraft = glm::mat4(1.0);
glm::mat4 modelMatrixDart = glm::mat4(1.0f);
glm::mat4 modelMatrixPokeball = glm::mat4(1.0f);
glm::mat4 modelMatrixHealingMachine = glm::mat4(1.0f);
glm::mat4 modelMatrixBuzz = glm::mat4(1.0f);

float rotDartHead = 0.0, rotDartLeftArm = 0.0, rotDartLeftHand = 0.0, rotDartRightArm = 0.0, rotDartRightHand = 0.0, rotDartLeftLeg = 0.0, rotDartRightLeg = 0.0;
float rotBuzzHip = 0.0, rotBuzzBody = 0.0, rotBuzzHead = 0.0, rotBuzzLeftArm = 0.0, rotBuzzLeftForearm = 0.0, rotBuzzLeftHand = 0.0, rotBuzzRightArm = 0.0,
 rotBuzzRightForearm = 0.0, rotBuzzRightHand = 0.0, rotBuzzLeftLeg = 0.0, rotBuzzRightLeg = 0.0, rotBuzzLeftCalf = 0.0, rotBuzzRightCalf = 0.0,
 rotBuzzLeftFoot = 0.0, rotBuzzRightFoot = 0.0, rotBuzzLeftWing1= 0.0, rotBuzzRightWing1= 0.0, rotBuzzLeftWing2= 0.0, rotBuzzRightWing2= 0.0;
int modelSelected = 0;
bool enableCountSelected = true;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
std::vector<std::vector<float>> keyFramesDartJoints;
std::vector<std::vector<glm::mat4>> keyFramesDart;
int indexFrameDartJoints = 0;
int indexFrameDartJointsNext = 1;
float interpolationDartJoints = 0.0;
int maxNumPasosDartJoints = 20;
int numPasosDartJoints = 0;
int indexFrameDart = 0;
int indexFrameDartNext = 1;
float interpolationDart = 0.0;
int maxNumPasosDart = 200;
int numPasosDart = 0;

// Joints interpolations Buzz
std::vector<std::vector<float>> keyFramesBuzzJoints;
std::vector<std::vector<glm::mat4>> keyFramesBuzz;
int indexFrameBuzzJoints = 0;
int indexFrameBuzzJointsNext = 1;
float interpolationBuzzJoints = 0.0;
int maxNumPasosBuzzJoints = 20;
int numPasosBuzzJoints = 0;
int indexFrameBuzz = 0;
int indexFrameBuzzNext = 1;
float interpolationBuzz = 0.0;
int maxNumPasosBuzz = 200;
int numPasosBuzz = 0;

// Var animate helicopter
float rotHelHelY = 0.0;
float rotHelHelX = 0.0;

// Var animate lambo dor
int stateDoor = 2;
float dorRotCount = 0.0;

float avance = 0.1f;
float giroEclipse = 0.5f;

float avanceHeli = 0.012f;

double deltaTime;
double currTime, lastTime;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	screenWidth = width;
	screenHeight = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (bFullScreen)
		window = glfwCreateWindow(width, height, strTitle.c_str(),
				glfwGetPrimaryMonitor(), nullptr);
	else
		window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
				nullptr);

	if (window == nullptr) {
		std::cerr
				<< "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
				<< std::endl;
		destroy();
		exit(-1);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glfwSetWindowSizeCallback(window, reshapeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Init glew
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Failed to initialize glew" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Inicialización de los shaders
	shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
	shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
	shaderMulLighting.initialize("../Shaders/iluminacion_texture_res.vs", "../Shaders/multipleLights.fs");

	// Inicializacion de los objetos.
	skyboxSphere.init();
	skyboxSphere.setShader(&shaderSkybox);
	skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

	boxCesped.init();
	boxCesped.setShader(&shaderMulLighting);

	boxWalls.init();
	boxWalls.setShader(&shaderMulLighting);

	boxHighway.init();
	boxHighway.setShader(&shaderMulLighting);

	boxLandingPad.init();
	boxLandingPad.setShader(&shaderMulLighting);

	esfera1.init();
	esfera1.setShader(&shaderMulLighting);

	modelRock.loadModel("../models/rock/rock.obj");
	modelRock.setShader(&shaderMulLighting);

	modelAircraft.loadModel("../models/Aircraft_obj/E 45 Aircraft_obj.obj");
	modelAircraft.setShader(&shaderMulLighting);

	// Eclipse
	modelEclipseChasis.loadModel("../models/Eclipse/2003eclipse_chasis.obj");
	modelEclipseChasis.setShader(&shaderMulLighting);
	modelEclipseFrontalWheels.loadModel("../models/Eclipse/2003eclipse_frontal_wheels.obj");
	modelEclipseFrontalWheels.setShader(&shaderMulLighting);
	modelEclipseRearWheels.loadModel("../models/Eclipse/2003eclipse_rear_wheels.obj");
	modelEclipseRearWheels.setShader(&shaderMulLighting);
	// Helicopter
	modelHeliChasis.loadModel("../models/Helicopter/Mi_24_chasis.obj");
	modelHeliChasis.setShader(&shaderMulLighting);
	modelHeliHeli.loadModel("../models/Helicopter/Mi_24_heli.obj");
	modelHeliHeli.setShader(&shaderMulLighting);
	modelHeliRear.loadModel("../models/Helicopter/Mi_24_heli_rear.obj");
	modelHeliRear.setShader(&shaderMulLighting);
	// Lamborginhi
	modelLambo.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_chasis.obj");
	modelLambo.setShader(&shaderMulLighting);
	modelLamboLeftDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_left_dor.obj");
	modelLamboLeftDor.setShader(&shaderMulLighting);
	modelLamboRightDor.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_right_dor.obj");
	modelLamboRightDor.setShader(&shaderMulLighting);
	modelLamboFrontLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_left_wheel.obj");
	modelLamboFrontLeftWheel.setShader(&shaderMulLighting);
	modelLamboFrontRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_front_right_wheel.obj");
	modelLamboFrontRightWheel.setShader(&shaderMulLighting);
	modelLamboRearLeftWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_left_wheel.obj");
	modelLamboRearLeftWheel.setShader(&shaderMulLighting);
	modelLamboRearRightWheel.loadModel("../models/Lamborginhi_Aventador_OBJ/Lamborghini_Aventador_rear_right_wheel.obj");
	modelLamboRearRightWheel.setShader(&shaderMulLighting);

	// Dart Lego
	modelDartLegoBody.loadModel("../models/LegoDart/LeoDart_body.obj");
	modelDartLegoBody.setShader(&shaderMulLighting);
	modelDartLegoMask.loadModel("../models/LegoDart/LeoDart_mask.obj");
	modelDartLegoMask.setShader(&shaderMulLighting);
	modelDartLegoHead.loadModel("../models/LegoDart/LeoDart_head.obj");
	modelDartLegoHead.setShader(&shaderMulLighting);
	modelDartLegoLeftArm.loadModel("../models/LegoDart/LeoDart_left_arm.obj");
	modelDartLegoLeftArm.setShader(&shaderMulLighting);
	modelDartLegoRightArm.loadModel("../models/LegoDart/LeoDart_right_arm.obj");
	modelDartLegoRightArm.setShader(&shaderMulLighting);
	modelDartLegoLeftHand.loadModel("../models/LegoDart/LeoDart_left_hand.obj");
	modelDartLegoLeftHand.setShader(&shaderMulLighting);
	modelDartLegoRightHand.loadModel("../models/LegoDart/LeoDart_right_hand.obj");
	modelDartLegoRightHand.setShader(&shaderMulLighting);
	modelDartLegoLeftLeg.loadModel("../models/LegoDart/LeoDart_left_leg.obj");
	modelDartLegoLeftLeg.setShader(&shaderMulLighting);
	modelDartLegoRightLeg.loadModel("../models/LegoDart/LeoDart_right_leg.obj");
	modelDartLegoRightLeg.setShader(&shaderMulLighting);

	// Pokeball
	modelPokeball.loadModel("../models/Pokeball/Pokeball.obj");
	modelPokeball.setShader(&shaderMulLighting);

	// Healing Machine
	modelHealingMachine.loadModel("../models/HealingMachine/HealingMachine.obj");
	modelHealingMachine.setShader(&shaderMulLighting);

	// Buzz lightyeat
	modelBuzzBody.loadModel("../models/buzz/buzzlightyTorso.obj");
	modelBuzzBody.setShader(&shaderMulLighting);
	modelBuzzHead.loadModel("../models/buzz/buzzlightyHead.obj");
	modelBuzzHead.setShader(&shaderMulLighting);
	modelBuzzHip.loadModel("../models/buzz/buzzlightyHip.obj");
	modelBuzzHip.setShader(&shaderMulLighting);
	modelBuzzLeftArm.loadModel("../models/buzz/buzzlightyLeftArm.obj");
	modelBuzzLeftArm.setShader(&shaderMulLighting);
	modelBuzzRightArm.loadModel("../models/buzz/buzzlightyRightArm.obj");
	modelBuzzRightArm.setShader(&shaderMulLighting);
	modelBuzzLeftForearm.loadModel("../models/buzz/buzzlightyLeftForearm.obj");
	modelBuzzLeftForearm.setShader(&shaderMulLighting);
	modelBuzzRightForearm.loadModel("../models/buzz/buzzlightyRightForearm.obj");
	modelBuzzRightForearm.setShader(&shaderMulLighting);
	modelBuzzLeftHand.loadModel("../models/buzz/buzzlightyLeftHand.obj");
	modelBuzzLeftHand.setShader(&shaderMulLighting);
	modelBuzzRightHand.loadModel("../models/buzz/buzzlightyRightHand.obj");
	modelBuzzRightHand.setShader(&shaderMulLighting);
	modelBuzzLeftLeg.loadModel("../models/buzz/buzzlightyLeftThigh.obj");
	modelBuzzLeftLeg.setShader(&shaderMulLighting);
	modelBuzzRightLeg.loadModel("../models/buzz/buzzlightyRightThigh.obj");
	modelBuzzRightLeg.setShader(&shaderMulLighting);
	modelBuzzLeftCalf.loadModel("../models/buzz/buzzlightyLeftCalf.obj");
	modelBuzzLeftCalf.setShader(&shaderMulLighting);
	modelBuzzRightCalf.loadModel("../models/buzz/buzzlightyRightCalf.obj");
	modelBuzzRightCalf.setShader(&shaderMulLighting);
	modelBuzzLeftFoot.loadModel("../models/buzz/buzzlightyLeftFoot.obj");
	modelBuzzLeftFoot.setShader(&shaderMulLighting);
	modelBuzzRightFoot.loadModel("../models/buzz/buzzlightyRightFoot.obj");
	modelBuzzRightFoot.setShader(&shaderMulLighting);
	modelBuzzLeftWing1.loadModel("../models/buzz/buzzlightyLeftWing1.obj");
	modelBuzzLeftWing1.setShader(&shaderMulLighting);
	modelBuzzRightWing1.loadModel("../models/buzz/buzzlightyRightWing1.obj");
	modelBuzzRightWing1.setShader(&shaderMulLighting);
	modelBuzzLeftWing2.loadModel("../models/buzz/buzzlightyLeftWing2.obj");
	modelBuzzLeftWing2.setShader(&shaderMulLighting);
	modelBuzzRightWing2.loadModel("../models/buzz/buzzlightyRightWing2.obj");
	modelBuzzRightWing2.setShader(&shaderMulLighting);


	camera->setPosition(glm::vec3(0.0, 3.0, 4.0));
	
	// Carga de texturas para el skybox
	Texture skyboxTexture = Texture("");
	glGenTextures(1, &skyboxTextureID);
	// Tipo de textura CUBE MAP
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
		skyboxTexture = Texture(fileNames[i]);
		skyboxTexture.loadImage(true);
		if (skyboxTexture.getData()) {
			glTexImage2D(types[i], 0, skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, skyboxTexture.getWidth(), skyboxTexture.getHeight(), 0,
			skyboxTexture.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, skyboxTexture.getData());
		} else
			std::cout << "Failed to load texture" << std::endl;
		skyboxTexture.freeImage();
	}

	// Definiendo la textura a utilizar
	Texture textureCesped("../Textures/grassy2.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureCesped.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureCespedID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureCespedID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureCesped.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		std::cout << "Numero de canales :=> " << textureCesped.getChannels() << std::endl;
		glTexImage2D(GL_TEXTURE_2D, 0, textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, textureCesped.getWidth(), textureCesped.getHeight(), 0,
		textureCesped.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureCesped.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureCesped.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWall("../Textures/whiteWall.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWall.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWallID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWallID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWall.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWall.getWidth(), textureWall.getHeight(), 0,
		textureWall.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWall.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWall.freeImage();

	// Definiendo la textura a utilizar
	Texture textureWindow("../Textures/ventana.png");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureWindow.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureWindowID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureWindowID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureWindow.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, textureWindow.getWidth(), textureWindow.getHeight(), 0,
		textureWindow.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureWindow.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureWindow.freeImage();

	// Definiendo la textura a utilizar
	Texture textureHighway("../Textures/highway.jpg");
	// Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
	textureHighway.loadImage();
	// Creando la textura con id 1
	glGenTextures(1, &textureHighwayID);
	// Enlazar esa textura a una tipo de textura de 2D.
	glBindTexture(GL_TEXTURE_2D, textureHighwayID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Verifica si se pudo abrir la textura
	if (textureHighway.getData()) {
		// Transferis los datos de la imagen a memoria
		// Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
		// Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
		// a los datos
		glTexImage2D(GL_TEXTURE_2D, 0, textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHighway.getWidth(), textureHighway.getHeight(), 0,
		textureHighway.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHighway.getData());
		// Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
		std::cout << "Failed to load texture" << std::endl;
	// Libera la memoria de la textura
	textureHighway.freeImage();

	// Definiendo la textura
	Texture textureLandingPad("../Textures/landingPad.jpg");
	textureLandingPad.loadImage(); // Cargar la textura
	glGenTextures(1, &textureLandingPadID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureLandingPadID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureLandingPad.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, textureLandingPad.getWidth(), textureLandingPad.getHeight(), 0,
		textureLandingPad.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureLandingPad.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureLandingPad.freeImage(); // Liberamos memoria

	// Definiendo la textura de la pokeball
	Texture texturePokeball("../models/Pokeball/Pokeball.png");
	texturePokeball.loadImage(); // Cargar la textura
	glGenTextures(1, &texturePokeballID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, texturePokeballID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(texturePokeball.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, texturePokeball.getChannels() == 3 ? GL_RGB : GL_RGBA, texturePokeball.getWidth(), texturePokeball.getHeight(), 0,
		texturePokeball.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texturePokeball.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	texturePokeball.freeImage(); // Liberamos memoria

	// Definiendo la textura de la Healing Machine
	Texture textureHealingMachineBody("../models/HealingMachine/HealingMachineBody.png");
	textureHealingMachineBody.loadImage(); // Cargar la textura
	glGenTextures(1, &textureHealingMachineBodyID); // Creando el id de la textura del landingpad
	glBindTexture(GL_TEXTURE_2D, textureHealingMachineBodyID); // Se enlaza la textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Wrapping en el eje u
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Wrapping en el eje v
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Filtering de minimización
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Filtering de maximimizacion
	if(textureHealingMachineBody.getData()){
		// Transferir los datos de la imagen a la tarjeta
		glTexImage2D(GL_TEXTURE_2D, 0, textureHealingMachineBody.getChannels() == 3 ? GL_RGB : GL_RGBA, textureHealingMachineBody.getWidth(), textureHealingMachineBody.getHeight(), 0,
		textureHealingMachineBody.getChannels() == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, textureHealingMachineBody.getData());
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else 
		std::cout << "Fallo la carga de textura" << std::endl;
	textureHealingMachineBody.freeImage(); // Liberamos memoria

}

void destroy() {
	glfwDestroyWindow(window);
	glfwTerminate();
	// --------- IMPORTANTE ----------
	// Eliminar los shader y buffers creados.

	// Shaders Delete
	shader.destroy();
	shaderMulLighting.destroy();
	shaderSkybox.destroy();

	// Basic objects Delete
	skyboxSphere.destroy();
	boxCesped.destroy();
	boxWalls.destroy();
	boxHighway.destroy();
	boxLandingPad.destroy();
	esfera1.destroy();

	// Custom objects Delete
	modelAircraft.destroy();
	modelDartLegoBody.destroy();
	modelDartLegoHead.destroy();
	modelDartLegoLeftArm.destroy();
	modelDartLegoLeftHand.destroy();
	modelDartLegoLeftLeg.destroy();
	modelDartLegoMask.destroy();
	modelDartLegoRightArm.destroy();
	modelDartLegoRightHand.destroy();
	modelDartLegoRightLeg.destroy();
	modelEclipseChasis.destroy();
	modelEclipseFrontalWheels.destroy();
	modelEclipseRearWheels.destroy();
	modelHeliChasis.destroy();
	modelHeliHeli.destroy();
	modelHeliRear.destroy();
	modelLambo.destroy();
	modelLamboFrontLeftWheel.destroy();
	modelLamboFrontRightWheel.destroy();
	modelLamboLeftDor.destroy();
	modelLamboRearLeftWheel.destroy();
	modelLamboRearRightWheel.destroy();
	modelLamboRightDor.destroy();
	modelRock.destroy();
	modelPokeball.destroy();
	modelHealingMachine.destroy();

	modelBuzzBody.destroy();
	modelBuzzHead.destroy();
	modelBuzzHip.destroy();
	modelBuzzLeftArm.destroy();
	modelBuzzRightArm.destroy();
	modelBuzzLeftForearm.destroy();
	modelBuzzRightForearm.destroy();
	modelBuzzLeftHand.destroy();
	modelBuzzRightHand.destroy();
	modelBuzzLeftLeg.destroy();
	modelBuzzRightLeg.destroy();
	modelBuzzLeftCalf.destroy();
	modelBuzzRightCalf.destroy();
	modelBuzzLeftFoot.destroy();
	modelBuzzRightFoot.destroy();
	modelBuzzLeftWing1.destroy();
	modelBuzzRightWing1.destroy();
	modelBuzzLeftWing2.destroy();
	modelBuzzRightWing2.destroy();

	// Textures Delete
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &textureCespedID);
	glDeleteTextures(1, &textureWallID);
	glDeleteTextures(1, &textureWindowID);
	glDeleteTextures(1, &textureHighwayID);
	glDeleteTextures(1, &textureLandingPadID);
	glDeleteTextures(1, &texturePokeballID);
	glDeleteTextures(1, &textureHealingMachineBodyID);

	// Cube Maps Delete
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
	screenWidth = widthRes;
	screenHeight = heightRes;
	glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
		int mode) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			exitApp = true;
			break;
		}
	}
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
	offsetX = xpos - lastMousePosX;
	offsetY = ypos - lastMousePosY;
	lastMousePosX = xpos;
	lastMousePosY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
	if (state == GLFW_PRESS) {
		switch (button) {
		case GLFW_MOUSE_BUTTON_RIGHT:
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
			std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
			break;
		}
	}
}

bool processInput(bool continueApplication) {
	if (exitApp || glfwWindowShouldClose(window) != 0) {
		return false;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveFrontCamera(true, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveFrontCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveRightCamera(false, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRightCamera(true, deltaTime);
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		camera->mouseMoveCamera(offsetX, offsetY, deltaTime);
	offsetX = 0;
	offsetY = 0;

	// Seleccionar modelo
	if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
		enableCountSelected = false;
		modelSelected++;
		if(modelSelected > 4)
			modelSelected = 0;
		if(modelSelected == 1)
			fileName = "../animaciones/animation_dart_joints.txt";
		if (modelSelected == 2)
			fileName = "../animaciones/animation_dart.txt";
		if(modelSelected == 3)
			fileName = "../animaciones/animation_buzz_joints.txt";
		if (modelSelected == 4)
			fileName = "../animaciones/animation_buzz.txt";
		std::cout << "modelSelected:" << modelSelected << std::endl;
	}
	else if(glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
		enableCountSelected = true;

	// Guardar key frames
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = true;
		if(myfile.is_open())
			myfile.close();
		myfile.open(fileName);
	}
	if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE
			&& glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
		record = false;
		myfile.close();
		if(modelSelected == 1)
			keyFramesDartJoints = getKeyRotFrames(fileName);
		if (modelSelected == 2)
			keyFramesDart = getKeyFrames(fileName);
		if(modelSelected == 3)
			keyFramesBuzzJoints = getKeyRotFrames(fileName);
		if (modelSelected == 4)
			keyFramesBuzz = getKeyFrames(fileName);
	}
	if(availableSave && glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
		saveFrame = true;
		availableSave = false;
	}if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE)
		availableSave = true;

	// Dart Lego model movements
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotDartHead -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotDartLeftArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotDartRightArm -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotDartLeftHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotDartRightHand -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotDartLeftLeg -= 0.02;
	if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg += 0.02;
	else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotDartRightLeg -= 0.02;
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixDart = glm::rotate(modelMatrixDart, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(-0.02, 0.0, 0.0));
	else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(0.02, 0.0, 0.0));
	
	// Buzz movements
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzBody += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		rotBuzzBody -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzHead += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		rotBuzzHead -= 0.02; 
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzHip += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		rotBuzzHip -= 0.02; 
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzLeftArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		rotBuzzLeftArm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzLeftForearm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
		rotBuzzLeftForearm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
		rotBuzzLeftHand -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzRightArm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
		rotBuzzRightArm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		rotBuzzRightForearm += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		rotBuzzRightForearm -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		rotBuzzRightHand += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		rotBuzzRightHand -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		rotBuzzLeftLeg += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		rotBuzzLeftLeg -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		rotBuzzLeftCalf += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		rotBuzzLeftCalf -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		rotBuzzLeftFoot += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		rotBuzzLeftFoot -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		rotBuzzRightLeg += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		rotBuzzRightLeg -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		rotBuzzRightCalf += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		rotBuzzRightCalf -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotBuzzRightFoot += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotBuzzRightFoot -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		rotBuzzLeftWing1 += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		rotBuzzLeftWing1  -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		rotBuzzRightWing1 += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		rotBuzzRightWing1  -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		rotBuzzLeftWing2 += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		rotBuzzLeftWing2  -= 0.02;
	if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE &&
			glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		rotBuzzRightWing2 += 0.02;
	else if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS &&
			glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		rotBuzzRightWing2  -= 0.02;
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, 0.02f, glm::vec3(0, 1, 0));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		modelMatrixBuzz = glm::rotate(modelMatrixBuzz, -0.02f, glm::vec3(0, 1, 0));
	if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, 0.02));
	else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(0.0, 0.0, -0.02));
	
	
	glfwPollEvents();
	return continueApplication;
}

void applicationLoop() {
	bool psi = true;

	modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(27.5, 0, 30.0));
	modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(180.0f), glm::vec3(0, 1, 0));
	int state = 0;
	int stateLambo = 0;
	int stateHeli = 0;
	float advanceCount = 0.0;
	float rotCount = 0.0;
	float rotCountHeli = 0.0;
	float advanceCountLambo = 0.0;
	float advanceCountHeli = 0.0;
	float rotCountLambo = 0.0;
	float rotWheelsX = 0.0;
	float rotWheelsY = 0.0;
	float rotWheelsLamboX = 0.0;
	float rotWheelsLamboY = 0.0;
	int numberAdvance = 0;
	int maxAdvance = 0.0;
	int numberAdvanceLambo = 0;
	int maxAdvanceLambo = 0.0;
	int numberAdvanceHeli = 0;
	int maxAdvanceHeli = 0.0;

	matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

	modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(5.0, 10.0, -5.0));

	modelMatrixAircraft = glm::translate(modelMatrixAircraft, glm::vec3(10.0, 2.0, -17.5));

	modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(23.0, 0.0, 0.0));

	modelMatrixDart = glm::translate(modelMatrixDart, glm::vec3(3.0, 0.0, 20.0));

	modelMatrixBuzz = glm::translate(modelMatrixBuzz, glm::vec3(15.0, 0.0, -10.0));

	modelMatrixPokeball = glm::translate(modelMatrixPokeball, glm::vec3(15.0, 1.5, 20.0));

	modelMatrixHealingMachine = glm::translate(modelMatrixHealingMachine, glm::vec3(15.0, 0.0, 20.0));

	// Variables to interpolation key frames
	fileName = "../animaciones/animation_dart_joints.txt";
	keyFramesDartJoints = getKeyRotFrames(fileName);
	keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");
	keyFramesBuzzJoints = getKeyRotFrames("../animaciones/animation_buzz_joints.txt");
	keyFramesBuzz = getKeyFrames("../animaciones/animation_buzz.txt");

	lastTime = TimeManager::Instance().GetTime();

	while (psi) {
		currTime = TimeManager::Instance().GetTime();
		if(currTime - lastTime < 0.016666667){
			glfwPollEvents();
			continue;
		}
		lastTime = currTime;
		TimeManager::Instance().CalculateFrameRate(true);
		deltaTime = TimeManager::Instance().DeltaTime;
		psi = processInput(true);

		// Variables donde se guardan las matrices de cada articulacion por 1 frame
		std::vector<float> matrixDartJoints;
		std::vector<glm::mat4> matrixDart;
		std::vector<float> matrixBuzzJoints;
		std::vector<glm::mat4> matrixBuzz;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(45.0f),
				(float) screenWidth / (float) screenHeight, 0.01f, 100.0f);
		glm::mat4 view = camera->getViewMatrix();

		// Settea la matriz de vista y projection al shader con solo color
		shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
		shader.setMatrix4("view", 1, false, glm::value_ptr(view));

		// Settea la matriz de vista y projection al shader con skybox
		shaderSkybox.setMatrix4("projection", 1, false,
				glm::value_ptr(projection));
		shaderSkybox.setMatrix4("view", 1, false,
				glm::value_ptr(glm::mat4(glm::mat3(view))));
		// Settea la matriz de vista y projection al shader con multiples luces
		shaderMulLighting.setMatrix4("projection", 1, false,
					glm::value_ptr(projection));
		shaderMulLighting.setMatrix4("view", 1, false,
				glm::value_ptr(view));

		/*******************************************
		 * Propiedades Luz direccional
		 *******************************************/
		shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(camera->getPosition()));
		shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.3, 0.3, 0.3)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.7, 0.7, 0.7)));
		shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.9, 0.9, 0.9)));
		shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

		/*******************************************
		 * Propiedades SpotLights
		 *******************************************/
		shaderMulLighting.setInt("spotLightCount", 0);

		/*******************************************
		 * Propiedades PointLights
		 *******************************************/
		shaderMulLighting.setInt("pointLightCount", 0);

		/*******************************************
		 * Cesped
		 *******************************************/
		glm::mat4 modelCesped = glm::mat4(1.0);
		modelCesped = glm::translate(modelCesped, glm::vec3(0.0, 0.0, 0.0));
		modelCesped = glm::scale(modelCesped, glm::vec3(200.0, 0.001, 200.0));
		// Se activa la textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureCespedID);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(200, 200)));
		boxCesped.render(modelCesped);
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************ww
		 * Casa
		 *******************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWallID);
		// Frontal wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, 0.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Left wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-15.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Right walls
		// First wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -1.875));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Second wall
		boxWalls.setScale(glm::vec3(3.75, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -8.125));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Third wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 4.375, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Four wall
		boxWalls.setScale(glm::vec3(2.5, 1.25, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 0.625, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Back wall
		boxWalls.setScale(glm::vec3(10.0, 5.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 2.5, -10.0));
		boxWalls.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxWalls.render();
		// Ceilling
		boxWalls.setScale(glm::vec3(12.0, 12.0, 0.05));
		boxWalls.setPosition(glm::vec3(-10.0, 5, -5.0));
		boxWalls.setOrientation(glm::vec3(90.0, 0.0, 0.0));
		boxWalls.render();
		// Window
		glBindTexture(GL_TEXTURE_2D, textureWindowID);
		boxWalls.setScale(glm::vec3(2.5, 2.5, 0.05));
		boxWalls.setPosition(glm::vec3(-5.0, 2.5, -5.0));
		boxWalls.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		boxWalls.render();
		// Highway 0
		glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, 10.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();
		// Highway 1
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 2
		boxHighway.setScale(glm::vec3(80.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(-25.0, 0.05, 0.0));
		boxHighway.setOrientation(glm::vec3(0.0, 90.0, 0.0));
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2, 1)));
		boxHighway.render();
		shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
		// Highway 3
		boxHighway.setScale(glm::vec3(40.0, 0.05, 10.0));
		boxHighway.setPosition(glm::vec3(0.0, 0.05, -35.0));
		boxHighway.setOrientation(glm::vec3(0.0, 0.0, 0.0));
		boxHighway.render();

		/*******************************************
		 * Esfera 1
		*********************************************/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHighwayID);
		shaderMulLighting.setInt("texture1", 0);
		esfera1.setScale(glm::vec3(3.0, 3.0, 3.0));
		esfera1.setPosition(glm::vec3(3.0f, 2.0f, -10.0f));
		esfera1.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureWallID);
		shaderMulLighting.setInt("texture1", 0);
		esfera1.setScale(glm::vec3(10.0, 10.0, 10.0));
		esfera1.setPosition(glm::vec3(3.0f, 2.0f, 10.0f));
		esfera1.enableWireMode();
		esfera1.render();
		esfera1.enableFillMode();

		/******************************************
		 * Landing pad
		*******************************************/
		boxLandingPad.setScale(glm::vec3(10.0f, 0.05f, 10.0f));
		boxLandingPad.setPosition(glm::vec3(5.0f, 0.05f, -5.0f));
		boxLandingPad.setOrientation(glm::vec3(0.0f, 0.0f, 0.0f));
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureLandingPadID);
		shaderMulLighting.setInt("texture1", 0);
		//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(2.0, 2.0)));
		boxLandingPad.render();
		//shaderMulLighting.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(1.0, 1.0)));
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Custom objects obj
		 *******************************************/
		//Rock render
		modelRock.render(matrixModelRock);
		// Forze to enable the unit texture to 0 always ----------------- IMPORTANT
		glActiveTexture(GL_TEXTURE0);

		// Render for the aircraft model
		modelAircraft.render(modelMatrixAircraft);

		// Render for the eclipse car
		glm::mat4 modelMatrixEclipseChasis = glm::mat4(modelMatrixEclipse);
		modelMatrixEclipseChasis = glm::scale(modelMatrixEclipse, glm::vec3(0.5, 0.5, 0.5));
		modelEclipseChasis.render(modelMatrixEclipseChasis);

		glm::mat4 modelMatrixFrontalWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, 1.05813, 4.11483 ));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsY, glm::vec3(0, 1, 0));
		modelMatrixFrontalWheels = glm::rotate(modelMatrixFrontalWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixFrontalWheels = glm::translate(modelMatrixFrontalWheels, glm::vec3(0.0, -1.05813, -4.11483));
		modelEclipseFrontalWheels.render(modelMatrixFrontalWheels);

		glm::mat4 modelMatrixRearWheels = glm::mat4(modelMatrixEclipseChasis);
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, 1.05813, -4.35157 ));
		modelMatrixRearWheels = glm::rotate(modelMatrixRearWheels, rotWheelsX, glm::vec3(1, 0, 0));
		modelMatrixRearWheels = glm::translate(modelMatrixRearWheels, glm::vec3(0.0, -1.05813, 4.35157));
		modelEclipseRearWheels.render(modelMatrixRearWheels);

		// Helicopter
		glm::mat4 modelMatrixHeliChasis = glm::mat4(modelMatrixHeli);
		modelHeliChasis.render(modelMatrixHeliChasis);

		glm::mat4 modelMatrixHeliHeli = glm::mat4(modelMatrixHeliChasis);
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, -0.249548));
		modelMatrixHeliHeli = glm::rotate(modelMatrixHeliHeli, rotHelHelY, glm::vec3(0, 1, 0));
		modelMatrixHeliHeli = glm::translate(modelMatrixHeliHeli, glm::vec3(0.0, 0.0, 0.249548));
		modelHeliHeli.render(modelMatrixHeliHeli);

		glm::mat4 modelMatrixHeliRear = glm::mat4(modelMatrixHeliChasis);//Para que mi matriz de helice herede transformaciones de la principal.
		modelMatrixHeliRear = glm::translate(modelMatrixHeliRear, glm::vec3(0.0f,2.09454f,-5.63825f));
		modelMatrixHeliRear = glm::rotate(modelMatrixHeliRear, rotHelHelY, glm::vec3(1.0f,0.0f,0.0f));
		modelMatrixHeliRear = glm::translate(modelMatrixHeliRear, glm::vec3(0.0f,-2.09454f,5.63825f));
		modelHeliRear.render(modelMatrixHeliRear);

		// Lambo car
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixLamboChasis = glm::mat4(modelMatrixLambo);
		modelMatrixLamboChasis = glm::scale(modelMatrixLamboChasis, glm::vec3(1.3, 1.3, 1.3));
		modelLambo.render(modelMatrixLamboChasis);
		glActiveTexture(GL_TEXTURE0);
		glm::mat4 modelMatrixLamboLeftDor = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(1.08816, 0.704289, 0.980277));
		modelMatrixLamboLeftDor = glm::rotate(modelMatrixLamboLeftDor, glm::radians(dorRotCount), glm::vec3(1.0, 0, 0));
		modelMatrixLamboLeftDor = glm::translate(modelMatrixLamboLeftDor, glm::vec3(-1.08816, -0.704289, -0.980277));
		modelLamboLeftDor.render(modelMatrixLamboLeftDor);
		modelLamboRightDor.render(modelMatrixLamboChasis);
		
		glm::mat4 modelMatrixLamboFrontalWheels = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboFrontalWheels = glm::translate(modelMatrixLamboFrontalWheels, glm::vec3(0.0, 0.36, 1.4 ));
		modelMatrixLamboFrontalWheels = glm::rotate(modelMatrixLamboFrontalWheels, -rotWheelsLamboY, glm::vec3(0, 1, 0));
		modelMatrixLamboFrontalWheels = glm::rotate(modelMatrixLamboFrontalWheels, rotWheelsLamboX, glm::vec3(1, 0, 0));
		modelMatrixLamboFrontalWheels = glm::translate(modelMatrixLamboFrontalWheels, glm::vec3(0.0, -0.36, -1.4));
		modelLamboFrontLeftWheel.render(modelMatrixLamboFrontalWheels);
		modelLamboFrontRightWheel.render(modelMatrixLamboFrontalWheels);

		glm::mat4 modelMatrixLamboRearWheels = glm::mat4(modelMatrixLamboChasis);
		modelMatrixLamboRearWheels = glm::translate(modelMatrixLamboRearWheels, glm::vec3(0.0, 0.4, -1.6 ));
		modelMatrixLamboRearWheels = glm::rotate(modelMatrixLamboRearWheels, rotWheelsLamboX, glm::vec3(1, 0, 0));
		modelMatrixLamboRearWheels = glm::translate(modelMatrixLamboRearWheels, glm::vec3(0.0, -0.4, 1.6));
		modelLamboRearLeftWheel.render(modelMatrixLamboRearWheels);
		modelLamboRearRightWheel.render(modelMatrixLamboRearWheels);

		// Se regresa el cull faces IMPORTANTE para las puertas
		glEnable(GL_CULL_FACE);

		// Dart lego
		// Se deshabilita el cull faces IMPORTANTE para la capa
		glDisable(GL_CULL_FACE);
		glm::mat4 modelMatrixDartBody = glm::mat4(modelMatrixDart);
		modelMatrixDartBody = glm::scale(modelMatrixDartBody, glm::vec3(0.5, 0.5, 0.5));
		modelDartLegoBody.render(modelMatrixDartBody);
		glm::mat4 modelMatrixDartHead = glm::mat4(modelMatrixDartBody);
		modelMatrixDartHead = glm::rotate(modelMatrixDartHead, rotDartHead, glm::vec3(0, 1, 0));
		modelDartLegoHead.render(modelMatrixDartHead);
		modelDartLegoMask.render(modelMatrixDartHead);
		glm::mat4 modelMatrixDartLeftArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(-0.023515, 2.43607, 0.446066));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, rotDartLeftArm, glm::vec3(0, 0, 1));
		modelMatrixDartLeftArm = glm::rotate(modelMatrixDartLeftArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftArm = glm::translate(modelMatrixDartLeftArm, glm::vec3(0.023515, -2.43607, -0.446066));
		modelDartLegoLeftArm.render(modelMatrixDartLeftArm);
		glm::mat4 modelMatrixDartLeftHand = glm::mat4(modelMatrixDartLeftArm);
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(0.201343, 1.68317, 0.99774));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, rotDartLeftHand, glm::vec3(0, 1, 0));
		modelMatrixDartLeftHand = glm::rotate(modelMatrixDartLeftHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartLeftHand = glm::translate(modelMatrixDartLeftHand, glm::vec3(-0.201343, -1.68317, -0.99774));
		modelDartLegoLeftHand.render(modelMatrixDartLeftHand);
		glm::mat4 modelMatrixDartRightArm = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(-0.023515, 2.43607, -0.446066));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, rotDartRightArm, glm::vec3(0, 0, 1));
		modelMatrixDartRightArm = glm::rotate(modelMatrixDartRightArm, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightArm = glm::translate(modelMatrixDartRightArm, glm::vec3(0.023515, -2.43607, 0.446066));
		modelDartLegoRightArm.render(modelMatrixDartRightArm);
		glm::mat4 modelMatrixDartRightHand = glm::mat4(modelMatrixDartRightArm);
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(0.201343, 1.68317, -0.99774));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, rotDartRightHand, glm::vec3(0, 1, 0));
		modelMatrixDartRightHand = glm::rotate(modelMatrixDartRightHand, glm::radians(-5.0f), glm::vec3(1, 0, 0));
		modelMatrixDartRightHand = glm::translate(modelMatrixDartRightHand, glm::vec3(-0.201343, -1.68317, 0.99774));
		modelDartLegoRightHand.render(modelMatrixDartRightHand);
		glm::mat4 modelMatrixDartLeftLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, 1.12632, 0.423349));
		modelMatrixDartLeftLeg = glm::rotate(modelMatrixDartLeftLeg, rotDartLeftLeg, glm::vec3(0, 0, 1));
		modelMatrixDartLeftLeg = glm::translate(modelMatrixDartLeftLeg, glm::vec3(0, -1.12632, -0.423349));
		modelDartLegoLeftLeg.render(modelMatrixDartLeftLeg);
		glm::mat4 modelMatrixDartRightLeg = glm::mat4(modelMatrixDartBody);
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, 1.12632, -0.423349));
		modelMatrixDartRightLeg = glm::rotate(modelMatrixDartRightLeg, rotDartRightLeg, glm::vec3(0, 0, 1));
		modelMatrixDartRightLeg = glm::translate(modelMatrixDartRightLeg, glm::vec3(0, -1.12632, 0.423349));
		modelDartLegoRightLeg.render(modelMatrixDartRightLeg);
		// Se regresa el cull faces IMPORTANTE para la capa
		glEnable(GL_CULL_FACE);

		// Buzz lightyear

		glm::mat4 modelMatrixBuzzBody = glm::mat4(modelMatrixBuzz);
		modelMatrixBuzzBody = glm::scale(modelMatrixBuzzBody, glm::vec3(3.0));
		modelMatrixBuzzBody = glm::rotate(modelMatrixBuzzBody, rotBuzzBody, glm::vec3(0, 1, 0));
		modelBuzzBody.render(modelMatrixBuzzBody);

		glm::mat4 modelMatrixBuzzHead = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzHead = glm::translate(modelMatrixBuzzHead, glm::vec3(0, 0.6, 0.0));
		modelMatrixBuzzHead = glm::rotate(modelMatrixBuzzHead, rotBuzzHead, glm::vec3(0, 1, 0));
		modelMatrixBuzzHead = glm::translate(modelMatrixBuzzHead, glm::vec3(0, -0.6, 0.0));
		modelBuzzHead.render(modelMatrixBuzzHead);

		glm::mat4 modelMatrixBuzzLeftWing1 = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftWing1 = glm::translate(modelMatrixBuzzLeftWing1, glm::vec3(0.065, 0.56, -0.13));
		modelMatrixBuzzLeftWing1 = glm::rotate(modelMatrixBuzzLeftWing1, rotBuzzLeftWing1, glm::vec3(0, 1, 0));
		modelMatrixBuzzLeftWing1 = glm::translate(modelMatrixBuzzLeftWing1, glm::vec3(-0.065, -0.56, 0.13));
		modelBuzzLeftWing1.render(modelMatrixBuzzLeftWing1);

		glm::mat4 modelMatrixBuzzLeftWing2 = glm::mat4(modelMatrixBuzzLeftWing1);
		modelMatrixBuzzLeftWing2 = glm::translate(modelMatrixBuzzLeftWing2, glm::vec3(0.325, 0.55, -0.13));
		modelMatrixBuzzLeftWing2 = glm::rotate(modelMatrixBuzzLeftWing2, rotBuzzLeftWing2, glm::vec3(0, 1, 0));
		modelMatrixBuzzLeftWing2 = glm::translate(modelMatrixBuzzLeftWing2, glm::vec3(-0.325, -0.55, 0.13));
		modelBuzzLeftWing2.render(modelMatrixBuzzLeftWing2);

		glm::mat4 modelMatrixBuzzRightWing1 = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightWing1 = glm::translate(modelMatrixBuzzRightWing1, glm::vec3(-0.065, 0.56, -0.13));
		modelMatrixBuzzRightWing1 = glm::rotate(modelMatrixBuzzRightWing1, rotBuzzRightWing1, glm::vec3(0, 1, 0));
		modelMatrixBuzzRightWing1 = glm::translate(modelMatrixBuzzRightWing1, glm::vec3(0.065, -0.56, 0.13));
		modelBuzzRightWing1.render(modelMatrixBuzzRightWing1);

		glm::mat4 modelMatrixBuzzRightWing2 = glm::mat4(modelMatrixBuzzRightWing1);
		modelMatrixBuzzRightWing2 = glm::translate(modelMatrixBuzzRightWing2, glm::vec3(-0.325, 0.55, -0.13));
		modelMatrixBuzzRightWing2 = glm::rotate(modelMatrixBuzzRightWing2, rotBuzzRightWing2, glm::vec3(0, 1, 0));
		modelMatrixBuzzRightWing2 = glm::translate(modelMatrixBuzzRightWing2, glm::vec3(0.325, -0.55, 0.13));
		modelBuzzRightWing2.render(modelMatrixBuzzRightWing2);

		glm::mat4 modelMatrixBuzzHip = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzHip = glm::translate(modelMatrixBuzzHip, glm::vec3(0, 0.49, 0.0));
		modelMatrixBuzzHip = glm::rotate(modelMatrixBuzzHip, rotBuzzHip, glm::vec3(0, 1, 0));
		modelMatrixBuzzHip = glm::translate(modelMatrixBuzzHip, glm::vec3(0, -0.49, 0.0));
		modelBuzzHip.render(modelMatrixBuzzHip);

		glm::mat4 modelMatrixBuzzLeftArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm, glm::vec3(0.15, 0.58, -0.02));
		modelMatrixBuzzLeftArm = glm::rotate(modelMatrixBuzzLeftArm, rotBuzzLeftArm, glm::vec3(0, 0, 1));
		modelMatrixBuzzLeftArm = glm::translate(modelMatrixBuzzLeftArm, glm::vec3(-0.15, -0.58, 0.02));
		modelBuzzLeftArm.render(modelMatrixBuzzLeftArm);

		glm::mat4 modelMatrixBuzzLeftForearm = glm::mat4(modelMatrixBuzzLeftArm);
		modelMatrixBuzzLeftForearm = glm::translate(modelMatrixBuzzLeftForearm, glm::vec3(0.27, 0.58, -0.01));
		modelMatrixBuzzLeftForearm = glm::rotate(modelMatrixBuzzLeftForearm, rotBuzzLeftForearm, glm::vec3(0, 1, 0));
		modelMatrixBuzzLeftForearm = glm::translate(modelMatrixBuzzLeftForearm, glm::vec3(-0.27, -0.58, 0.01));
		modelBuzzLeftForearm.render(modelMatrixBuzzLeftForearm);

		glm::mat4 modelMatrixBuzzLeftHand= glm::mat4(modelMatrixBuzzLeftForearm);
		modelMatrixBuzzLeftHand = glm::translate(modelMatrixBuzzLeftHand, glm::vec3(0.41, 0.59, 0.07));
		modelMatrixBuzzLeftHand = glm::rotate(modelMatrixBuzzLeftHand, glm::radians(45.0f), glm::vec3(0, 1, 0));
		modelMatrixBuzzLeftHand = glm::rotate(modelMatrixBuzzLeftHand, rotBuzzLeftHand, glm::vec3(1, 0, 0));
		modelMatrixBuzzLeftHand = glm::rotate(modelMatrixBuzzLeftHand, glm::radians(-45.0f), glm::vec3(0, 1, 0));
		modelMatrixBuzzLeftHand = glm::translate(modelMatrixBuzzLeftHand, glm::vec3(-0.41, -0.59, -0.07));
		modelBuzzLeftHand.render(modelMatrixBuzzLeftHand);

		glm::mat4 modelMatrixBuzzLeftLeg = glm::mat4(modelMatrixBuzzHip);
		modelMatrixBuzzLeftLeg = glm::translate(modelMatrixBuzzLeftLeg, glm::vec3(0.06, 0.39, 0.01));
		modelMatrixBuzzLeftLeg = glm::rotate(modelMatrixBuzzLeftLeg, rotBuzzLeftLeg, glm::vec3(1, 0, 0));
		modelMatrixBuzzLeftLeg = glm::translate(modelMatrixBuzzLeftLeg, glm::vec3(-0.06, -0.39, -0.01));
		modelBuzzLeftLeg.render(modelMatrixBuzzLeftLeg);

		glm::mat4 modelMatrixBuzzLeftCalf = glm::mat4(modelMatrixBuzzLeftLeg);
		modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf, glm::vec3(0.06, 0.22, -0.008));
		modelMatrixBuzzLeftCalf = glm::rotate(modelMatrixBuzzLeftCalf, rotBuzzLeftCalf, glm::vec3(1, 0, 0));
		modelMatrixBuzzLeftCalf = glm::translate(modelMatrixBuzzLeftCalf, glm::vec3(-0.06, -0.22, 0.008));
		modelBuzzLeftCalf.render(modelMatrixBuzzLeftCalf);

		glm::mat4 modelMatrixBuzzLeftFoot = glm::mat4(modelMatrixBuzzLeftCalf);
		modelMatrixBuzzLeftFoot = glm::translate(modelMatrixBuzzLeftFoot, glm::vec3(0.07, 0.09, -0.025));
		modelMatrixBuzzLeftFoot = glm::rotate(modelMatrixBuzzLeftFoot, rotBuzzLeftFoot, glm::vec3(1, 0, 0));
		modelMatrixBuzzLeftFoot = glm::translate(modelMatrixBuzzLeftFoot, glm::vec3(-0.07, -0.09, 0.025));
		modelBuzzLeftFoot.render(modelMatrixBuzzLeftFoot);

		glm::mat4 modelMatrixBuzzRightArm = glm::mat4(modelMatrixBuzzBody);
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm, glm::vec3(-0.15, 0.58, -0.02));
		modelMatrixBuzzRightArm = glm::rotate(modelMatrixBuzzRightArm, rotBuzzRightArm, glm::vec3(0, 0, 1));
		modelMatrixBuzzRightArm = glm::translate(modelMatrixBuzzRightArm, glm::vec3(0.15, -0.58, 0.02));
		modelBuzzRightArm.render(modelMatrixBuzzRightArm);

		glm::mat4 modelMatrixBuzzRightForearm = glm::mat4(modelMatrixBuzzRightArm);
		modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm, glm::vec3(-0.27, 0.58, -0.01));
		modelMatrixBuzzRightForearm = glm::rotate(modelMatrixBuzzRightForearm, rotBuzzRightForearm, glm::vec3(0, 1, 0));
		modelMatrixBuzzRightForearm = glm::translate(modelMatrixBuzzRightForearm, glm::vec3(0.27, -0.58, 0.01));
		modelBuzzRightForearm.render(modelMatrixBuzzRightForearm);

		glm::mat4 modelMatrixBuzzRightHand= glm::mat4(modelMatrixBuzzRightForearm);
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand, glm::vec3(-0.41, 0.59, 0.07));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, glm::radians(45.0f), glm::vec3(0, 1, 0));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, rotBuzzRightHand, glm::vec3(1, 0, 0));
		modelMatrixBuzzRightHand = glm::rotate(modelMatrixBuzzRightHand, glm::radians(-45.0f), glm::vec3(0, 1, 0));
		modelMatrixBuzzRightHand = glm::translate(modelMatrixBuzzRightHand, glm::vec3(0.41, -0.59, -0.07));
		modelBuzzRightHand.render(modelMatrixBuzzRightHand);

		glm::mat4 modelMatrixBuzzRightLeg = glm::mat4(modelMatrixBuzzHip);
		modelMatrixBuzzRightLeg = glm::translate(modelMatrixBuzzRightLeg, glm::vec3(-0.06, 0.39, 0.01));
		modelMatrixBuzzRightLeg = glm::rotate(modelMatrixBuzzRightLeg, rotBuzzRightLeg, glm::vec3(1, 0, 0));
		modelMatrixBuzzRightLeg = glm::translate(modelMatrixBuzzRightLeg, glm::vec3(0.06, -0.39, -0.01));
		modelBuzzRightLeg.render(modelMatrixBuzzRightLeg);

		glm::mat4 modelMatrixBuzzRightCalf = glm::mat4(modelMatrixBuzzRightLeg);
		modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf, glm::vec3(-0.06, 0.22, -0.008));
		modelMatrixBuzzRightCalf = glm::rotate(modelMatrixBuzzRightCalf, rotBuzzRightCalf, glm::vec3(1, 0, 0));
		modelMatrixBuzzRightCalf = glm::translate(modelMatrixBuzzRightCalf, glm::vec3(0.06, -0.22, 0.008));
		modelBuzzRightCalf.render(modelMatrixBuzzRightCalf);

		glm::mat4 modelMatrixBuzzRightFoot = glm::mat4(modelMatrixBuzzRightCalf);
		modelMatrixBuzzRightFoot = glm::translate(modelMatrixBuzzRightFoot, glm::vec3(-0.07, 0.09, -0.025));
		modelMatrixBuzzRightFoot = glm::rotate(modelMatrixBuzzRightFoot, rotBuzzRightFoot, glm::vec3(1, 0, 0));
		modelMatrixBuzzRightFoot = glm::translate(modelMatrixBuzzRightFoot, glm::vec3(0.07, -0.09, 0.025));
		modelBuzzRightFoot.render(modelMatrixBuzzRightFoot);



		// Render pokeball
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texturePokeballID);
		modelPokeball.render(modelMatrixPokeball);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Render Healing Machine
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureHealingMachineBodyID);
		modelHealingMachine.render(modelMatrixHealingMachine);
		glBindTexture(GL_TEXTURE_2D, 0);

		/*******************************************
		 * Skybox
		 *******************************************/
		GLint oldCullFaceMode;
		GLint oldDepthFuncMode;
		// deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
		glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
		glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
		shaderSkybox.setFloat("skybox", 0);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE0);
		skyboxSphere.render();
		glCullFace(oldCullFaceMode);
		glDepthFunc(oldDepthFuncMode);

		//Darth Vader Keyframes Guardado
		if(record && modelSelected == 1){
			matrixDartJoints.push_back(rotDartHead);
			matrixDartJoints.push_back(rotDartLeftArm);
			matrixDartJoints.push_back(rotDartLeftHand);
			matrixDartJoints.push_back(rotDartRightArm);
			matrixDartJoints.push_back(rotDartRightHand);
			matrixDartJoints.push_back(rotDartLeftLeg);
			matrixDartJoints.push_back(rotDartRightLeg);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixDartJoints);
			}
		}
		else if(keyFramesDartJoints.size() > 0){ //Para reproducir el frame de animación
			interpolationDartJoints = numPasosDartJoints / (float) maxNumPasosDartJoints;
			numPasosDartJoints++;
			if(interpolationDartJoints > 1.0){
				//Llegamos al punto objetivo.
				interpolationDartJoints = 0.0;
				numPasosDartJoints = 0;
				indexFrameDartJoints = indexFrameDartJointsNext;
				indexFrameDartJointsNext++;
			}
		
			if(indexFrameDartJointsNext > keyFramesDartJoints.size()-1)
				indexFrameDartJointsNext = 0;
			
			rotDartHead = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 0, interpolationDartJoints);
			rotDartLeftArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 1, interpolationDartJoints);
			rotDartLeftHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 2, interpolationDartJoints);
			rotDartRightArm = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 3, interpolationDartJoints);
			rotDartRightHand = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 4, interpolationDartJoints);
			rotDartLeftLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 5, interpolationDartJoints);
			rotDartRightLeg = interpolate(keyFramesDartJoints, indexFrameDartJoints, indexFrameDartJointsNext, 6, interpolationDartJoints);
		}
		if(record && modelSelected == 2){
			matrixDart.push_back(modelMatrixDart);
			if(saveFrame){
				saveFrame=false;
				appendFrame(myfile,matrixDart);
			}
		} else if(keyFramesDart.size() > 0){
			interpolationDart = numPasosDart / (float) maxNumPasosDart;
			numPasosDart++;
			if(interpolationDart > 1.0){
				numPasosDart = 0;
				interpolationDart = 0;
				indexFrameDart = indexFrameDartNext;
				indexFrameDartNext++;
			}
			if(indexFrameDartNext > keyFramesDart.size()-1)
				indexFrameDartNext=0;
			modelMatrixDart = interpolate(keyFramesDart, indexFrameDart, indexFrameDartNext, 0, interpolationDart);
			
		}

		//Buzz Keyframes Guardado
		if(record && modelSelected == 3){
			matrixBuzzJoints.push_back(rotBuzzHead);			
			matrixBuzzJoints.push_back(rotBuzzLeftArm);
			matrixBuzzJoints.push_back(rotBuzzLeftHand);
			matrixBuzzJoints.push_back(rotBuzzRightArm);
			matrixBuzzJoints.push_back(rotBuzzRightHand);
			matrixBuzzJoints.push_back(rotBuzzLeftLeg);
			matrixBuzzJoints.push_back(rotBuzzRightLeg);
			matrixBuzzJoints.push_back(rotBuzzBody);
			matrixBuzzJoints.push_back(rotBuzzHip);
			matrixBuzzJoints.push_back(rotBuzzLeftForearm);
			matrixBuzzJoints.push_back(rotBuzzLeftCalf);
			matrixBuzzJoints.push_back(rotBuzzLeftFoot);
			matrixBuzzJoints.push_back(rotBuzzLeftWing1);
			matrixBuzzJoints.push_back(rotBuzzLeftWing2);
			matrixBuzzJoints.push_back(rotBuzzRightForearm);
			matrixBuzzJoints.push_back(rotBuzzRightCalf);
			matrixBuzzJoints.push_back(rotBuzzRightFoot);
			matrixBuzzJoints.push_back(rotBuzzRightWing1);
			matrixBuzzJoints.push_back(rotBuzzRightWing2);
			if(saveFrame){
				saveFrame = false;
				appendFrame(myfile, matrixBuzzJoints);
			}
		}
		else if(keyFramesBuzzJoints.size() > 0){ //Para reproducir el frame de animación
			interpolationBuzzJoints = numPasosBuzzJoints / (float) maxNumPasosBuzzJoints;
			numPasosBuzzJoints++;
			if(interpolationBuzzJoints > 1.0){
				//Llegamos al punto objetivo.
				interpolationBuzzJoints = 0.0;
				numPasosBuzzJoints = 0;
				indexFrameBuzzJoints = indexFrameBuzzJointsNext;
				indexFrameBuzzNext++;
			}
		
			if(indexFrameBuzzJointsNext > keyFramesBuzzJoints.size()-1)
				indexFrameBuzzJointsNext = 0;
			
			rotBuzzHead = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 0, interpolationBuzzJoints);
			rotBuzzLeftArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 1, interpolationBuzzJoints);
			rotBuzzLeftHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 2, interpolationBuzzJoints);
			rotBuzzRightArm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 3, interpolationBuzzJoints);
			rotBuzzRightHand = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 4, interpolationBuzzJoints);
			rotBuzzLeftLeg = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 5, interpolationBuzzJoints);
			rotBuzzRightLeg = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 6, interpolationBuzzJoints);
			rotBuzzBody = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 7, interpolationBuzzJoints);
			rotBuzzHip = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 8, interpolationBuzzJoints);
			rotBuzzLeftForearm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 9, interpolationBuzzJoints);
			rotBuzzLeftCalf = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 10, interpolationBuzzJoints);
			rotBuzzLeftFoot = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 11, interpolationBuzzJoints);
			rotBuzzLeftWing1 = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 12, interpolationBuzzJoints);
			rotBuzzLeftWing2 = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 13, interpolationBuzzJoints);
			rotBuzzRightForearm = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 14, interpolationBuzzJoints);
			rotBuzzRightCalf = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 15, interpolationBuzzJoints);
			rotBuzzRightFoot = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 16, interpolationBuzzJoints);
			rotBuzzRightWing1 = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 17, interpolationBuzzJoints);
			rotBuzzRightWing2 = interpolate(keyFramesBuzzJoints, indexFrameBuzzJoints, indexFrameBuzzJointsNext, 18, interpolationBuzzJoints);
		}
		if(record && modelSelected == 4){
			matrixBuzz.push_back(modelMatrixBuzz);
			if(saveFrame){
				saveFrame=false;
				appendFrame(myfile,matrixBuzz);
			}
		} else if(keyFramesBuzz.size() > 0){
			interpolationBuzz = numPasosBuzz / (float) maxNumPasosBuzz;
			numPasosBuzz++;
			if(interpolationBuzz > 1.0){
				numPasosBuzz = 0;
				interpolationBuzz = 0;
				indexFrameBuzz = indexFrameBuzzNext;
				indexFrameBuzzNext++;
			}
			if(indexFrameBuzzNext > keyFramesBuzz.size()-1)
				indexFrameBuzzNext=0;
			modelMatrixBuzz = interpolate(keyFramesBuzz, indexFrameBuzz, indexFrameBuzzNext, 0, interpolationBuzz);
			
		}

		/****************Maquinas de estado de eclipse*************/
		switch(state){
			case 0:
				if(numberAdvance == 0)
					maxAdvance = 65.0;
				if(numberAdvance == 1)
					maxAdvance = 49.0;
				if(numberAdvance == 2)
					maxAdvance = 44.5;
				if(numberAdvance == 3)
					maxAdvance = 49.0;
				if(numberAdvance == 4)
					maxAdvance = 44.5;
				state = 1;
				break;
			case 1:
				modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, avance));
				advanceCount += avance;
				rotWheelsX += 0.05f;
				rotWheelsY -= 0.02f;
				if(rotWheelsY < 0.0f){
					rotWheelsY = 0.0f;
				}
				if(advanceCount > maxAdvance){
					advanceCount = 0;
					numberAdvance++;
					state = 2;
				}
				break;
			case 2:
				modelMatrixEclipse = glm::translate(modelMatrixEclipse, glm::vec3(0.0f, 0.0f, 0.025f));
				modelMatrixEclipse = glm::rotate(modelMatrixEclipse, glm::radians(giroEclipse), glm::vec3(0.0f,1.0f,0.0f));
				rotCount += giroEclipse;
				rotWheelsX += 0.005f;
				rotWheelsY += 0.02f;
				if(rotWheelsY >= 0.25f){
					rotWheelsY = 0.25f;
				}
				if(rotCount >= 90.0f){
					state = 0;
					rotCount = 0;
					if(numberAdvance > 4)
						numberAdvance = 1;
				}
				break;
			default:
				break;
		}

		/****************Maquinas de estado de lambo*************/
		switch(stateLambo){
			case 0:
				if(numberAdvanceLambo == 0)
					maxAdvanceLambo = 5.0;
				if(numberAdvanceLambo == 1)
					maxAdvanceLambo = 40.0;
				if(numberAdvanceLambo == 2)
					maxAdvanceLambo = 35.5;
				if(numberAdvanceLambo == 3)
					maxAdvanceLambo = 40.0;
				if(numberAdvanceLambo == 4)
					maxAdvanceLambo = 30.5;
				if(numberAdvanceLambo > 4){
					break;
				}
				stateLambo = 1;
				break;
			case 1:
				modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, avance));
				advanceCountLambo += avance;
				rotWheelsLamboX += 0.05f;
				rotWheelsLamboY -= 0.02f;
				if(rotWheelsLamboY < 0.0f){
					rotWheelsLamboY = 0.0f;
				}
				if(advanceCountLambo > maxAdvanceLambo){
					advanceCountLambo = 0;
					numberAdvanceLambo++;
					stateLambo = 2;
				}
				break;
			case 2:
				if(numberAdvanceLambo > 4){
					stateLambo = 0;
					stateDoor = 0;
					break;
				}
				modelMatrixLambo = glm::translate(modelMatrixLambo, glm::vec3(0.0f, 0.0f, 0.025f));
				modelMatrixLambo = glm::rotate(modelMatrixLambo, glm::radians(-giroEclipse), glm::vec3(0.0f,1.0f,0.0f));
				rotCountLambo += giroEclipse;
				rotWheelsLamboX += 0.005f;
				rotWheelsLamboY += 0.02f;
				if(rotWheelsLamboY >= 0.25f){
					rotWheelsLamboY = 0.25f;
				}
				if(rotCountLambo >= 90.0f){
					stateLambo = 0;
					rotCountLambo = 0;
				}
				break;
			default:
				break;
		}
		
		/****************Maquinas de estado de helicoptero*************/

		switch(stateHeli){
			case 0:
				if(numberAdvanceHeli == 0)
					maxAdvanceHeli = 4.0;
				if(numberAdvanceHeli == 1){
					maxAdvanceHeli = 9.0;
					stateHeli = 2;
					break;
				}
					
				rotHelHelY += 0.5f;
				stateHeli = 1;
				break;
			case 1:
				rotHelHelX= 0.0f;
				if(rotCountHeli < 30.0f){
					rotHelHelX = 0.1f;
					rotCountHeli += rotHelHelX;
				}
				modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(0.0f, 0.0f, avanceHeli));
				modelMatrixHeli = glm::rotate(modelMatrixHeli, glm::radians(rotHelHelX), glm::vec3(1.0f,0.0f,0.0f));
				advanceCountHeli += avanceHeli;
				rotHelHelY += 0.5;
				if(advanceCountHeli > maxAdvanceHeli){
					advanceCountHeli = 0;
					numberAdvanceHeli++;
					stateHeli = 0;
				}
				break;
			case 2:
				rotHelHelX= 0.0f;
				if(rotCountHeli > 0.0f){
					rotHelHelX = -0.1f;
					rotCountHeli += rotHelHelX;
				}
				modelMatrixHeli = glm::translate(modelMatrixHeli, glm::vec3(0.0f, -avanceHeli, 0.0f));
				modelMatrixHeli = glm::rotate(modelMatrixHeli, glm::radians(rotHelHelX), glm::vec3(1.0f,0.0f,0.0f));
				advanceCountHeli += avanceHeli;
				rotHelHelY += 0.5;
				if(advanceCountHeli > maxAdvanceHeli){
					advanceCountHeli = 0;
					numberAdvanceHeli++;
					stateHeli = 3;
					rotCountHeli = 0.5f;
				}
				break;
			case 3:
				if(rotCountHeli > 0.0f){
					rotHelHelY += rotCountHeli;
					rotCountHeli -= 0.005f;
				} else {
					rotHelHelY += 0.0f; 
				}
				break;
			default:
				break;
		}

		/*********Maquina de estados de puerta del lambo********/
		switch (stateDoor){
			case 0:
				dorRotCount += 0.6f;
				if(dorRotCount > 75.0f)
					stateDoor = 1;
				break;
			default:
				break;
		}
		
		

		glfwSwapBuffers(window);
	}
}

int main(int argc, char **argv) {
	init(800, 700, "Window GLFW", false);
	applicationLoop();
	destroy();
	return 1;
}
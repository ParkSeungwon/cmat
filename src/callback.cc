#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<valarray>
#include<fstream>
#include<thread>
#include"globj.h"
#define STEP 0.05
using namespace std;

static mat4 m;
mat4 KeyBindMatrix{m.E()};
float thz, dest_x, dest_y, dest_z;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{// && action == GLFW_PRESS) 
	switch(key) {
	case GLFW_KEY_Q: thz += STEP; break;
	case GLFW_KEY_E: thz -= STEP; break;
	case GLFW_KEY_Z: KeyBindMatrix = m.gltranslate(0, 0, STEP) * KeyBindMatrix; break;
	case GLFW_KEY_X: KeyBindMatrix = m.gltranslate(0, 0, -STEP) * KeyBindMatrix; break;
	case GLFW_KEY_UP: KeyBindMatrix = m.gltranslate(0,STEP,0) * KeyBindMatrix; break;
	case GLFW_KEY_LEFT: KeyBindMatrix = m.gltranslate(-STEP,0,0)* KeyBindMatrix; break;
	case GLFW_KEY_DOWN: KeyBindMatrix = m.gltranslate(0,-STEP,0)* KeyBindMatrix; break;
	case GLFW_KEY_RIGHT: KeyBindMatrix = m.gltranslate(STEP,0,0)* KeyBindMatrix; break;
	case GLFW_KEY_A: KeyBindMatrix = m.glrotateY(-STEP) * KeyBindMatrix; break;
	case GLFW_KEY_D: KeyBindMatrix = m.glrotateY(STEP) * KeyBindMatrix; break;
	case GLFW_KEY_W: KeyBindMatrix = m.glrotateX(STEP) * KeyBindMatrix; break;
	case GLFW_KEY_S: KeyBindMatrix = m.glrotateX(-STEP) * KeyBindMatrix; break;
	case GLFW_KEY_SPACE: KeyBindMatrix.E(); break;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {//GLFW_RELEASE
        glfwGetCursorPos(window, &x, &y);
		cout << '(' << x / 4 << ',' << y / 4 << ')' << flush;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
//	KeyBindMatrix = m.glscale(1+yoffset, 1+yoffset, 1+yoffset) * KeyBindMatrix;
}

void cursor_pos_callback(GLFWwindow* window, double to_x, double to_y) {
	dest_x = to_x / 512 - 1;
	dest_y = -(to_y / 512 - 1);
	if(dest_x > 0.8) dest_x = 0.8;
	else if(dest_x < -0.8) dest_x = -0.8;
	if(dest_y > 0.8) dest_y = 0.8;
	else if(dest_y < -0.8) dest_y = -0.8;
}

bool glinit(GLFWwindow* window) 
{
	if(!window) {
		glfwTerminate();
		return false;
	}
	// callbacks here
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(0, 0, 0, 0); // black background
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
//	glDisable(GL_CULL_FACE);
//	glPolygonMode(GL_BACK, GL_FILL);
//	glDepthMask(GL_FALSE);
//	glEnable(GL_STENCIL_TEST);
//	glStencilFunc(GL_ALWAYS, 1, 0xff);
//	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_COLOR_MATERIAL);
//	glEnable(GL_TEXTURE_CUBE_MAP);
//	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_PROGRAM_POINT_SIZE);
//	glCullFace(GL_FRONT_AND_BACK);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		cerr << "Failed to initialize GLEW" << endl;
		glfwTerminate();
		return false;
	}
	if (glewIsSupported("GL_VERSION_3_3")) printf("Ready for OpenGL 3.3\n");
    else {
        printf("OpenGL 3.3 not supported\n");
        exit(1);
    }
	cout << "shading language version : " << 
			glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
	cout << glGetString( GL_VENDOR ) << endl;
	cout << glGetString( GL_RENDERER ) << endl;
	cout << glGetString( GL_VERSION   ) << endl;
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	return true;
}

unsigned make_shader_program(string v_shader, string f_shader)
{
	auto* vp = v_shader.data();
	auto* fp = f_shader.data();
	char** vertex_shader = &vp;
	char** fragment_shader = &fp;

	unsigned vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vertex_shader, NULL);
	glCompileShader(vs);
	int status;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) cerr << "compile error in vertex shader" << endl;
	
	unsigned fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fragment_shader, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) cerr << "compile error in fragment shader" << endl;

	unsigned shader_program = glCreateProgram();
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);
	glLinkProgram(shader_program);

	//linking error message
	int linked = 0;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &linked);
	if(!linked)  {
		int infolen = 0;
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &infolen);
		if(infolen > 1) {
			char* infoLog = (char*)malloc(sizeof(char) * infolen);
			glGetProgramInfoLog(shader_program, infolen, NULL, infoLog);
			cout << "error linking program\n" << infoLog << endl;
			free(infoLog);
		}
		glDeleteProgram(shader_program);
		return 0;
	}
	return shader_program;
}

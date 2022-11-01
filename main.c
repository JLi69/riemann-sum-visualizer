#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>

#define MAX_SHADER_SIZE 8192

enum
{
	LEFT,
	RIGHT,
	MID,
	TRAPEZOID,
	HIDE
};

const float rectangle[] = 
{
	1.0f, 1.0f,
	-1.0f, 1.0f,
	-1.0f, -1.0f,

	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f
};

unsigned int program;
//Uniforms
int winSizeLoc;

int intervalLoc;
#define MIN_INTERVAL 0.01f
float interval = 0.5f;
#define CHANGE_SPEED 0.001f
float intervalChange = 0.0f;

int scaleLoc;
float scale = 40.0f;
#define SCALE_UP 1.1
#define MAX_SCALE 400.0f
#define MIN_SCALE 10.0f

int minLoc, maxLoc;
float minVal = -20.0f, maxVal = 20.0f;
float minValChange = 0.0f, maxValChange = 0.0f;
#define SPEED 0.3f

int approximationMethodLoc;
int method = LEFT;

int centerLoc;
float centerX = 0.0f, centerY = 0.0f;
float changeX = 0.0f, changeY = 0.0f;
#define MOVE_SPEED 15.0f

void setupProgram()
{
	program = glCreateProgram();	

	unsigned int vertShaderId = glCreateShader(GL_VERTEX_SHADER),
				 fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	//Shader source code
	char vertSrc[MAX_SHADER_SIZE + 1];
	char fragSrc[MAX_SHADER_SIZE + 1];
	for(int i = 0; i < MAX_SHADER_SIZE + 1; i++) vertSrc[i] = '\0';
	for(int i = 0; i < MAX_SHADER_SIZE + 1; i++) fragSrc[i] = '\0';
	//Read the shaders
	FILE* vertFile = fopen("integral-vert.glsl", "r");
	FILE* fragFile = fopen("integral-frag.glsl", "r");

	int sz = 0;
	char ch;
	//Read the vertex shader file
	while(((ch = fgetc(vertFile)) != EOF) && sz < MAX_SHADER_SIZE) vertSrc[sz++] = ch;	
	sz = 0;
	//Read the fragment shader file
	while(((ch = fgetc(fragFile)) != EOF) && sz < MAX_SHADER_SIZE) fragSrc[sz++] = ch;

	//Compile the shaders
	const char* vertBegin = vertSrc;
	const char* fragBegin = fragSrc;
	glShaderSource(vertShaderId, 1, &vertBegin, NULL);
	glShaderSource(fragShaderId, 1, &fragBegin, NULL);
	glCompileShader(vertShaderId);
	//Check for compile errors
	int compiled;
	glGetShaderiv(vertShaderId, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		char log[4097];
		int len;
		glGetShaderInfoLog(vertShaderId, 4096, &len, log);
		log[len] = '\0';
		fprintf(stderr, "Vertex shader failed to compile: %s\n", log);
	}

	glCompileShader(fragShaderId);
	//Check for compile errors
	glGetShaderiv(fragShaderId, GL_COMPILE_STATUS, &compiled);
	if(!compiled)
	{
		char log[4097];
		int len;
		glGetShaderInfoLog(fragShaderId, 4096, &len, log);
		log[len] = '\0';
		fprintf(stderr, "Fragment shader failed to compile: %s\n", log);
	}	

	glAttachShader(program, vertShaderId);
	glAttachShader(program, fragShaderId);
	glLinkProgram(program);
	glValidateProgram(program);
	
	//Clean up
	glDetachShader(program, vertShaderId);
	glDetachShader(program, fragShaderId);
	glDeleteShader(vertShaderId);
	glDeleteShader(fragShaderId);
}

//Run this function whenever the window is resized
void handleWinResize(GLFWwindow *win, int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
	glUniform2f(winSizeLoc, (float)newWidth, (float)newHeight);
}

//handle key input
void handleKeyInput(GLFWwindow *win, int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_I && action == GLFW_PRESS)
		intervalChange = CHANGE_SPEED;
	else if(key == GLFW_KEY_D && action == GLFW_PRESS)
		intervalChange = -CHANGE_SPEED;

	//CTRL + increases scale
	if(key == GLFW_KEY_EQUAL && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS)
	{
		scale *= SCALE_UP;
		if(scale >= MAX_SCALE)
			scale = MAX_SCALE;
		glUniform1f(scaleLoc, scale);	
	}
	//CTRL - decreases scale
	else if(key == GLFW_KEY_MINUS && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS)
	{	
		scale /= SCALE_UP;
		if(scale <= MIN_SCALE)
			scale = MIN_SCALE;
		glUniform1f(scaleLoc, scale);	
	}

	if(key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		maxValChange = SPEED;
	}
	else if(key == GLFW_KEY_K && action == GLFW_PRESS)
	{	
		maxValChange = -SPEED;
	}
	else if(key == GLFW_KEY_J && action == GLFW_PRESS)
	{	
		minValChange = SPEED;
	}
	else if(key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		minValChange = -SPEED;
	}

	//Change method
	switch(key)
	{
	case GLFW_KEY_1:
		method = LEFT;
		glUniform1i(approximationMethodLoc, method);
		break;
	case GLFW_KEY_2:
		method = RIGHT;
		glUniform1i(approximationMethodLoc, method);
		break;
	case GLFW_KEY_3:
		method = MID;
		glUniform1i(approximationMethodLoc, method);
		break;
	case GLFW_KEY_4:
		method = TRAPEZOID;
		glUniform1i(approximationMethodLoc, method);
		break;
	case GLFW_KEY_5:
		method = HIDE;	
		glUniform1i(approximationMethodLoc, method);
		break;
	}

	//Move around with the arrow keys
	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
		changeY = MOVE_SPEED;
	else if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		changeY = -MOVE_SPEED;

	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		changeX = -MOVE_SPEED;
	else if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		changeX = MOVE_SPEED;

	if(action == GLFW_RELEASE)
	{
		intervalChange = 0.0f;	
		maxValChange = 0.0f;
		minValChange = 0.0f;
		changeX = 0.0f;
		changeY = 0.0f;
	}
}

int main(void)
{
	//Init glfw and glad
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(800, 800, "Integral Visualization", NULL, NULL);
	glfwMakeContextCurrent(win);
	glfwSetWindowSizeCallback(win, handleWinResize);		
	glfwSetKeyCallback(win, handleKeyInput);	
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//Set up rectangle
	unsigned int rectBuff;
	glGenBuffers(1, &rectBuff);
	glBindBuffer(GL_ARRAY_BUFFER, rectBuff);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, NULL);	
	glEnableVertexAttribArray(0);

	setupProgram();
	glUseProgram(program);
	winSizeLoc = glGetUniformLocation(program, "uWinSize");
	intervalLoc = glGetUniformLocation(program, "uInterval");
	scaleLoc = glGetUniformLocation(program, "uScale");
	minLoc = glGetUniformLocation(program, "uMin");
	maxLoc = glGetUniformLocation(program, "uMax");
	approximationMethodLoc = glGetUniformLocation(program, "uApproximationMethod");
	centerLoc = glGetUniformLocation(program, "uCenter");

	//Main loop
	while(!glfwWindowShouldClose(win))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Output any errors
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR)
			fprintf(stderr, "OpenGL Error: %d\n", err);

		glUniform1f(program, (float)interval);

		if(interval >= MIN_INTERVAL)
			interval += intervalChange;
		else if(interval < MIN_INTERVAL)
			interval = 0.01f;

		minVal += minValChange;
		maxVal += maxValChange;

		centerX += changeX / scale;
		centerY += changeY / scale;
		glUniform2f(centerLoc, centerX, centerY);
		
		if(maxVal < minVal + 1.0f)
		{
			minValChange = 0.0f;
			maxValChange = 0.0f;
		}

		glUniform1f(minLoc, minVal);
		glUniform1f(maxLoc, maxVal);

		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
}

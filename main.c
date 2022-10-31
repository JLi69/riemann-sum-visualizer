#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <stdio.h>

#define MAX_SHADER_SIZE 8192

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
int winSizeLoc;
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
}

int main(void)
{
	//Init glfw and glad
	glfwInit();
	GLFWwindow* win = glfwCreateWindow(800, 800, "Integral Visualization", NULL, NULL);
	glfwMakeContextCurrent(win);
	glfwSetWindowSizeCallback(win, handleWinResize);	
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

	//Main loop
	while(!glfwWindowShouldClose(win))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Output any errors
		GLenum err;
		while((err = glGetError()) != GL_NO_ERROR)
			fprintf(stderr, "OpenGL Error: %d\n", err);

		//Constantly update the window size
		int winW, winH;
		glfwGetWindowSize(win, &winW, &winH);
		glUniform2f(winSizeLoc, (float)winW, (float)winH);
	
		glfwSwapBuffers(win);
		glfwPollEvents();
	}

	glfwTerminate();
}

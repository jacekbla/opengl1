#include "ShaderProgram.h"



ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile)
{
	setShaders(vertexFile, fragmentFile, programHandle, vertexShaderHandle, fragmentShaderHandle);

	bindAttributes();
	getAllUniformLocations();
}


ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::start()
{
	glFuncs::ref().glUseProgram(programHandle);
}

void ShaderProgram::stop()
{
	glFuncs::ref().glUseProgram(0);
}

void ShaderProgram::cleanUp()
{
	stop();
	glFuncs::ref().glDetachShader(programHandle, vertexShaderHandle);
	glFuncs::ref().glDetachShader(programHandle, fragmentShaderHandle);
	glFuncs::ref().glDeleteShader(vertexShaderHandle);
	glFuncs::ref().glDeleteShader(fragmentShaderHandle);
	glFuncs::ref().glDeleteProgram(programHandle);
}

char* ShaderProgram::readShader(const char * aShaderFile)
{
	FILE* filePointer = fopen(aShaderFile, "rb");
	char* content = NULL;
	long numVal = 0;

	fseek(filePointer, 0L, SEEK_END);
	numVal = ftell(filePointer);
	fseek(filePointer, 0L, SEEK_SET);
	content = (char*)malloc((numVal + 1) * sizeof(char));
	fread(content, 1, numVal, filePointer);
	content[numVal] = '\0';
	fclose(filePointer);
	return content;
}

void ShaderProgram::setShaders(const char* p_vertex, const char* p_pixel, int &programHandle, int &vertexShaderHandle, int &fragmentShaderHandle)
{
	GLint status = 0;

	programHandle = glFuncs::ref().glCreateProgram();
	vertexShaderHandle = glFuncs::ref().glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderHandle = glFuncs::ref().glCreateShader(GL_FRAGMENT_SHADER);

	setShader(p_vertex, programHandle, vertexShaderHandle);
	setShader(p_pixel, programHandle, fragmentShaderHandle);

	glFuncs::ref().glLinkProgram(programHandle);
	glFuncs::ref().glGetObjectParameterivARB(programHandle, GL_OBJECT_LINK_STATUS_ARB, &status);
	if (!status) {
		const int maxInfoLogSize = 2048;
		GLchar infoLog[maxInfoLogSize];
		glFuncs::ref().glGetInfoLogARB(programHandle, maxInfoLogSize, NULL, infoLog);
		std::cout << infoLog;
	}

	//locMVP = glFuncs::ref().glGetUniformLocation(programHandle, "MVP");
}

void ShaderProgram::setShader(const char* p_file, int &programHandle, int &shaderHandle)
{
	GLint status = 0;

	char* shader = readShader(p_file);

	glFuncs::ref().glShaderSource(shaderHandle, 1, (const char**)&shader, NULL);

	glFuncs::ref().glCompileShader(shaderHandle);

	glFuncs::ref().glGetObjectParameterivARB(shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &status);
	if (!status)
	{
		const int maxInfoLogSize = 2048;
		GLchar infoLog[maxInfoLogSize];
		glFuncs::ref().glGetInfoLogARB(shaderHandle, maxInfoLogSize, NULL, infoLog);
		std::cout << infoLog;
	}

	glFuncs::ref().glAttachShader(programHandle, shaderHandle);
}

void ShaderProgram::bindAttributes() {}

void ShaderProgram::bindAttribute(int attribute, const char* variableName)
{
	glFuncs::ref().glBindAttribLocation(programHandle, attribute, variableName);
}

void ShaderProgram::getAllUniformLocations() {}

int ShaderProgram::getUniformLocation(const char * p_uniformName)
{
	return glFuncs::ref().glGetUniformLocation(programHandle, p_uniformName);
}

void ShaderProgram::loadInt(int p_location, int p_value)
{
	glFuncs::ref().glUniform1i(p_location, p_value);
}

void ShaderProgram::loadFloat(int p_location, float &p_value)
{
	glFuncs::ref().glUniform1f(p_location, p_value);
}

void ShaderProgram::loadVector(int p_location, glm::vec3 &p_value)
{
	/*std::vector<float> tmp;
	tmp.push_back(p_value.x);
	tmp.push_back(p_value.y);
	tmp.push_back(p_value.z);
	glFuncs::ref().glUniform3fv(p_location, 1, &tmp[0]);*/
	glFuncs::ref().glUniform3f(p_location, p_value.x, p_value.y, p_value.z);
}

void ShaderProgram::loadVector(int p_location, glm::vec4 & p_value)
{
	glFuncs::ref().glUniform4f(p_location, p_value.x, p_value.y, p_value.z, p_value.w);
}

void ShaderProgram::loadBoolean(int p_location, bool &p_value)
{
	glFuncs::ref().glUniform1f(p_location, p_value ? 1.0f : 0.0f);
}

void ShaderProgram::loadMatrix(int p_location, glm::mat4 &p_matrix)
{
	glFuncs::ref().glUniformMatrix4fv(p_location, 1, GL_FALSE, &p_matrix[0][0]);
}

#include "tess.h"

Tess::Tess(Loader p_loader, glm::mat4 p_projMatrix)
{
	_FILE_VS = "1_vs.GLSL";
	_FILE_TC = "2_tc.GLSL";
	_FILE_TE = "3_te.GLSL";
	_FILE_GS = "4_gs.GLSL";
	_FILE_FS = "5_fs.GLSL";

	_square_x = 0.0f;
	_square_z = -7.0f;
	_square_y = -1.0f;
	_square_scale = 5.0f;

	createRawModel(p_loader);

	createProgram();

	glFuncs::ref().glBindAttribLocation(h_program, 0, "position");

	_location_projMatrix = glFuncs::ref().glGetUniformLocation(h_program, "projMatrix");
	_location_viewMatrix = glFuncs::ref().glGetUniformLocation(h_program, "viewMatrix");
	_location_modelMatrix = glFuncs::ref().glGetUniformLocation(h_program, "modelMatrix");

	enableShader();

	glm::mat4 modelMatrix = Maths::createTransformMatrix(glm::fvec3(_square_x, _square_y, _square_z), 0.0f, 0.0f, 0.0f, _square_scale);
	loadMatrix(_location_modelMatrix, modelMatrix);

	loadMatrix(_location_projMatrix, p_projMatrix);

	disableShader();
	// ? bindAttributes();
	// ? getAllUniformLocations();

}

Tess::~Tess()
{

}

void Tess::enableShader()
{
	glFuncs::ref().glUseProgram(h_program);
}

void Tess::disableShader()
{
	glFuncs::ref().glUseProgram(0);
}

void Tess::cleanUp()
{
	disableShader();

	glFuncs::ref().glDetachShader(h_program, h_VS);
	glFuncs::ref().glDetachShader(h_program, h_FS);
	glFuncs::ref().glDetachShader(h_program, h_TC);
	glFuncs::ref().glDetachShader(h_program, h_TE);
	glFuncs::ref().glDetachShader(h_program, h_GS);

	glFuncs::ref().glDeleteShader(h_VS);
	glFuncs::ref().glDeleteShader(h_FS);
	glFuncs::ref().glDeleteShader(h_TC);
	glFuncs::ref().glDeleteShader(h_TE);
	glFuncs::ref().glDeleteShader(h_GS);

	glFuncs::ref().glDeleteProgram(h_program);
}

void Tess::render(Camera& p_camera)
{
	enableShader();

	glFuncs::ref().glBindVertexArray(_square_rawModel.getVaoID());
	glFuncs::ref().glEnableVertexAttribArray(0);

	glm::mat4 viewMatrix = Maths::createViewMatrix(p_camera);
	loadMatrix(_location_viewMatrix, viewMatrix);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glDrawArrays(GL_TRIANGLES, 0, _square_rawModel.getVertexCount());
	glDrawArrays(GL_PATCHES, 0, _square_rawModel.getVertexCount());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glFuncs::ref().glDisableVertexAttribArray(0);
	glFuncs::ref().glBindVertexArray(0);

	disableShader();
}

void Tess::createRawModel(Loader p_loader)
{
	std::vector<float> vertices = { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
	_square_rawModel = p_loader.loadToVAO(vertices, 2);
}

char * Tess::readShaderFile(const char * p_shaderFile)
{
	FILE* filePointer = fopen(p_shaderFile, "rb");
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

void Tess::addShaderToProgram(const char * p_file, int & p_shaderHandle)
{
	GLint status = 0;
	char* shader = readShaderFile(p_file);
	glFuncs::ref().glShaderSource(p_shaderHandle, 1, (const char**)&shader, NULL);
	glFuncs::ref().glCompileShader(p_shaderHandle);
	glFuncs::ref().glGetObjectParameterivARB(p_shaderHandle, GL_OBJECT_COMPILE_STATUS_ARB, &status);
	if (!status)
	{
		const int maxInfoLogSize = 2048;
		GLchar infoLog[maxInfoLogSize];
		glFuncs::ref().glGetInfoLogARB(p_shaderHandle, maxInfoLogSize, NULL, infoLog);
		std::cout << infoLog;
	}
	glFuncs::ref().glAttachShader(h_program, p_shaderHandle);
}

void Tess::createProgram()
{
	GLint status = 0;
	h_program = glFuncs::ref().glCreateProgram();

	h_VS = glFuncs::ref().glCreateShader(GL_VERTEX_SHADER);
	h_FS = glFuncs::ref().glCreateShader(GL_FRAGMENT_SHADER);
	h_TC = glFuncs::ref().glCreateShader(GL_TESS_CONTROL_SHADER);
	h_TE = glFuncs::ref().glCreateShader(GL_TESS_EVALUATION_SHADER);
	h_GS = glFuncs::ref().glCreateShader(GL_GEOMETRY_SHADER);

	addShaderToProgram(_FILE_VS, h_VS);
	addShaderToProgram(_FILE_FS, h_FS);
	addShaderToProgram(_FILE_TC, h_TC);
	addShaderToProgram(_FILE_TE, h_TE);
	addShaderToProgram(_FILE_GS, h_GS);

	glFuncs::ref().glLinkProgram(h_program);
	glFuncs::ref().glGetObjectParameterivARB(h_program, GL_OBJECT_LINK_STATUS_ARB, &status);
	if (!status) {
		const int maxInfoLogSize = 2048;
		GLchar infoLog[maxInfoLogSize];
		glFuncs::ref().glGetInfoLogARB(h_program, maxInfoLogSize, NULL, infoLog);
		std::cout << infoLog;
	}
}

void Tess::loadMatrix(int p_location, glm::mat4 &p_matrix)
{
	glFuncs::ref().glUniformMatrix4fv(p_location, 1, GL_FALSE, &p_matrix[0][0]);
}

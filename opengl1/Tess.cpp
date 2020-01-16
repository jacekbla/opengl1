#include "tess.h"

const char* Tess::_DUDV_MAP = "res/dudv/dudv_water_map4.bmp";
const float Tess::_WAVE_SPEED = 0.01f;
const float Tess::_HEIGHT = 0.02f;

Tess::Tess(Loader p_loader, glm::mat4 p_projMatrix, WaterTile& p_quad, WaterFrameBuffers p_fbos)
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


	_water = p_quad;
	_fbos = p_fbos;

	_dudvTexture = p_loader.loadTexture(_DUDV_MAP);
	_moveFactor = 0.0f;

	createProgram();

	enableShader();

	glFuncs::ref().glBindAttribLocation(h_program, 0, "position");
	glFuncs::ref().glBindAttribLocation(h_program, 1, "indicators");

	_location_projMatrix = glFuncs::ref().glGetUniformLocation(h_program, "projMatrix");
	_location_viewMatrix = glFuncs::ref().glGetUniformLocation(h_program, "viewMatrix");
	_location_modelMatrix = glFuncs::ref().glGetUniformLocation(h_program, "modelMatrix");

	_location_reflectionTexture = glFuncs::ref().glGetUniformLocation(h_program, "reflectionTexture");
	_location_refractionTexture = glFuncs::ref().glGetUniformLocation(h_program, "refractionTexture");
	_location_dudvMap = glFuncs::ref().glGetUniformLocation(h_program, "dudvMap");
	_location_moveFactor = glFuncs::ref().glGetUniformLocation(h_program, "moveFactor");
	_location_cameraPosition = glFuncs::ref().glGetUniformLocation(h_program, "cameraPosition");
	_location_depthMap = glFuncs::ref().glGetUniformLocation(h_program, "depthMap");
	_location_height = glFuncs::ref().glGetUniformLocation(h_program, "height");
	_location_waveTime = glFuncs::ref().glGetUniformLocation(h_program, "waveTime");
	_location_lightPosition = glFuncs::ref().glGetUniformLocation(h_program, "lightPosition");
	_location_lightColour = glFuncs::ref().glGetUniformLocation(h_program, "lightColour");

	loadInt(_location_reflectionTexture, 0);
	loadInt(_location_refractionTexture, 1);
	loadInt(_location_dudvMap, 2);
	loadInt(_location_depthMap, 3);

	glm::mat4 modelMatrix = Maths::createTransformMatrix(glm::fvec3(_water.getX(), _water.getHeight(), _water.getZ()), 0.0f, 0.0f, 0.0f, WaterTile::TILE_SIZE);
	loadMatrix(_location_modelMatrix, modelMatrix);

	loadMatrix(_location_projMatrix, p_projMatrix);

	disableShader();
}

Tess::~Tess(){}

void Tess::render(Camera& p_camera, Light &p_light, bool seeTessEdges)
{
	enableShader();

	beforeRender(p_camera, p_light);

	if (seeTessEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	//glDrawArrays(GL_TRIANGLES, 0, _square_rawModel.getVertexCount());
	//glDrawArrays(GL_PATCHES, 0, _water.getVertexCount());
	//glDrawArrays(GL_TRIANGLES, 0, _water.getVertexCount());

	glDrawArrays(GL_PATCHES, 0, _water.getVertexCount());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	afterRender();

	disableShader();
}

void Tess::beforeRender(Camera & p_camera, Light & p_light)
{
	glFuncs::ref().glBindVertexArray(_water.getVaoID());
	glFuncs::ref().glEnableVertexAttribArray(0);
	glFuncs::ref().glEnableVertexAttribArray(1);

	_waveTime += _WAVE_SPEED;
	loadFloat(_location_waveTime, _waveTime);

	_moveFactor += _WAVE_SPEED * DisplayManager::getFrameTimeSeconds().count();
	_moveFactor = _moveFactor - floor(_moveFactor);
	loadFloat(_location_moveFactor, _moveFactor);

	loadFloat(_location_height, _HEIGHT);

	loadVec3(_location_lightPosition, p_light.getPostion());
	loadVec3(_location_lightColour, p_light.getColor());

	glm::mat4 viewMatrix = Maths::createViewMatrix(p_camera);
	loadMatrix(_location_viewMatrix, viewMatrix);

	glFuncs::ref().glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _fbos.getReflectionTexture());
	glFuncs::ref().glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _fbos.getRefractionTexture());
	glFuncs::ref().glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _dudvTexture);
	glFuncs::ref().glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _fbos.getRefractionDepthTexture());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Tess::afterRender()
{
	glDisable(GL_BLEND);
	glFuncs::ref().glDisableVertexAttribArray(0);
	glFuncs::ref().glDisableVertexAttribArray(1);
	glFuncs::ref().glBindVertexArray(0);


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

void Tess::loadVec3(int p_location, glm::vec3 &p_value)
{
	glFuncs::ref().glUniform3f(p_location, p_value.x, p_value.y, p_value.z);
}

void Tess::loadFloat(int p_location, float p_value)
{
	glFuncs::ref().glUniform1f(p_location, p_value);
}

void Tess::loadInt(int p_location, int p_value)
{
	glFuncs::ref().glUniform1i(p_location, p_value);
}

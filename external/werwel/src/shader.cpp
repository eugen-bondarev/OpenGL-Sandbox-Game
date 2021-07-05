#include "Shader.h"

namespace ww {

Shader::Shader(const std::string &vsCode, const std::string &fsCode, const std::vector<std::string>& uniforms)
{
	handle = glCreateProgram();

	vsHandle = CreateShader(vsCode, GL_VERTEX_SHADER);
	fsHandle = CreateShader(fsCode, GL_FRAGMENT_SHADER);

	Link();

	for (int i = 0; i < uniforms.size(); i++)
	{
		CreateUniform(uniforms[i]);
	}

	WERWEL_DEBUG_LOG_OUT("[Call] Shader constructor");
}

Shader::~Shader()
{
	Unbind();

	glDetachShader(handle, vsHandle);
	glDetachShader(handle, fsHandle);
	glDeleteShader(vsHandle);
	glDeleteShader(fsHandle);
	glDeleteProgram(handle);

	WERWEL_DEBUG_LOG_OUT("[Call] Shader destructor");
}

void Shader::Link() const
{
	glLinkProgram(handle);

	if (vsHandle != 0)
	{
		glDetachShader(handle, vsHandle);
	}

	if (fsHandle != 0)
	{
		glDetachShader(handle, fsHandle);
	}

	glValidateProgram(handle);
}

GLuint Shader::CreateShader(const std::string &shaderCode, GLuint shaderType)
{
	const unsigned int shaderID = glCreateShader(shaderType);

	const char *c_str = shaderCode.c_str();

	glShaderSource(shaderID, 1, &c_str, NULL);
	glCompileShader(shaderID);

	int status;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		int length;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
		Str log;
		log.resize(length);
		glGetShaderInfoLog(shaderID, length, &length, &log[0]);

		Str errorFunctionName = "--------[ "
								"Foo"
								" ]--------";
		Str separator;
		for (size_t i = 0; i < errorFunctionName.size(); i++)
			separator += "-";

		WERWEL_LOG_OUT(errorFunctionName);
		WERWEL_LOG_OUT("Error occured while compiling a shader: " << log);
		WERWEL_LOG_OUT(separator);

		return -1;
	}

	glAttachShader(handle, shaderID);

	return shaderID;
}

void Shader::Bind() const
{
	glUseProgram(handle);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::CreateUniform(const std::string &name)
{
	int location = glGetUniformLocation(handle, name.c_str());

	uniformLocations.insert(
		std::pair<std::string, int>(
			name.c_str(),
			location));
}

void Shader::SetFloat(const std::string &name, GLfloat value)
{
	glUniform1f(uniformLocations.at(name), value);
}

void Shader::SetInt(const std::string &name, GLint value)
{
	glUniform1i(uniformLocations.at(name), value);
}

void Shader::SetMat2x4(const std::string &name, float const *const matrix)
{
	glUniformMatrix2x4fv(uniformLocations.at(name), 1, GL_FALSE, matrix);
}

void Shader::SetMat4x4(const std::string &name, float const *const matrix)
{
	glUniformMatrix4fv(uniformLocations.at(name), 1, GL_FALSE, matrix);
}

void Shader::SetMat3x3(const std::string &name, float const *const matrix)
{
	glUniformMatrix3fv(uniformLocations.at(name), 1, GL_FALSE, matrix);
}

void Shader::SetVec2(const std::string &name, float const *const vec)
{
	glUniform2fv(uniformLocations.at(name), 1, vec);
}

void Shader::SetVec3(const std::string &name, float const *const vec)
{
	glUniform3fv(uniformLocations.at(name), 1, vec);
}

void Shader::SetVec4(const std::string &name, float const *const vec)
{
	glUniform4fv(uniformLocations.at(name), 1, vec);
}

void Shader::SetListMat4x4(const std::string &name, float const *const list, unsigned int size)
{
	glUniformMatrix4fv(uniformLocations.at(name), size, GL_FALSE, list);
}

}
#include "Shader.h"

Shader::~Shader()
{
    Unbind();
    glDetachShader(m_Handle, m_VsHandle);
    glDetachShader(m_Handle, m_FsHandle);
    glDeleteShader(m_VsHandle);
    glDeleteShader(m_FsHandle);
    glDeleteProgram(m_Handle);

    DEBUG_LOG_OUT("[Call] Shader destructor");
}

void Shader::Link() const
{
    glLinkProgram(m_Handle);

    if (m_VsHandle != 0)
    {
        glDetachShader(m_Handle, m_VsHandle);
    }

    if (m_FsHandle != 0)
    {
        glDetachShader(m_Handle, m_FsHandle);
    }

    glValidateProgram(m_Handle);
}

GLuint Shader::CreateShader(const std::string& shaderCode, GLuint shaderType)
{
    const unsigned int shaderID = glCreateShader(shaderType);

    const char* c_str = shaderCode.c_str();

    glShaderSource(shaderID, 1, &c_str, NULL);
    glCompileShader(shaderID);

    int status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

    if (!status)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        std::string log;
        log.resize(length);
        glGetShaderInfoLog(shaderID, length, &length, &log[0]);

        std::string errorFunctionName = "--------[ " __FUNCTION__ " ]--------";
        std::string separator; for (size_t i = 0; i < errorFunctionName.size(); i++) separator += "-";

        LOG_OUT(errorFunctionName);
        LOG_OUT("Error occured while compiling a shader: " << log);
        LOG_OUT(separator);

        return -1;
    }

    glAttachShader(m_Handle, shaderID);

    return shaderID;
}

void Shader::Bind() const
{
    glUseProgram(m_Handle);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::CreateUniform(const std::string& name)
{
    int location = glGetUniformLocation(m_Handle, name.c_str());

    m_UniformLocations.insert(
        std::pair<std::string, int>(
            name.c_str(),
            location
            )
    );
}

void Shader::SetFloat(const std::string& name, GLfloat value)
{
#ifdef A_SHADER_DEBUG
    if (uniformLocations.find(name) == uniformLocations.end())
    {
        LOG_OUT("[DEBUG, File: astrum_gpu/shader.h, Function: setMat4x4()]: Variable " << name << " doesn't exist.");
        return;
    }
#endif

    glUniform1f(m_UniformLocations.at(name), value);
}

void Shader::SetInt(const std::string& name, GLint value)
{
#ifdef A_SHADER_DEBUG
    if (uniformLocations.find(name) == uniformLocations.end())
    {
        LOG_OUT("[DEBUG, File: astrum_gpu/shader.h, Function: setMat4x4()]: Variable " << name << " doesn't exist.");
        return;
    }
#endif

    glUniform1i(m_UniformLocations.at(name), value);
}

void Shader::SetMat4x4(const std::string& name, float const* const matrix)
{
#ifdef A_SHADER_DEBUG
    if (uniformLocations.find(name) == uniformLocations.end())
    {
        LOG_OUT("[DEBUG, File: astrum_gpu/shader.h, Function: setMat4x4()]: Variable " << name << " doesn't exist.");
        return;
    }
#endif

    glUniformMatrix4fv(m_UniformLocations.at(name), 1, GL_FALSE, matrix);
}

void Shader::SetVec2(const std::string& name, float const* const vec)
{
#ifdef A_SHADER_DEBUG
    if (uniformLocations.find(name) == uniformLocations.end())
    {
        LOG_OUT("[DEBUG, File: astrum_gpu/shader.h, Function: set_vec3()]: Variable " << name << " doesn't exist.");
        return;
    }
#endif

    glUniform2fv(m_UniformLocations.at(name), 1, vec);
}

void Shader::SetVec3(const std::string& name, float const* const vec)
{
#ifdef A_SHADER_DEBUG
    if (uniformLocations.find(name) == uniformLocations.end())
    {
        LOG_OUT("[DEBUG, File: astrum_gpu/shader.h, Function: set_vec3()]: Variable " << name << " doesn't exist.");
        return;
    }
#endif

    glUniform3fv(m_UniformLocations.at(name), 1, vec);
}

void Shader::SetVec4(const std::string& name, float const* const vec)
{
#ifdef A_SHADER_DEBUG
    if (uniformLocations.find(name) == uniformLocations.end())
    {
        LOG_OUT("[DEBUG, File: astrum_gpu/shader.h, Function: setVec4()]: Variable " << name << " doesn't exist.");
        return;
    }
#endif

    glUniform4fv(m_UniformLocations.at(name), 1, vec);
}

void Shader::SetListMat4x4(const std::string& name, float const* const list, unsigned int size)
{

#ifdef A_SHADER_DEBUG
    if (uniformLocations.find(name) == uniformLocations.end())
    {
        LOG_OUT("[DEBUG, File: astrum_gpu/shader.h, Function: setListMat4x4()]: Variable " << name << " doesn't exist.");
        return;
    }
#endif

    glUniformMatrix4fv(m_UniformLocations.at(name), size, GL_FALSE, list);
}
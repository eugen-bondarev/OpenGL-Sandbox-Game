#pragma once

#include "GpuEntity.h"

#include <GL/glew.h>

#include <string>

class Shader : public GpuEntity {
public:
    template <typename... Args>
    Shader(const std::string& vsCode, const std::string& fsCode, Args&&... args)
    {
        m_Handle = glCreateProgram();

        m_VsHandle = CreateShader(vsCode, GL_VERTEX_SHADER);
        m_FsHandle = CreateShader(fsCode, GL_FRAGMENT_SHADER);

        Link();

        std::vector<std::string> uniforms{ args... };

        for (int i = 0; i < uniforms.size(); i++)
        {
            CreateUniform(uniforms[i]);
        }

        DEBUG_LOG_OUT("[Call] Shader constructor");
    }

    ~Shader() override;

    void Bind() const override;
    void Unbind() const override;

    void CreateUniform(const std::string& name);
    void SetFloat(const std::string& name, GLfloat value);
    void SetInt(const std::string& name, GLint value);
    void SetVec2(const std::string& name, float const* const vec);
    void SetVec3(const std::string& name, float const* const vec);
    void SetVec4(const std::string& name, float const* const vec);
    void SetMat4x4(const std::string& name, float const* const matrix);
    void SetListMat4x4(const std::string& name, float const* const list, unsigned int size);

private:
    GLuint m_VsHandle{ 0 };
    GLuint m_FsHandle{ 0 };

    std::map<std::string, GLuint> m_UniformLocations;

    void Link() const;
    GLuint CreateShader(const std::string& shaderCode, GLuint shaderType);

    Shader(const Shader&) = delete;
    Shader operator=(const Shader&) = delete;
};
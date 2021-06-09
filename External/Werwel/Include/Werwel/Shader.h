#pragma once

#include "GpuEntity.h"

#include <GL/glew.h>

namespace Werwel {

class Shader : public GpuEntity {
public:
	template <typename... Args>
	Shader(const std::string& vsCode, const std::string& fsCode, Args&&... args) {
		handle = glCreateProgram();

		vsHandle = CreateShader(vsCode, GL_VERTEX_SHADER);
		fsHandle = CreateShader(fsCode, GL_FRAGMENT_SHADER);

		Link();

		std::vector<Str> uniforms{ args... };

		for (int i = 0; i < uniforms.size(); i++) {
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

	template <typename T>
	inline void SetListVec2(const std::string& name, const std::vector<T>& vec) {
		glUniform2fv(uniformLocations.at(name), vec.size(), reinterpret_cast<const float*>(&vec[0]));
	}

	void SetVec3(const std::string& name, float const* const vec);
	void SetVec4(const std::string& name, float const* const vec);
	void SetMat2x4(const std::string& name, float const* const matrix);
	void SetMat3x3(const std::string& name, float const* const matrix);
	void SetMat4x4(const std::string& name, float const* const matrix);
	void SetListMat4x4(const std::string& name, float const* const list, unsigned int size);

private:
	GLuint vsHandle { 0 };
	GLuint fsHandle { 0 };

	std::map<std::string, GLuint> uniformLocations;

	void Link() const;
	GLuint CreateShader(const std::string& shaderCode, GLuint shaderType);

	Shader(const Shader&) = delete;
	Shader operator=(const Shader&) = delete;
};

}
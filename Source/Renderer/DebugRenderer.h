// #pragma once

// #include "Werwel/Shader.h"
// #include "Werwel/VAO.h"

// #include "Maths/Primitive.h"

// #include "Core/Window.h"

// #include <array>
// #include <vector>
// #include <map>

// struct Line {
//   std::array<Vec2, 2> points;
//   Color color { 1, 0, 0, 1 };
  
//   inline Line(const Vec2& start, const Vec2& end, const Color& color = Color(1.0f, 0.0f, 0.0f, 1.0f)) {
//     points[0] = start;
//     points[1] = end;
//     this->color = color;
//   }
// };

// inline const std::string vsCode = R"(
//   #version 440 core

//   layout (location = 0) in mat2x4 in_Line;

//   uniform mat4 u_Proj;
//   uniform mat4 u_View;

//   out vec4 color;

//   void main() {
//     vec4 line = in_Line[0];
//     color = in_Line[1];

//     gl_Position = u_Proj * u_View * vec4(gl_VertexID == 0 ? line.xy : line.zw, 0.0, 1.0);
//   }
// )";

// inline const std::string fsCode = R"(
//   #version 440 core

//   in vec4 color;

//   out vec4 out_Color;

//   uniform vec4 u_Color;

//   void main() {
//     out_Color = color;
//   }
// )";

// class Shader {
// public:
// 	template <typename... Args>
// 	Shader(const std::string& vsCode, const std::string& fsCode, Args&&... args) {
// 		handle = glCreateProgram();

// 		vsHandle = CreateShader(vsCode, GL_VERTEX_SHADER);
// 		fsHandle = CreateShader(fsCode, GL_FRAGMENT_SHADER);

// 		Link();

// 		std::vector<std::string> uniforms{ args... };

// 		for (int i = 0; i < uniforms.size(); i++) {
// 			CreateUniform(uniforms[i]);
// 		}
// 	}

// 	~Shader() {    
//     Unbind();
    
//     glDetachShader(handle, vsHandle);
//     glDetachShader(handle, fsHandle);
//     glDeleteShader(vsHandle);
//     glDeleteShader(fsHandle);
//     glDeleteProgram(handle);
//   }

// 	void Bind() const {
//     glUseProgram(handle);
//   }

// 	void Unbind() const {
//     glUseProgram(0);
//   }

// 	void CreateUniform(const std::string& name) {
//     int location = glGetUniformLocation(handle, name.c_str());

//     uniformLocations.insert(
//       std::pair<std::string, int>(
//         name.c_str(),
//         location
//         )
//     );
//   }

// 	template <typename T>
// 	inline void SetListVec2(const std::string& name, const std::vector<T>& vec) {
// 		glUniform2fv(uniformLocations.at(name), vec.size(), reinterpret_cast<const float*>(&vec[0]));
// 	}

// 	void SetMat4x4(const std::string& name, float const* const matrix) {
// 	  glUniformMatrix4fv(uniformLocations.at(name), 1, GL_FALSE, matrix);
//   }

// private:
//   GLuint handle { 0 };

// 	GLuint vsHandle { 0 };
// 	GLuint fsHandle { 0 };

// 	std::map<std::string, GLuint> uniformLocations;

// 	void Link() const {
//     glLinkProgram(handle);

//     if (vsHandle != 0) {
//       glDetachShader(handle, vsHandle);
//     }

//     if (fsHandle != 0) {
//       glDetachShader(handle, fsHandle);
//     }

//     glValidateProgram(handle);
//   }

// 	GLuint CreateShader(const std::string& shaderCode, GLuint shaderType) {
//     const unsigned int shaderID = glCreateShader(shaderType);

//     const char* c_str = shaderCode.c_str();

//     glShaderSource(shaderID, 1, &c_str, NULL);
//     glCompileShader(shaderID);

//     int status;
//     glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);

//     if (!status) {
//       int length;
//       glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
//       std::string log;
//       log.resize(length);
//       glGetShaderInfoLog(shaderID, length, &length, &log[0]);

//       std::string errorFunctionName = "--------[ " __FUNCTION__ " ]--------";
//       std::string separator; for (size_t i = 0; i < errorFunctionName.size(); i++) separator += "-";

//       std::cout << errorFunctionName << '\n';
//       std::cout << "Error occured while compiling a shader: " << log << '\n';
//       std::cout << separator << '\n';

//       return -1;
//     }

//     glAttachShader(handle, shaderID);

//     return shaderID;
//   }

// 	Shader(const Shader&) = delete;
// 	Shader operator=(const Shader&) = delete;
// };

// class VBO {
// public:
//   VBO(int attribute = 0) {
//     GLuint startAttribute = attribute;
//     attributes.resize(2);
    
// 		glGenBuffers(1, &handle);
// 		glBindBuffer(GL_ARRAY_BUFFER, handle);
//       glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4) * 2, reinterpret_cast<void *>(0));
//       glVertexAttribDivisor(0, 1);

//       glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4) * 2, reinterpret_cast<void *>(4 * sizeof(float)));
//       glVertexAttribDivisor(1, 1);

//       attributes.push_back(0);
//       attributes.push_back(1);
//       glBufferData(GL_ARRAY_BUFFER, sizeof(Vec4) * 512, nullptr, GL_DYNAMIC_DRAW);
// 		glBindBuffer(GL_ARRAY_BUFFER, 0);
//   }

//   void Bind() const {
// 		glBindBuffer(GL_ARRAY_BUFFER, handle);
    
//     for (const auto attrib : attributes) {
//       glEnableVertexAttribArray(attrib);
//     }
//   }

//   template <typename T, bool bind = true>
//   inline void Update(const std::vector<T>& vec, int amount, int pos = 0) const {
//     if (bind) {
//       Bind();
//     }
//     glBufferSubData(GL_ARRAY_BUFFER, pos, sizeof(Vec4) * 2 * amount, vec.data());
//   }

// private:
//   GLuint handle { 0 };

//   std::vector<GLuint> attributes;
// };

// class VAO {
// public:
//   VAO() {
// 		glGenVertexArrays(1, &handle);
//   }

//   void Bind() {    
// 		glBindVertexArray(handle);
//   }

//   void Unbind() {
// 		glBindVertexArray(0);
//   }

// private:
//   GLuint handle { 0 };

//   Ref<VBO> vbo;

// 	VAO(const VAO&) = delete;
// 	VAO operator=(const VAO&) = delete;
// };

// namespace Linow {

// inline Ref<VAO> vao;
// inline Ref<VBO> vbo;

// inline std::vector<Line> lines;
// inline Shader* lineShader;

// inline void Init() {
//   lineShader = new Shader(vsCode, fsCode, "u_Proj", "u_View");
//   lineShader->Bind();
//     lineShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));

//   vao = CreateRef<VAO>();
//   vao->Bind();
//     vbo = CreateRef<VBO>();
// }

// inline void Render(const Mat4& viewMatrix) {    
//   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//   lineShader->Bind();
//     lineShader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
//     lineShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));

//   vao->Bind();
//     vbo->Update(lines, lines.size());
//     glDrawArraysInstanced(GL_TRIANGLES, 0, 3, lines.size());

//   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// }

// inline void ClearLines() {
//   lines.clear();
// }

// template <typename... Args>
// inline void AddLine(Args&&... args) {
//   lines.emplace_back(std::forward<Args>(args)...);
// }

// inline void AddQuad(Vec2 start, Vec2 end, Color color = Color(1.0f, 0.0f, 0.0f, 1.0f)) {
//   Line left(start, Vec2(start.x, end.y), color);
//   Line right(Vec2(end.x, start.y), end, color);
//   Line top(start, Vec2(end.x, start.y), color);
//   Line bottom(Vec2(start.x, end.y), end, color);

//   AddLine(left);
//   AddLine(right);
//   AddLine(top);
//   AddLine(bottom);
// }

// }
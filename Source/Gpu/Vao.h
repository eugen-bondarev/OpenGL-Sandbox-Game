#pragma once

#include "GpuEntity.h"

#include "VertexBufferLayout.h"

class Vao : public GpuEntity {
public:
    GLuint posVbo;

    template <typename T_Vertex>
    Vao(const std::vector<T_Vertex> &vertices, const std::vector<VertexBufferLayout> &layouts, const std::vector<int> &indices, bool dynamic = false) {
        m_Attributes.resize(layouts.size());

        glGenVertexArrays(1, &m_Handle);

        glBindVertexArray(m_Handle);

        unsigned int vbo{ 0 };
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T_Vertex), &vertices[0], GL_STATIC_DRAW);       
        for (int i = 0; i < layouts.size(); i++) {
            glVertexAttribPointer(i, layouts[i].Size, GL_FLOAT, GL_FALSE, layouts[i].Stride, reinterpret_cast<void *>(layouts[i].Offset));
            m_Attributes[i] = i;
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_Buffers.emplace_back(vbo);

        if (dynamic) {
            glGenBuffers(1, &posVbo);
            glBindBuffer(GL_ARRAY_BUFFER, posVbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 0, NULL, GL_STREAM_DRAW);    //NULL (empty) buffer
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            m_Buffers.emplace_back(posVbo);
        }

        glGenBuffers(1, &m_IndexVboHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        m_Buffers.emplace_back(m_IndexVboHandle);

        m_VertexCount = static_cast<unsigned int>(indices.size());

        glBindVertexArray(0);

        DEBUG_LOG_OUT("[Call] Vao constructor");
    }
    ~Vao() override;

    void Bind() const override;
    void Unbind() const override;

    void Render() const;

    GLuint GetVertexCount() const;

private:
    GLuint m_IndexVboHandle{0};
    GLuint m_VertexCount{0};
    std::vector<GLuint> m_Attributes;
    std::vector<GLuint> m_Buffers;

    Vao(const Vao &) = delete;
    Vao &operator=(const Vao &) = delete;
};
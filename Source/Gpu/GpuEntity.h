#pragma once

#include <GL/glew.h>

class GpuEntity {
public:
    GpuEntity() = default;
    virtual ~GpuEntity() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    inline GLuint GetHandle() { return m_Handle; }

protected:
    GLuint m_Handle { 0 };

private:
    GpuEntity(const GpuEntity&) = delete;
    GpuEntity operator=(const GpuEntity&) = delete;
};
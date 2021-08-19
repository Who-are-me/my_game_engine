#pragma once

namespace MatrixEngine {
    class ShaderProgram {

        bool m_isCompiled = false;
        unsigned int m_id = 0;

    public:
        ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
        ShaderProgram& operator=(ShaderProgram&& shaderProgram);
        ShaderProgram(ShaderProgram&& shaderProgram);
        ~ShaderProgram();

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;

        void bind();
        static void unbind();
        bool isCompiled() const;
    };
}

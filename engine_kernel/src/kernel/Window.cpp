#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "kernel/Window.hpp"
#include "kernel/Log.hpp"
#include "kernel/Rendering/OpenGL/ShaderProgram.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

namespace MatrixEngine {

    static bool s_GLFW_initialized = false;

    GLfloat points[] = {
        0.1f,  0.6f,  0.0f,
        0.6f, -0.36f, 0.0f,
       -0.5f, -0.5f,  0.0f
    };

    GLfloat points_color[] = {
        0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f
    };

    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "out vec3 color;"
        "void main() {"
        "   color = vertex_color;"
        "   gl_Position = vec4(vertex_position, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = vec4(color, 1.0);"
        "}";

    std::unique_ptr<ShaderProgram> p_shader_program;
    GLuint vao;

    Window::Window(std::string title, const unsigned int width, const unsigned int height)
        : m_data({std::move(title), width, height})
    {
        m_background_color[0] = 0.3f;
        m_background_color[1] = 0.45f;
        m_background_color[2] = 0.45f;
        m_background_color[3] = 1.0f;

        int resultCode = init();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplOpenGL3_Init();
        ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
    }

    Window::~Window() {
        shutdown();
    }

    int Window::init() {
        LOG_INFO("Initialize class Window '{0}' width size {1}x{2}",
                 m_data.title, m_data.width, m_data.height);

        if(!s_GLFW_initialized) {
            if (!glfwInit()) {
                LOG_CRITICAL("Can't initialize GLFW!");
                return -1;
            }
        }

        s_GLFW_initialized = true;

        m_pWindow = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);

        if (!m_pWindow) {
            LOG_CRITICAL("Can't create window {0} width size {1}x{2}", m_data.title, m_data.width, m_data.height);
            glfwTerminate();
            return -2;
        }

        glfwMakeContextCurrent(m_pWindow);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_CRITICAL("'Glad' library is not Initialize");
            return -3;
        }

        glfwSetWindowUserPointer(m_pWindow, &m_data);

        glfwSetWindowSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height){
                LOG_INFO("New size {0}x{1}", width, height);

                WindowData& data = *static_cast<WindowData*> (glfwGetWindowUserPointer(pWindow));

                data.width = width;
                data.height = height;

                EventWindowResize event(width, height);
                data.eventCallbackFn(event);
            }
        );

        glfwSetCursorPosCallback(m_pWindow,
            [](GLFWwindow* pWindow, double x, double y) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                EventMouseMoved event(x, y);
                data.eventCallbackFn(event);
            }
        );

        glfwSetWindowCloseCallback(m_pWindow,
            [](GLFWwindow* pWindow) {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

                EventWindowClose event;
                data.eventCallbackFn(event);
            }
        );

        // set limit draw on window
        glfwSetFramebufferSizeCallback(m_pWindow,
            [](GLFWwindow* pWindow, int width, int height) {
                glViewport(0, 0, width, height);
            }
        );

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if(!p_shader_program->isCompiled()) {
            return false;
        }
/*
        GLuint vs = glCreateShader(GL_VERTEX_SHADER);                   // init and compile vertex shader, vs - id vertex shader
        glShaderSource(vs, 1, &vertex_shader, nullptr);                 // load source code shader, vs - id, 1 - count lines, adders sourse, nullptr
        glCompileShader(vs);                                            // compile shader

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);                 // init and compile fragment shader, fs - id fragment shader
        glShaderSource(fs, 1, &fragment_shader, nullptr);               // load source code shader, vs - id, 1 - count lines, adders sourse, nullptr
        glCompileShader(fs);                                            // compile shader

        shader_program = glCreateProgram();                             // create id shader program
        glAttachShader(shader_program, vs);                             // attach vertex shader with program shader
        glAttachShader(shader_program, fs);                             // attach fragment shader with program shader
        glLinkProgram(shader_program);                                  // link attached shader with program

        glDeleteShader(vs);                                             // delete vertex shader
        glDeleteShader(fs);                                             // delete fragment shader
*/
                                                                        // vbo - vertex buffer object
        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);                                                       // 1 - number buffers, create id for points vbo
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                                          // make current buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);              // set data in current buffer, type, size(bite), what, how

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);                                                       // 1 - number buffers, create id for colors vbo
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);                                          // make current buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(points_color), points_color, GL_STATIC_DRAW);  // set data in current buffer, type, size(bite), what, how

        glGenVertexArrays(1, &vao);                                     // create vertex array
        glBindVertexArray(vao);                                         // make current vao buffer

        glEnableVertexAttribArray(0);                                   // add position 0 (vertex)
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                      // make current buffer
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    // bind data, position in shader, count parametr (x,y,z), format data, normalaze?(Y,n), bias, nullptr

        glEnableVertexAttribArray(1);                                   // add position 1 (vertex_color)
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);                      // make current buffer
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);    // bind data, position in shader, count parametr (x,y,z), format data, normalaze?(Y,n), bias, nullptr

        return 0;
    }

    // function for paints on frame in the window
    void Window::onUpdate() {
        glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        p_shader_program->bind();                                       // old(include shader program)
        glBindVertexArray(vao);                                         // bind vao (vertex array object have all bind data and shader)
        glDrawArrays(GL_TRIANGLES, 0, 3);                               // command draw on window, what, position start draw(vertex), count vertex

        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize.x = static_cast<float>(getWidth());
        io.DisplaySize.y = static_cast<float>(getHeight());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow(); // show built-in function for paint demo window

        ImGui::Begin("Background Color Window");
        ImGui::ColorEdit4("Background Color", m_background_color);
        ImGui::End();

        ImGui::Render(); // render ImGui window
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();

    }

    void Window::shutdown() {
        glfwDestroyWindow(m_pWindow);
        LOG_INFO("[Window] window will be closed");
    }

    void Window::setEventCallback(const EventCallbackFn &callback) {
        m_data.eventCallbackFn = callback;
    }

}

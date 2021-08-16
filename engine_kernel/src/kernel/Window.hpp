//
// Created by forever on 04.07.21.
//

#pragma once

#include <string>
#include <functional>

#include <kernel/Event.hpp>

struct GLFWwindow;

namespace MatrixEngine {

    class Window {
    public:
        using EventCallbackFn = std::function<void(BaseEvent&)>;

    private:
        struct WindowData {
            std::string title;
            unsigned int width;
            unsigned int height;
            EventCallbackFn eventCallbackFn;
        };

        GLFWwindow* m_pWindow = nullptr;
        WindowData m_data;
        float m_background_color[4];

        int init();
        void shutdown();

    public:
        Window(std::string title,
               const unsigned int width,
               const unsigned int height);

        Window(const Window &) = delete;
        Window(Window &&) = delete;
        Window& operator=(const Window &) = delete;
        Window& operator=(Window &&) = delete;

        void onUpdate();
        void setEventCallback(const EventCallbackFn& callback);
        unsigned int getWidth() const { return m_data.width; }
        unsigned int getHeight() const { return m_data.height; }

        ~Window();
    };
}

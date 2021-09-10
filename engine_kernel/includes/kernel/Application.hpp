#pragma once

//
// Created by forever on 04.07.21.
//

#include <memory>


#include "Event.hpp"


namespace MatrixEngine
{
    class Application
    {
        std::unique_ptr<class Window> m_pWindow;
        EventDispatcher m_event_dispatcher;
        bool m_bCloseWindow = false;

    public:
        Application();
        Application(const Application &) = delete;
        Application(Application &&) = delete;

        Application& operator=(const Application &) = delete;
        Application& operator=(Application &&) = delete;

        virtual int start(unsigned int window_width, unsigned int window_height, const char* title);
        virtual void onUpdate();

        virtual ~Application();
    };
}

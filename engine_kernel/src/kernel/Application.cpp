//
// Created by forever on 04.07.21.
//

#include "kernel/Application.hpp"
#include "kernel/Log.hpp"
#include "kernel/Window.hpp"


namespace MatrixEngine
{
    Application::Application() {
        LOG_INFO("Application to start!");
    }


    Application::~Application() {
        LOG_INFO("Application to destroyed!");
    }


    int Application::start(unsigned int window_width, unsigned int window_height, const char* title) {
        m_pWindow = std::make_unique<Window>(title, window_width, window_height);

        m_event_dispatcher.addEventListener<EventMouseMoved>(
            [](EventMouseMoved& event){
                //LOG_INFO("[MouseMoved] Mouse moved to {0}x{1}", event.x, event.y);
            }
        );

        m_event_dispatcher.addEventListener<EventWindowResize>(
            [](EventWindowResize& event){
                LOG_INFO("[WindowResize] Changed size to {0}x{1}", event.width, event.height);
            }
        );

        m_event_dispatcher.addEventListener<EventWindowClose>(
            [&](EventWindowClose& event){
                LOG_INFO("[Application] window is will be closed");
                m_bCloseWindow = true;
            }
        );

        m_pWindow->setEventCallback(
            [&](BaseEvent& event) {
                m_event_dispatcher.dispatch(event);
            }
        );

        while(!m_bCloseWindow) {
            m_pWindow->onUpdate();
            onUpdate();
        }
        m_pWindow = nullptr;

        return 0;
    }


    void Application::onUpdate() {

    }
}

//
// Created by forever on 04.07.21.
//

#pragma once

#include <functional>
#include <array>

namespace MatrixEngine {

    enum class EventType {
        WindowResize = 0,
        WindowClose,
        KeyPressed,
        KeyReleased,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        EventsCount
    };


    struct BaseEvent {
        virtual ~BaseEvent() = default;
        virtual EventType getType() const = 0;
    };


    class EventDispatcher {
        std::array<std::function<void(BaseEvent&)>, static_cast<size_t>(EventType::EventsCount)> m_eventCallbacks;

    public:
        template<typename EventType>
        void addEventListener(std::function<void(EventType&)> callback) {
            auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
                if (e.getType() == EventType::type)
                    func(static_cast<EventType&>(e));
            };

            m_eventCallbacks[static_cast<size_t>(EventType::type)] = std::move(baseCallback);
        }

        void dispatch(BaseEvent& event) {
            auto& callback = m_eventCallbacks[static_cast<size_t>(event.getType())];
            if (callback)
                callback(event);
        }
    };


    struct EventMouseMoved : public BaseEvent {

        static const EventType type = EventType::MouseMoved;
        double x;
        double y;

        EventMouseMoved(const double new_x, const double new_y)
            : x(new_x) , y(new_y)
        {

        }

        virtual EventType getType() const override {
            return type;
        }

    };


    struct EventWindowResize : public BaseEvent {

        static const EventType type = EventType::WindowResize;
        unsigned int width;
        unsigned int height;

        EventWindowResize(const unsigned int new_width, const unsigned int new_height)
            : width(new_width) , height(new_height)
        {

        }

        virtual EventType getType() const override {
            return type;
        }
    };


    struct EventWindowClose : public BaseEvent {

        static const EventType type = EventType::WindowClose;

        virtual EventType getType() const override {
            return type;
        }
    };
}

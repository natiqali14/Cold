#include "../includes.h"
#include "Events.h"
#include "EventHandler.h"
namespace EventSystemHelper {
    void initialise();
    void subscribe(EventType type, IEventHandler* event_handle);
    void unsubscribe(EventType type, IEventHandler* event_handle);
    void trigger_event(EventType type, IEvent* event_data);
    void dispatch_events();
    void queue_events(EventType type, IEvent* event_data);
    void shut_down();
}


/**
 * @brief Use this macro to make a new object for event handler
 * @param event_type Pass the object type of event
 * @param function_to_call function to call when event is dispatched
 * @param object_pointer pointer to object, that the function is calling
 */
#define BIND_EVENT_FUNCTION(event_type,function_to_call, object_pointer) \
    new EventHandler<event_type> (std::bind(function_to_call, object_pointer, std::placeholders::_1))
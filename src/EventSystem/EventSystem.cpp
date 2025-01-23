#include "EventSystem.h"

#include <Logger.h>

#include "EventHandler.h"
void EventSystem::initialise()
{
    
}


void EventSystem::subscribe(EventType type, IEventHandler* event_handle)
{
     if(PFN_to_trigger.count(type)) {
        PFN_to_trigger[type].push_back(event_handle->get_uid());
        event_handler_map.insert({event_handle->get_uid(),event_handle});
    }
    else {
        std::vector<u32> uid_list;
        uid_list.push_back(event_handle->get_uid());
        PFN_to_trigger.insert({type, uid_list});
        event_handler_map.insert({event_handle->get_uid(),event_handle});
    }
    
}

void EventSystem::unsubscribe(EventType type, IEventHandler* event_handle)
{
    if(PFN_to_trigger.count(type)) {
        u32 id = event_handle->get_uid();
        auto& id_list = PFN_to_trigger[type];

        for(int i = 0; i < id_list.size(); i++) {
            if(id_list[i] == id){
                id_list.erase(id_list.begin() + i);   // deleted the uid from map PFN_to_trigger
                event_handler_map.erase(id);   // delete the event handler object.
            } 
        }
    }
}

void EventSystem::trigger_event(EventType type, IEvent* event_data)
{
    bool key_found = false;
    if(!PFN_to_trigger.count(type)) {
        event_data->set_handled_flag(true);
        return;
    }

    if(type == EVENTTYPE_KEY_RELEASED) {
        auto e = static_cast<KeyReleasedEvent*>(event_data);
        if (key_pressed.count(e->get_key_code())) key_pressed.erase(e->get_key_code());
    }
    else if (type == EVENTTYPE_KEY_PRESSED) {
        auto e = static_cast<KeyPressedEvent*>(event_data);
        if (key_pressed.count(e->get_key_code())) key_found = true;
        else key_pressed.insert(e->get_key_code());
    }
    for(auto& PFN_handle : PFN_to_trigger[type]) {
        if (event_handler_map[PFN_handle]->is_toggle && key_found) continue;
        event_handler_map[PFN_handle]->call(event_data);


    }
}

void EventSystem::dispatch_events()
{
    while(!events_queue.empty()) {
        auto queue_event_data = events_queue.front();
        trigger_event(queue_event_data.first, queue_event_data.second);
        events_queue.pop();
        delete queue_event_data.second;
    }
}

void EventSystem::queue_events(EventType type,  IEvent* event_data)
{
    events_queue.push({type, event_data});
}

void EventSystem::shut_down()
{
    PFN_to_trigger.clear();
    event_handler_map.clear();
    events_queue = {};
}


#pragma once
static int u_id_generator() {
    static int id = 0;
    return id++;
}

/**
 * @class IEventHandler
 * @brief An interface for an event handler
 */
class IEventHandler {
public:
    /**
     * @brief A pure virtual function to call the event handler
     * @param e A pointer to an IEvent object
     */
    virtual void call(IEvent* e) = 0;
    
    /**
     * @brief A pure virtual function to get the unique identifier of the event handler
     * @return A u32 representing the unique identifier
     */
    virtual u32 get_uid() = 0;
    
    /**
     * @brief A virtual destructor
     */
    virtual ~IEventHandler() = default;
};

/**
 * @class EventHandler
 * @brief A template class for an event handler
 * @tparam T The type of the event
 */
template <typename T>
class EventHandler : public IEventHandler {
public:
    /**
     * @brief An alias for the event type
     */
    using EventType = T;
    
    /**
     * @brief An alias for the event handler function
     */
    using handler = std::function<void(EventType* e_type)>;
    
    /**
     * @brief A constructor that takes an event handler function as an argument
     * @param h A handler function
     */
    explicit EventHandler(handler h) : e_handler(h) {
        uid = u_id_generator();
        std::cout << uid;
    }
    
    /**
     * @brief A function to call the event handler
     * @param e A pointer to an IEvent object
     */
    void call(IEvent* e) override {
        e_handler(static_cast<EventType*>(e));
    }
    
    /**
     * @brief A function to get the unique identifier of the event handler
     * @return A u32 representing the unique identifier
     */
    u32 get_uid() override {
        return uid;
    }
    
private:
    /**
     * @brief The event handler function
     */
    handler e_handler;
    
    /**
     * @brief The unique identifier of the event handler
     */
    u32 uid;
    
    /**
     * @brief A function to set the unique identifier of the event handler
     */
    
};
#pragma once

#include "../includes.h"

// Enumeration for different types of events
enum EventType : u16
{
    EVENTTYPE_NONE,                  // No event
    EVENTTYPE_KEY_PRESSED,           // Key pressed event
    EVENTTYPE_KEY_RELEASED,          // Key released event
    EVENTTYPE_MOUSE_MOVED,           // Mouse moved event
    EVENTTYPE_MOUSE_BUTTON_PRESSED,  // Mouse button pressed event
    EVENTTYPE_MOUSE_BUTTON_RELEASED, // Mouse button released event
    EVENTTYPE_MOUSE_SCROLLED,        // Mouse scrolled event

    // GUI EVENTS
    EVENTTYPE_CURSOR_ACTIVATED,
    // GUI EVENTS
};

// Interface for all events
class IEvent
{
    bool b_handled; // Flag to check if the event has been handled

public:
    /**
     * @brief Construct a new IEvent object
     */
    IEvent() {}

    /**
     * @brief Get the type of the event
     *
     * @return EventType The type of the event
     */
    virtual EventType get_type() const = 0;

    /**
     * @brief Check if the event has been handled
     *
     * @return true If the event has been handled
     * @return false If the event has not been handled
     */
    bool is_handled() const { return b_handled; }

    /**
     * @brief Set the handled flag
     *
     * @param b_flag The handled flag
     */
    void set_handled_flag(bool b_flag) { b_handled = b_flag; }

    /**
     * @brief Destroy the IEvent object
     */
    virtual ~IEvent() {}
};

// Event for key press
class KeyPressedEvent : public IEvent
{
    int key_code; // Key code for the key press event

public:
    /**
     * @brief Construct a new KeyPressedEvent object
     *
     * @param code The key code for the key press event
     */
    KeyPressedEvent(int code)
        : IEvent(), key_code(code) {}

    /**
     * @brief Get the key code for the key press event
     *
     * @return int The key code for the key press event
     */
    int get_key_code() const { return key_code; }

    /**
     * @brief Get the type of the event
     *
     * @return EventType The type of the event
     */
    EventType get_type() const override { return EVENTTYPE_KEY_PRESSED; }
};

// Event for key release
class KeyReleasedEvent : public IEvent
{
    int key_code; // Key code for the key release event

public:
    /**
     * @brief Construct a new KeyReleasedEvent object
     *
     * @param code The key code for the key release event
     */
    KeyReleasedEvent(int code)
        : IEvent(), key_code(code) {}

    /**
     * @brief Get the key code for the key release event
     *
     * @return int The key code for the key release event
     */
    int get_key_code() const { return key_code; }

    /**
     * @brief Get the type of the event
     *
     * @return EventType The type of the event
     */
    EventType get_type() const override { return EVENTTYPE_KEY_RELEASED; }
};

// Event for mouse move
class KeyMouseMovedEvent : public IEvent
{
    f64 x_val; // X-coordinate for the mouse move event
    f64 y_val; // Y-coordinate for the mouse move event

public:
    /**
     * @brief Construct a new KeyMouseMoved object
     *
     * @param x The x-coordinate for the mouse move event
     * @param y The y-coordinate for the mouse move event
     */
    KeyMouseMovedEvent(f64 x, f64 y)
        : IEvent(), x_val(x), y_val(y) {}

    /**
     * @brief Get the coordinates for the mouse move event
     *
     * @return std::array<u16, 2> The coordinates for the mouse move event
     */
    std::array<f64, 2> get_values() const { return {x_val, y_val}; }

    /**
     * @brief Get the type of the event
     *
     * @return EventType The type of the event
     */
    EventType get_type() const override { return EVENTTYPE_MOUSE_MOVED; }
};

//  -------------------------------------- GUI EVENTS  --------------------------------------  //
class EventCursorActivated : public IEvent {
    bool is_activated;
public:
    EventCursorActivated(bool activate) {
        is_activated = activate;
    }
    bool get_status() {
        return is_activated;
    }

    EventType get_type() const override { return EVENTTYPE_CURSOR_ACTIVATED; }
};
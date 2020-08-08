#ifndef __KEYBOARDEVENT_H__
#define __KEYBOARDEVENT_H__
#include "Event.h"

namespace Rush{ 

class KeyboardPressEvent : public Event{
public:
    int keycode;
    int modifiers;

    KeyboardPressEvent(int keycode, int modifiers)
    : keycode(keycode), modifiers(modifiers){}
    EVENT_BODY(KeyboardPress);
    virtual std::string GetString() const override { return "KeyboardPressEvent(" + std::to_string(keycode) + ")"; }
};

class KeyboardRepeatEvent : public Event{
public:
    int keycode;
    int modifiers;

    KeyboardRepeatEvent(int keycode, int modifiers)
    : keycode(keycode), modifiers(modifiers){}
    EVENT_BODY(KeyboardRepeat);
    virtual std::string GetString() const override { return "KeyboardRepeatEvent(" + std::to_string(keycode) + ")"; }
};

class KeyboardReleaseEvent : public Event{
public:
    int keycode;
    int modifiers;

    KeyboardReleaseEvent(int keycode, int modifiers)
    : keycode(keycode), modifiers(modifiers){}
    EVENT_BODY(KeyboardRelease);
    virtual std::string GetString() const override { return "KeyboardReleaseEvent(" + std::to_string(keycode) + ")"; }
};

class KeyboardTypeEvent : public Event {
public:
    unsigned int typed;

    KeyboardTypeEvent(unsigned int typed)
    : typed(typed) {}
    EVENT_BODY(KeyboardType);
    virtual std::string GetString() const override { return "KeyboardTypeEvent(" + std::to_string(typed) + ")"; }
};

} 
#endif // __KEYBOARDEVENT_H__

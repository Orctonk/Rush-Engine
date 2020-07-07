#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdint.h>
#include <utility>

#include "Rush/events/Keycodes.h"
#include "Rush/events/Mousecodes.h"
#include "Rush/events/Event.h"
#include "Rush/events/MouseEvent.h"
#include "Rush/events/KeyboardEvent.h"

namespace Rush{

class Input {
private:
    bool m_KeyStates[RUSH_KEYCODE_LAST+1];
    bool m_MouseButtonStates[RUSH_MOUSE_BUTTON_LAST + 1];
    std::pair<float,float> m_MousePos;

    static Input *s_Instance;
    
    Input();

    bool MousePressHandle(MousePressedEvent &e);
    bool MouseReleaseHandle(MouseReleasedEvent &e);
    bool MouseMoveHandle(MouseMoveEvent &e);
    bool KeyPressHandle(KeyboardPressEvent &e);
    bool KeyReleaseHandle(KeyboardReleaseEvent &e);
public:
    static bool KeyDown(int keycode) { return s_Instance->m_KeyStates[keycode]; }
    static bool MousePressed(int button) { return s_Instance->m_MouseButtonStates[button]; }
    static std::pair<float,float> MousePos() { return s_Instance->m_MousePos; }

    static void Update(Event &e);
};

}

#endif // __INPUT_H__
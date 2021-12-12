#include "Input.h"
#include "Rushpch.h"

namespace Rush {
Input *Input::s_Instance = new Input();

Input::Input() {
    for (int i = 0; i < RUSH_KEYCODE_LAST + 1; i++)
        m_KeyStates[i] = false;
    for (int i = 0; i < RUSH_MOUSE_BUTTON_LAST + 1; i++)
        m_MouseButtonStates[i] = false;
    m_MousePos = std::pair{ 0, 0 };
}

void Input::Update(Event &e) {
    RUSH_PROFILE_FUNCTION();
    e.Dispatch<MousePressedEvent>(std::bind(&Input::MousePressHandle, s_Instance, std::placeholders::_1));
    e.Dispatch<MouseReleasedEvent>(std::bind(&Input::MouseReleaseHandle, s_Instance, std::placeholders::_1));
    e.Dispatch<MouseMoveEvent>(std::bind(&Input::MouseMoveHandle, s_Instance, std::placeholders::_1));
    e.Dispatch<KeyboardPressEvent>(std::bind(&Input::KeyPressHandle, s_Instance, std::placeholders::_1));
    e.Dispatch<KeyboardReleaseEvent>(std::bind(&Input::KeyReleaseHandle, s_Instance, std::placeholders::_1));
}

bool Input::MousePressHandle(MousePressedEvent &e) {
    m_MouseButtonStates[e.keycode] = true;
    return false;
}

bool Input::MouseReleaseHandle(MouseReleasedEvent &e) {
    m_MouseButtonStates[e.keycode] = false;
    return false;
}

bool Input::MouseMoveHandle(MouseMoveEvent &e) {
    m_MousePos = std::pair{ e.x, e.y };
    return false;
}

bool Input::KeyPressHandle(KeyboardPressEvent &e) {
    m_KeyStates[e.keycode] = true;
    return false;
}

bool Input::KeyReleaseHandle(KeyboardReleaseEvent &e) {
    m_KeyStates[e.keycode] = false;
    return false;
}

bool Input::KeyDown(int keycode) { return s_Instance->m_KeyStates[keycode]; }
bool Input::MousePressed(int button) { return s_Instance->m_MouseButtonStates[button]; }
std::pair<int, int> Input::MousePos() { return s_Instance->m_MousePos; }

} // namespace Rush
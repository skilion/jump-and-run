#pragma once

namespace myengine {

template<class T> class Sender;

class KeyEvent
{
public:
    enum KeyCode {
        K_UNKNOW,

        //Mouse buttons
        MK_LEFT,
        MK_RIGHT,
        MK_MIDDLE,

        //Numbers
        K_0, K_1, K_2, K_3, K_4, K_5, K_6, K_7, K_8, K_9,

        //Letters
        K_A, K_B, K_C, K_D, K_E, K_F, K_G, K_H, K_I, K_J, K_K, K_L, K_M,
        K_N, K_O, K_P, K_Q, K_R, K_S, K_T, K_U, K_V, K_W, K_X, K_Y, K_Z,

        K_ESCAPE,
        K_BACKSPACE,
        K_TAB,
        K_ENTER,
        K_PAUSE,
        K_CAPITAL,
        K_SPACE,

        //Arrows
        K_LEFT,
        K_UP,
        K_RIGHT,
        K_DOWN,

        // Ctrl, Alt, Shift
        K_LCTRL,
        K_RCTRL,
        K_LSHIFT,
        K_RSHIFT,
        K_LALT,
        K_RALT,

        K_PRIOR, //PAGE UP key
        K_NEXT,  //PAGE DOWN key
        K_END,   //END key
        K_HOME,  //HOME key
        K_INSERT,
        K_DELETE,

        K_ADD,
        K_DIVIDE,
        K_DECIMAL,
        K_NUMLOCK,
        K_MULTIPLY,
        K_SUBTRACT,
        K_CLEAR,
        //KP_SEPARATOR,

        //Numeric keypad
        KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9,

        //Function keys
        K_F1, K_F2, K_F3, K_F4, K_F5, K_F6, K_F7, K_F8, K_F9, K_F10, K_F11,
        K_F12, K_F13, K_F14, K_F15, K_F16, K_F17, K_F18, K_F19, K_F20, K_F21, K_F22,

        K_OEM_1,	// US ;:	IT ��
        K_OEM_2,	// US /?	IT ��
        K_OEM_3,	// US `~	IT ��
        K_OEM_4,	// US [{	IT '?
        K_OEM_5,	// US \|	IT \|
        K_OEM_6,	// US ]}	IT �^
        K_OEM_7,	// US '"	IT ��
        //K_OEM_8,
        K_OEM_102,	// US \		IT <>

        K_OEM_PLUS,		// +
        K_OEM_COMMA,	// ,
        K_OEM_MINUS,	// -
        K_OEM_PERIOD,	// .

        K_LAST
    };

    enum KeyState {
        KEY_RELEASE,
        KEY_PRESS
    };

    KeyEvent(KeyCode keyCode, KeyState keySate);
    //~KeyEvent();

    KeyCode			getKeyCode() const;
    KeyState		getKeyState() const;


private:
    KeyCode		keyCode;
    KeyState	keyState;
};

extern Sender<KeyEvent> keyEventSender;

}

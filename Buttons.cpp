#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <windef.h>
#include <wincon.h>
#include <functional>

#include "Interface.h"

using namespace std;

struct Button {
    short x, y, width;
    string text;
    function <void(void)> callback;

    Button(short x, short y, short width, const string &text, const function<void(void)> &callback = NULL) {
        this -> x = x;
        this -> y = y;
        this -> width = width;
        this -> text = text;
        this -> callback = callback;
    }

    void on_click() const {
        if(callback != NULL) {
            callback();
        }
    }
};

void btn1_callback() {
    MessageBoxA(NULL, "BUTTON 1 PRESSED", "TEST MESSAGE BOX", MB_OK | MB_ICONINFORMATION);
}

void btn2_callback() {
    MessageBoxA(NULL, "BUTTON 2 PRESSED", "TEST MESSAGE BOX", MB_OK | MB_ICONINFORMATION);
}

void ShowConsoleCursor(Interface *it, bool show)
{
    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(it->h_out, &cursorInfo);
    cursorInfo.bVisible = show;
    SetConsoleCursorInfo(it->h_out, &cursorInfo);
}

void drawButton(Interface *it, const Button &b, bool selected) {

    if(selected)
        it->invertColors();

    it -> SetPosition(b.x, b.y);
    cout << string(b.width, '-');

    for(int i = 1; i < 2; i++) {
        it -> SetPosition(b.x, b.y + i);
        cout << '|';

        if(i == 1) {

            int padding = b.width/2 - b.text.length() / 2;
                cout << string(padding, ' ') << b.text << string(b.width - padding - b.text.length() - 1, ' ');
        }

        it -> SetPosition(b.x + b.width - 1, b.y + i);
        cout << '|';
    }

    it -> SetPosition(b.x, b.y + 2);
    cout << string(b.width, '-');

    if(selected)
        it->invertColors();

}

void draw_loop(Interface *it, const vector<Button> &btns) {

    INPUT_RECORD event_buffer;
    DWORD event_number;

    COORD pos;

    while(true) {

        ReadConsoleInputA(it->h_in, &event_buffer, 1, &event_number);

        for(int i = 0; i < event_number; i++) {

            switch(event_buffer.EventType) {
                case MOUSE_EVENT:

                    if(event_buffer.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
                        for(const auto &btn : btns) {
                            if (pos.X > btn.x && pos.X < (btn.x + btn.width) && pos.Y > btn.y && pos.Y < (btn.y + 3)) {
                                btn.on_click();
                            }
                        }
                    } else if(event_buffer.Event.MouseEvent.dwEventFlags == MOUSEEVENTF_MOVE) {

                        pos = event_buffer.Event.MouseEvent.dwMousePosition;

                        for(auto &btn : btns) {
                            if(pos.X > btn.x && pos.X < (btn.x + btn.width) && pos.Y > btn.y && pos.Y < (btn.y + 3)) {
                                drawButton(it, btn, true);
                            } else {
                                drawButton(it, btn, false);
                            }
                        }

                    }
                    break;

                case KEY_EVENT:
                    if(event_buffer.Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)
                        return;
                    break;

                case WINDOW_BUFFER_SIZE_EVENT:          //The cursor appears again whenever the window gets resized
                    ShowConsoleCursor(it, false);
            }
        }
    }

}

int main() {

    Interface itf{};

    itf.AcquireHandles();

    DWORD in_mode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    SetConsoleMode(itf.h_in, in_mode);

    ShowConsoleCursor(&itf, false);

    vector<Button> buttons;

    buttons.emplace_back(Button(10, 5, 20, "TEST1", btn1_callback));
    buttons.emplace_back(Button(40, 5, 20, "TEST2", btn2_callback));
    buttons.emplace_back(Button(25, 15, 36, "Test button 3"));

    draw_loop(&itf, buttons);

    getchar();

    return 0;
}

#include <windows.h>
#include <windef.h>
#include <wincon.h>
#include <stdexcept>

#include "Interface.h"

using namespace std;

void Interface::AcquireHandles() {

    h_out = GetStdHandle(STD_OUTPUT_HANDLE);
    h_in = GetStdHandle(STD_INPUT_HANDLE);

    if(h_out == INVALID_HANDLE_VALUE || h_in == INVALID_HANDLE_VALUE) {
        throw runtime_error("Unable to acquire console handles");
    }
}

void Interface::SetPosition(short x, short y) {

    if(h_out != NULL && h_out != INVALID_HANDLE_VALUE) {
        COORD pos{x, y};
        SetConsoleCursorPosition(h_out, pos);
    }
}

void Interface::invertColors() {
    if(color <= 0xf) {
        color *= 0x10;
        SetConsoleTextAttribute(h_out, color);
    } else {
        color /= 0x10;
        SetConsoleTextAttribute(h_out, color);
    }
}

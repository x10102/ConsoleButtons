#ifndef CONMOUSE_INTERFACE_H
#define CONMOUSE_INTERFACE_H

typedef void* HANDLE;
typedef unsigned short WORD;

class Interface {

    WORD color = 0x7;

public:
    HANDLE h_out;
    HANDLE h_in;
    void AcquireHandles();
    void SetPosition(short x, short y);
    void invertColors();
};

#endif //CONMOUSE_INTERFACE_H

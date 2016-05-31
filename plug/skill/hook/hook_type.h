#ifndef _HOOK_TYPE_H_
#define _HOOK_TYPE_H_

class MessageListening
{
public:
    virtual ~MessageListening() {}
   
    virtual void ProcessMessage(HWND hwnd, UINT msg, WPARAM w, LPARAM l) = 0;
};


#endif
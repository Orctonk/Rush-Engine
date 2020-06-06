#ifndef __RUSH_ENTRY_H__
#define __RUSH_ENTRY_H__
#include "Application.h"

extern Rush::Application* Rush::CreateApplication();

namespace Rush{
    bool Init();
    void Exit();
}

int main(int argc, char** argv){
    Rush::Init();
    auto app = Rush::CreateApplication();
    app->Run();
    delete app;
    Rush::Exit();
}
#endif


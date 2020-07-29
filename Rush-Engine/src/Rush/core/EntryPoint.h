#ifndef __RUSH_ENTRY_H__
#define __RUSH_ENTRY_H__
#include "Application.h"

extern Rush::Application* Rush::CreateApplication();

int main(int argc, char** argv){
    RUSH_PROFILE_FUNCTION();
    auto app = Rush::CreateApplication();
    app->Run();
    delete app;
}

#endif


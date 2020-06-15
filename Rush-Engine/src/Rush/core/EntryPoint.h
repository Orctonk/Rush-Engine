#ifndef __RUSH_ENTRY_H__
#define __RUSH_ENTRY_H__
#include "Application.h"

extern Rush::Application* Rush::CreateApplication();

int main(int argc, char** argv){
    auto app = Rush::CreateApplication();
    app->Run();
    delete app;
}

#endif


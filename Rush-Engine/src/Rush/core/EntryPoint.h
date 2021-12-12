#ifndef __RUSH_ENTRY_H__
#define __RUSH_ENTRY_H__
#include "Application.h"

extern Rush::Application *Rush::CreateApplication();

int main(int argc, char **argv) {
    RUSH_PROFILE_FUNCTION();
    // TODO: Command line parsing
    Rush::Path executable(argv[0]);
    Rush::Path::SetCWD(executable.GetParentDirectory().GetParentDirectory());
    std::cout << Rush::Path::GetCWD().GetRawPath() << std::endl;
    auto app = Rush::CreateApplication();
    app->Run();
    delete app;
}

#endif

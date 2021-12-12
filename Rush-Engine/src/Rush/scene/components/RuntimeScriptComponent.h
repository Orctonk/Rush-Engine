#ifndef __RUNTIMESCRIPTCOMPONENT_H__
#define __RUNTIMESCRIPTCOMPONENT_H__

#include <string>
#include <mono/metadata/object.h>

struct RuntimeScriptComponent {
    MonoObject *behaviourInstance;
};

#endif // __RUNTIMESCRIPTCOMPONENT_H__

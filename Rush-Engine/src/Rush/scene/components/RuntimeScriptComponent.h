#ifndef __RUNTIMESCRIPTCOMPONENT_H__
#define __RUNTIMESCRIPTCOMPONENT_H__

#include <mono/metadata/object.h>
#include <string>

struct RuntimeScriptComponent {
    MonoObject *behaviourInstance;
};

#endif // __RUNTIMESCRIPTCOMPONENT_H__

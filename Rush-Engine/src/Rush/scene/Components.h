#ifndef __COMPONENTS_H__
#define __COMPONENTS_H__

#include "components/CameraComponent.h"
#include "components/LightComponent.h"
#include "components/MeshRendererComponent.h"
#include "components/ParticleComponent.h"
#include "components/ScriptComponent.h"
#include "components/TagComponent.h"
#include "components/TransformComponent.h"

#ifdef RUSH_BUILD
    #include "components/RuntimeScriptComponent.h"
#endif

#endif // __COMPONENTS_H__

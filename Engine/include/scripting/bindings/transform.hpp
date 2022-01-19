#pragma once
#include <squirrel.h>
#include <sqstdio.h>
#include <sqrat.h>

namespace Engine{
    namespace Scripting{
        namespace Bindings{
            namespace Transform{
                void Bind(HSQUIRRELVM sq_vm);
            }
        }
    }
}
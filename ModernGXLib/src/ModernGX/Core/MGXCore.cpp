#include "ModernGX.h"
#include "MGXCore.h"

#include <ModernGX/Core/Serialisation/SerPipelineState.h>

void MGX::Core::Init() noexcept
{
    // Init core subsystems
    Serialisation::PipelineState::Init();
}

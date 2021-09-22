#include "ModernGX.h"
#include "IAllocator.h"

// Instance of allocator
MGX::Core::StdAllocator MGX::Core::StdAllocator::s_instance;

void* MGX::Core::StdAllocator::Allocate(size_t size) noexcept
{
    return malloc(size);
}

void MGX::Core::StdAllocator::Free(void* memory) noexcept
{
    free(memory);
}

MGX::Core::StdAllocator* MGX::Core::StdAllocator::Instance() noexcept
{
    return &s_instance;
}

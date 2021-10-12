#include "ModernGX.h"
#include "GTypeBuffer.h"

MGX::Core::GType::MappedBuffer::MappedBuffer(ID3D12Resource* ptrResource, UINT64 size, UINT64 offset)
{
    if (!Map(ptrResource, size, offset))
    {
        throw std::exception("Failed to map resource. Call to MappedBuffer::MappedBuffer(...)");
    }
}

MGX::Core::GType::MappedBuffer::~MappedBuffer()
{
    // Make shure to unmap
    Unmap();
}

bool MGX::Core::GType::MappedBuffer::Map(ID3D12Resource* ptrResource, UINT64 size, UINT64 offset)
{
    // Desribe range
    D3D12_RANGE mapRange = {};
    mapRange.Begin = offset;
    mapRange.End = offset + size;

    // Map and store
    bool mapped = SUCCEEDED(ptrResource->Map(0, &mapRange, (void**)&m_ptrData));
    m_mappedSize = size;
    m_mappedOffset = offset;
    m_ptrResource = mapped ? ptrResource : nullptr;
    
    return mapped;
}

void MGX::Core::GType::MappedBuffer::Unmap()
{
    if (m_ptrResource)
    {
        // Describe used range
        D3D12_RANGE usedRange;
        usedRange.Begin = m_mappedOffset;
        usedRange.End = m_mappedOffset + m_mappedSize;

        // Unmap and unset
        m_ptrResource->Unmap(0, &usedRange);
        m_ptrResource = nullptr;
        m_ptrData = nullptr;
        m_mappedSize = 0;
    }
}

bool MGX::Core::GType::MappedBuffer::CopyTo(const void* ptrSource, UINT64 size, UINT64 offset)
{
    bool canCopy = this->IsValid() && size + offset < m_mappedSize;

    if (canCopy)
    {
        memcpy(&m_ptrData[m_mappedOffset + offset], ptrSource, size);
    }

    return canCopy;
}

bool MGX::Core::GType::MappedBuffer::CopyFrom(void* ptrDest, UINT64 size, UINT64 offset)
{
    bool canCopy = this->IsValid() && size + offset < m_mappedSize;

    if (canCopy)
    {
        memcpy(ptrDest, &m_ptrData[m_mappedOffset + offset], size);
    }

    return canCopy;
}

MGX::Core::GType::MappedBuffer& MGX::Core::GType::MappedBuffer::operator=(MappedBuffer&& other) noexcept
{
    // Unmap old data
    Unmap();
    
    // Copy new data
    m_ptrResource = other.m_ptrResource;
    m_ptrData = other.m_ptrData;
    m_mappedSize = other.m_mappedSize;
    m_mappedOffset = other.m_mappedOffset;

    // Unset other
    other.m_ptrResource = nullptr;
    other.m_ptrData = nullptr;
    other.m_mappedSize = 0;

    return *this;
}

unsigned char& MGX::Core::GType::MappedBuffer::operator[](unsigned int index)
{
    // Range check
    if (index >= m_mappedSize)
    {
        throw std::exception("Index out of range! Call to MappedBuffer::operator[]");
    }

    // Return
    return m_ptrData[m_mappedOffset + index];
}

MGX::Core::GType::Buffer::Buffer(ID3D12Device* ptrDevice, GPU::HeapUsage bufferUsage, UINT64 bufferSize, D3D12_HEAP_FLAGS heapFlags)
{
    // Create resource desc
    D3D12_RESOURCE_DESC rd = CreateResourceDesc(bufferSize);

    // Call Resource (parent) construct
    this->MGX::Core::GPU::Resource::Resource(ptrDevice, bufferUsage, &rd, D3D12_RESOURCE_STATE_COMMON, nullptr, heapFlags);

    // Get base address
    m_baseAddress = m_ptrBase->GetGPUVirtualAddress();
    m_bufferSize = bufferSize;
}

MGX::Core::GType::Buffer::Buffer(ID3D12Device* ptrDevice, GPU::HeapAllocationCookie allocationCookie, UINT64 bufferSize)
{
    // Create resource desc
    D3D12_RESOURCE_DESC rd = CreateResourceDesc(bufferSize);

    // Call Resource (parent) construct
    this->MGX::Core::GPU::Resource::Resource(ptrDevice, &rd, allocationCookie);

    // Get base address
    m_baseAddress = m_ptrBase->GetGPUVirtualAddress();
    m_bufferSize = bufferSize;
}

MGX::Core::GType::Buffer& MGX::Core::GType::Buffer::operator=(Buffer&& other) noexcept
{
    // Destruct this
    this->~Buffer();

    // Copy from other
    m_baseAddress = other.m_baseAddress;
    m_bufferSize = other.m_bufferSize;
    m_ptrBase = other.m_ptrBase;

    // Invalidate other object
    other.m_ptrBase = nullptr;
    other.m_bufferSize = 0;
    other.m_baseAddress = 0;
}

MGX::Core::GType::MappedBuffer MGX::Core::GType::Buffer::Map(UINT64 size, UINT64 offset)
{
    // Alight size
    if (size == UINT64_MAX && offset == 0)
        size = m_bufferSize;

    // Map base resource
    return MappedBuffer(m_ptrBase, size, offset);
}

bool MGX::Core::GType::Buffer::CreateCBV(ID3D12Device* ptrDevice, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 size, UINT64 offset)
{
    bool canCreateView = offset + size < m_bufferSize;

    if (canCreateView)
    {
        // Describe view
        D3D12_CONSTANT_BUFFER_VIEW_DESC cvd = {};
        cvd.BufferLocation = this->operator[](offset);
        cvd.SizeInBytes = size;

        // Create SRV
        ptrDevice->CreateConstantBufferView(&cvd, handle);
    }

    return canCreateView;
}

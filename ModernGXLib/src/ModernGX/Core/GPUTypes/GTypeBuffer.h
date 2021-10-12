#pragma once

#include <ModernGX.h>
#include <ModernGX/Core/GPU/GPUHeap.h>
#include <ModernGX/Core/GPU/GPUResource.h>
#include <ModernGX/Core/GPU/GPUDescriptorHeap.h>

#include <exception>

namespace MGX::Core::GType
{
    // GPU Mapped Buffer
    class MappedBuffer
    {
        public:
            // Construct
            MappedBuffer() = default;
            MappedBuffer(const MappedBuffer&) = delete;
            MappedBuffer(ID3D12Resource* ptrResource, UINT64 size, UINT64 offset = 0);
            
            // Destruct
            ~MappedBuffer();

            // Unmap this resource
            void Unmap();

            // Copy TO this buffer
            bool CopyTo(const void* ptrSource, UINT64 size, UINT64 offset = 0);
            // Copy FROM this buffer
            bool CopyFrom(void* ptrDest, UINT64 size, UINT64 offset = 0);

            // Template copy to
            template<typename T>
            inline bool TCopyTo(const T* ptrSource, UINT count, UINT64 byteOffset = 0)
            {
                return CopyTo(ptrSource, count * sizeof(T), byteOffset);
            }

            // Template copy from
            template<typename T>
            inline bool TCopyFrom(T* ptrDest, UINT count, UINT64 byteOffset = 0)
            {
                return CopyFrom(ptrDest, count * sizeof(T), byteOffset);
            }

            // No assign
            MappedBuffer& operator=(const MappedBuffer&) = delete;

            // Allow move
            MappedBuffer& operator=(MappedBuffer&& other) noexcept;

            // Get Pointer
            inline void* Ptr() noexcept
            {
                return &m_ptrData[m_mappedOffset];
            }

            // Get size
            inline UINT64 Size() const noexcept
            {
                return m_mappedSize;
            }

            // Valid check
            inline bool IsValid() const noexcept
            {
                return m_ptrData != nullptr;
            }

            // Boolean valid check
            inline operator bool() const noexcept
            {
                return IsValid();
            }

            // Array operator
            unsigned char& operator[](unsigned int index);

        private:
            // Map this resource
            bool Map(ID3D12Resource* ptrResource, UINT64 size, UINT64 offset = 0);

        private:
            // Ref to resource
            ComPointer<ID3D12Resource> m_ptrResource;

            // Data pointer
            unsigned char* m_ptrData = nullptr;
            // Mapped area
            UINT64 m_mappedSize = 0, m_mappedOffset = 0;
    };

    // GPU Buffer
    class Buffer : public GPU::Resource
    {
        public:
            // Construct
            Buffer() = default;
            Buffer(const Buffer&) = delete;
            Buffer(ID3D12Device* ptrDevice, GPU::HeapUsage bufferUsage, UINT64 bufferSize, D3D12_HEAP_FLAGS heapFlags = D3D12_HEAP_FLAG_NONE);
            Buffer(ID3D12Device* ptrDevice, GPU::HeapAllocationCookie allocationCookie, UINT64 bufferSize);

            // No copy supported
            Buffer& operator=(const Buffer&) = delete;

            // Move assign
            Buffer& operator=(Buffer&& other) noexcept;
            
            // Mapping functions
            MappedBuffer Map(UINT64 size = UINT64_MAX, UINT64 offset = 0);

            // Create Views
            bool CreateCBV(ID3D12Device* ptrDevice, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT size = UINT_MAX, UINT64 offset = 0);
            bool CreateSRV(ID3D12Device* ptrDevice, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT stride, UINT count = UINT_MAX, UINT64 offset = 0, bool raw = false);
            bool CreateVBV(D3D12_VERTEX_BUFFER_VIEW* ptrView, UINT stride, UINT count, UINT64 offset = 0);
            bool CreateIBV(D3D12_INDEX_BUFFER_VIEW* ptrView, UINT indexCount, UINT64 offset = 0, UINT indexByteSize = 4);

            // Helper for view cration with pairs
            inline bool CreateCBV(ID3D12Device* ptrDevice, GPU::DescriptorPair pair, UINT size = UINT_MAX, UINT64 offset = 0)
            {
                return CreateCBV(ptrDevice, pair.cpu, size, offset);
            }
            inline bool CreateSRV(ID3D12Device* ptrDevice, GPU::DescriptorPair pair, UINT stride, UINT count = UINT_MAX, UINT64 offset = 0, bool raw = false)
            {
                return CreateSRV(ptrDevice, pair.cpu, stride, count, offset, raw);
            }

            // Templated SRV
            template<typename T>
            inline bool CreateSRV(ID3D12Device* ptrDevice, D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT count = UINT_MAX, UINT64 byteOffset = 0, bool raw = false)
            {
                return CreateSRV(ptrDevice, handle, sizeof(T), count, byteOffset, raw);
            }
            template<typename T>
            inline bool CreateSRV(ID3D12Device* ptrDevice, GPU::DescriptorPair pair, UINT count = UINT_MAX, UINT64 byteOffset = 0, bool raw = false)
            {
                return CreateSRV(ptrDevice, pair, sizeof(T), count, byteOffset, raw);
            }

            // Template VBV
            template<typename T>
            inline bool CreateVBV(D3D12_VERTEX_BUFFER_VIEW* ptrView, UINT count, UINT byteOffset = 0)
            {
                return CreateVBV(ptrView, sizeof(T), count, byteOffset);
            }

            // Get size of bufer
            inline UINT64 Size() const noexcept
            {
                return m_bufferSize;
            }

            // Get GPU virtual address
            inline D3D12_GPU_VIRTUAL_ADDRESS operator[](UINT64 offset) noexcept
            {
                return m_baseAddress + offset;
            }

        private:
            // Resource description crafting
            static constexpr D3D12_RESOURCE_DESC CreateResourceDesc(UINT64 bufferSize)
            {
                // Describe buffer resource
                D3D12_RESOURCE_DESC rd = {};
                rd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
                rd.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
                rd.Width = bufferSize;
                rd.Height = 1;
                rd.DepthOrArraySize = 1;
                rd.MipLevels = 1;
                rd.Format = DXGI_FORMAT_UNKNOWN;
                rd.SampleDesc.Count = 1;
                rd.SampleDesc.Quality = 0;
                rd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
                rd.Flags = D3D12_RESOURCE_FLAG_NONE;
                return rd;
            }

        private:
            // Base GPU address
            D3D12_GPU_VIRTUAL_ADDRESS m_baseAddress = 0;

            // Size of buffer
            UINT64 m_bufferSize = 0;
    };
}

#pragma once

#include <ModernGX.h>
#include <ModernGX/Foundation/COMGetable.h>
#include <ModernGX/Foundation/HRException.h>

#include <exception>
#include <type_traits>
#include <initializer_list>

namespace MGX::Core::GPU 
{
    // Command queue
    class CommandList : public Foundation::COMGetable<ID3D12GraphicsCommandList> 
    {
        public:
            // Construct
            CommandList() = default;
            CommandList(const CommandList&) = delete;
            CommandList(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);

            // Copy
            CommandList& operator=(const CommandList&) = delete;

            // Assign move
            CommandList& operator=(CommandList&& other) noexcept;

            // Close and reset command list
            void Close() noexcept;
            void Reset() noexcept;
            bool IsOpen() noexcept;

            // Retrive type of command list
            inline D3D12_COMMAND_LIST_TYPE GetType() const noexcept
            {
                return m_type;
            }

            // Peek next barrier
            inline D3D12_RESOURCE_BARRIER* ResourceBarrierPeek() noexcept
            {
                return &m_resBarriers[m_barrierUsage];
            }

            // Incrment barrieren usage
            inline void ResourceBarrierPush() noexcept
            {
                m_barrierUsage++;
            }

            // Peek next barrier and push it (accept usage)
            inline D3D12_RESOURCE_BARRIER* ResourceBarrierPeekAndPush() noexcept
            {
                return &m_resBarriers[m_barrierUsage++];
            }

            // Flush ressource barriere execution
            void ResourceBarrierFlush() noexcept;

            // Name command list
            HRESULT name(LPCWSTR name) override;

            // === COMMANDS ===

            // Viewport or/and Sicor Rect
            void RSSetViewportAndRect(D3D12_VIEWPORT* viewPort, D3D12_RECT* rect);
            // Format: count [viewPort, rect]...
            void RSSetViewportAndRect(unsigned int count ...);

            // RTV Clearing
            void ClearRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE rtView, FLOAT clearValue[4] = nullptr);
            void ClearDepthStencil(D3D12_CPU_DESCRIPTOR_HANDLE dsView, FLOAT depth = 1.0f, UINT8 stencil = 0x00);

            // Set RTV / DSV
            void OMSetRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtView);
            void OMSetRenderTargetViews(D3D12_CPU_DESCRIPTOR_HANDLE dsView, D3D12_CPU_DESCRIPTOR_HANDLE rtView);
            template<typename... T, typename = std::disjunction<std::is_same<T, D3D12_CPU_DESCRIPTOR_HANDLE>...>>
            void OMSetRenderTargetViews(D3D12_CPU_DESCRIPTOR_HANDLE dsView, T... rtViews)
            {
                static_assert(sizeof...(T) > 0 && sizeof...(T) <= 8, "Number of RTVs must be 1...8!");

                // Target buffers
                D3D12_CPU_DESCRIPTOR_HANDLE rtViews[8];
                unsigned int rtViewsCount = 0;

                // Aquire using initializer list
                std::initializer_list<D3D12_CPU_DESCRIPTOR_HANDLE> list({ rtViews... });
                for (auto it = list.begin(); it != list.end(); it++)
                {
                    rtViews[rtViewsCount++] = *it;
                }

                m_ptrBase->OMSetRenderTargets(rtViewsCount, rtViews, FALSE, dsView.ptr ? &dsView : nullptr);
            }

            // Combined RT and clear
            inline void OMPrepareRtDsViews(D3D12_CPU_DESCRIPTOR_HANDLE rtView, D3D12_CPU_DESCRIPTOR_HANDLE dsView = { 0 }, FLOAT rtClearValue[4] = nullptr, FLOAT depth = 1.0f, UINT8 stencil = 0x00)
            {
                ClearRenderTarget(rtView, rtClearValue);
                if(dsView.ptr)
                    ClearDepthStencil(dsView, depth, stencil);
                OMSetRenderTargetViews(dsView, rtView);
            }

            // IA Set Topology and buffer
            void IASetBuffer(D3D12_VERTEX_BUFFER_VIEW* vbView, D3D12_PRIMITIVE_TOPOLOGY topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            void IASetBuffer(D3D12_VERTEX_BUFFER_VIEW* vbView, D3D12_INDEX_BUFFER_VIEW* ibView, D3D12_PRIMITIVE_TOPOLOGY topology = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            
            // Set heaps
            inline void SetHeaps(ID3D12DescriptorHeap* ptrHeap, ID3D12DescriptorHeap* ptrHeap2 = nullptr)
            {
                ID3D12DescriptorHeap* heaps[] = { ptrHeap, ptrHeap2 };
                m_ptrBase->SetDescriptorHeaps(ptrHeap2 ? 2 : 1, heaps);
            }

            // Draw
            inline void Draw(UINT vertexPerInstanceCount, UINT instanceCount = 1, UINT startVertexOffset = 0, UINT startInstanceOffset = 0)
            {
                // Flush and draw
                ResourceBarrierFlush();
                m_ptrBase->DrawInstanced(vertexPerInstanceCount, instanceCount, startVertexOffset, startInstanceOffset);
            }
            inline void DrawIndexed(UINT indexPerInstanceCount, UINT instanceCount = 1, UINT startIndexOffset = 0, INT baseVertexOffset = 0, UINT startInstanceOffset = 0)
            {
                // Flush and draw
                ResourceBarrierFlush();
                m_ptrBase->DrawIndexedInstanced(indexPerInstanceCount, instanceCount, startIndexOffset, baseVertexOffset, startInstanceOffset);
            }



        private:
            // Command allocator
            ComPointer<ID3D12CommandAllocator> m_ptrAllocator;

            // Resource Barrieres
            D3D12_RESOURCE_BARRIER m_resBarriers[256];
            UINT m_barrierUsage = 0;

            // Type of commands list
            D3D12_COMMAND_LIST_TYPE m_type = D3D12_COMMAND_LIST_TYPE_DIRECT;

            // Indicates command list state
            bool m_bOpen = true;
    };
}

#include "ModernGX.h"
#include "GPUQueue.h"

MGX::Core::GPU::CommandQueue::CommandQueue(ID3D12Device* ptrDevice, D3D12_COMMAND_LIST_TYPE type, D3D12_COMMAND_QUEUE_PRIORITY prio) 
{
    // Describe command queue
    D3D12_COMMAND_QUEUE_DESC qd = {};
    qd.Type = type;
    qd.Priority = prio;
    qd.NodeMask = NULL;
    qd.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    // Create queue
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateCommandQueue(...)", ptrDevice->CreateCommandQueue(&qd, IID_PPV_ARGS(&m_ptrBase)));

    // Create fence
    MGX_EVALUATE_HRESULT("ID3D12Device->CreateFence(...)", ptrDevice->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_ptrFence)));

    // Create event
    m_waitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

MGX::Core::GPU::CommandQueue::~CommandQueue() 
{
    // Wait if valid
    if (m_ptrBase) Wait();
    
    // Destroy event
    if (m_waitEvent != INVALID_HANDLE_VALUE) CloseHandle(m_waitEvent);
}

MGX::Core::GPU::CommandQueue& MGX::Core::GPU::CommandQueue::operator=(CommandQueue&& other) noexcept 
{
    // Release this
    this->~CommandQueue();

    // Copy from other
    m_ptrBase = other.m_ptrBase;
    m_type = other.m_type;
    m_ptrFence = other.m_ptrFence;
    m_fenceValue = other.m_fenceValue;
    m_waitEvent = other.m_waitEvent;

    // Clear other
    other.m_waitEvent = INVALID_HANDLE_VALUE;
    other.m_ptrBase = nullptr;
    other.m_ptrFence = nullptr;
    other.m_fenceValue = 0;

    // Return
    return *this;
}

UINT64 MGX::Core::GPU::CommandQueue::Execute(ID3D12CommandList* ptrCommandList) noexcept 
{
    // Pack in array
    ID3D12CommandList* arr[] = { ptrCommandList };
    
    // Execute and signal
    m_ptrBase->ExecuteCommandLists(1, arr);
    m_ptrBase->Signal(m_ptrFence, ++m_fenceValue);

    // Return fence value
    return m_fenceValue;
}

void MGX::Core::GPU::CommandQueue::Wait(UINT64 value) noexcept 
{
    // Adjust fence value for default argument
    if (value == UINT64_MAX) value = m_fenceValue;

    // Check if wait is required
    if (m_ptrFence->GetCompletedValue() < value) 
    {
        // Check and ready event
        if (m_waitEvent != INVALID_HANDLE_VALUE && ResetEvent(m_waitEvent)) 
        {
            // Signal event
            if (SUCCEEDED(m_ptrFence->SetEventOnCompletion(value, m_waitEvent))) 
            {
                // Wait for event
                if (WaitForSingleObject(m_waitEvent, INFINITE) == WAIT_OBJECT_0) 
                {
                    // Wait done
                    return;
                }
            }
        }

        // Naive waite
        while (m_ptrFence->GetCompletedValue() < value);
    }
}

void MGX::Core::GPU::CommandQueue::Flush(unsigned int count) noexcept 
{
    for (unsigned int i = 0; i < count; i++) 
    {
        // Signal queue
        m_ptrBase->Signal(m_ptrFence, ++m_fenceValue);
        // Wait
        Wait();
    }
}

HRESULT MGX::Core::GPU::CommandQueue::name(LPCWSTR name)
{
    HRESULT hr;
    if (FAILED(hr = m_ptrBase.name(name))) return hr;
    if (FAILED(hr = m_ptrFence.name(name))) return hr;
    return hr;
}

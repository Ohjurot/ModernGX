#pragma once

#include <type_traits>

// Com pointer class
template<typename CT>
class ComPointer {
	public:
		// Default empty constructor
		ComPointer() = default;

		// Construct by raw pointer (add ref)
		ComPointer(CT* pointer) {
			// Clear internal pointer
			clearPointer();

			// Add ref and set
			setPointerAddRef(pointer);
		}

		// Construct by other compointer
		ComPointer(const ComPointer<CT>& other) {
			// Clear internal pointer
			clearPointer();

			// Set and add ref
			setPointerAddRef(other.m_pointer);
		}

		// Destructor
		~ComPointer() {
			// Clear pointer
			clearPointer();
		}

		// Naming
		template<typename T = CT, typename = std::enable_if_t<std::is_base_of_v<ID3D12Object, T>>>
		HRESULT name(LPCWSTR name) {
			return m_pointer ? m_pointer->SetName(name) : E_FAIL;
		}

		// Release the pointer
		unsigned long release() {
			return clearPointer();
		}

		// Get reference (add ref)
		CT* getRef() {
			// Only when pointer exists
			if (m_pointer) {
				m_pointer->AddRef();
				return m_pointer;
			}

			// Fallback
			return nullptr;
		}

		// Get pointer (NO add ref)
		CT* get() {
			// Return pointer
			return m_pointer;
		}

		// Query interface function
		template<typename T>
		bool queryInterface(ComPointer<T>& other) {
			// Only on valid pointer
			if (m_pointer) {
				return m_pointer->QueryInterface(IID_PPV_ARGS(&other)) == S_OK;
			}

			// Fallback
			return false;
		}

		// Assign operator
		ComPointer<CT>& operator=(const ComPointer<CT>& other) {
			// Clear pointer
			clearPointer();

			// Set
			setPointerAddRef(other.m_pointer);
			return *this;
		}

		// Assign operator
		ComPointer<CT>& operator=(CT* other) {
			// Clear pointer
			clearPointer();

			// Set
			setPointerAddRef(other);
			return *this;
		}

		// Compare pointer
		bool operator==(const ComPointer<CT>& other) {
			return m_pointer == other.m_pointer;
		}

		// Compare pointer
		bool operator==(const CT* other) {
			return m_pointer == other;
		}

		// Pointer operator
		CT* operator->() {
			return m_pointer;
		}

		// Address of operator
		CT** operator&() {
			return &m_pointer;
		}

		// Auto conversion
		operator bool() {
			return m_pointer != nullptr;
		}

		// Implicit opperator CT
		operator CT* () {
			return m_pointer;
		}


	private:
		// Inline pointer clear function
		inline unsigned long clearPointer() {
			unsigned long newRef = 0;
			
			// Only on valid pointer
			if (m_pointer) {
				// Release and unset
				newRef = m_pointer->Release();
				m_pointer = nullptr;
			}

			return newRef;
		}

		// Inline set pointer and add ref call
		inline void setPointerAddRef(CT* pointer) {
			// Assign and add ref
			m_pointer = pointer;
			if (m_pointer) {
				m_pointer->AddRef();
			}
		}

	private:
		// Pointer container
		CT* m_pointer = nullptr;
};

#pragma once
#include <ModernGX.h>

#include <exception>
#include <string>
#include <sstream>
#include <cstring>

#define MGX_EVALUATE_HRESULT(what, call) {HRESULT hr = call; if(FAILED(hr)) throw MGX_HR_EXCEPTION(what, hr); }
#define MGX_HR_EXCEPTION(what, hr) MGX::Foundation::HRException(__FILE__, __LINE__, what, hr)

namespace MGX::Foundation {
    // HRESULT Exception class
    class HRException : public std::exception {
        public:
            // Construct
            HRException() noexcept = default;
            HRException(const HRException&) noexcept = default;
            HRException(const char* file, int line, const char* what, HRESULT result);

            // Override what
            const char* what() const noexcept override;

            // Show exeception to user (MessageBox)
            void show() const noexcept;
        private:
            // Exception text
            std::string m_text;
    };
}

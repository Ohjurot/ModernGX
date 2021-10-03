#pragma once

#include <ModernGX.h>
#include <ModernGX/Util/Hash.h>

namespace MGX::Serialisation
{
    // String hash table object
    template<typename T>
    struct StringHashTable
    {
        // Source text
        const char* text;
        // Hash value
        UINT64 hash = 0;
        // Target value
        T value;

        // Gennerate a table entrie
        static constexpr StringHashTable Entrie(const char* text, T value)
        {
            return { text, 0, value };
        }

        // Gennerate a tabel terminator
        static constexpr StringHashTable Terminator()
        {
            return { nullptr, 0, T() };
        }
    };

    // Hash table class
    template<typename T>
    class StringHashTableOps
    {
        public:
            // No instance
            StringHashTableOps() = delete;
            StringHashTableOps(const StringHashTableOps&) = delete;
            StringHashTableOps& operator=(const StringHashTableOps&) = delete;

            // Operations
            static void GennerateHashs(StringHashTable<T>* ptrTableBegin)
            {
                // Go over list
                StringHashTable<T>* ptrHead = ptrTableBegin;
                while (ptrHead->text != nullptr)
                {
                    // Hash current text
                    ptrHead->hash = Util::Hash64Bit(ptrHead->text, strlen(ptrHead->text));

                    // Next element
                    ptrHead++;
                }
            }
            
            // Search or return default value
            static T SearchOrDefault(StringHashTable<T>* ptrTableBegin, const char* key, T defaultValue)
            {
                return SearchOrDefault(ptrTableBegin, Util::Hash64Bit(key, strlen(key)), defaultValue);
            }
            static T SearchOrDefault(StringHashTable<T>* ptrTableBegin, UINT64 hash, T defaultValue)
            {
                // Init with default and do normal search
                T value = defaultValue;
                Search(ptrTableBegin, hash, &value);
                return value;
            }
            
            // Search and set
            static bool Search(StringHashTable<T>* ptrTableBegin, const char* key, T* ptrOut)
            {
                return Search(ptrTableBegin, Util::Hash64Bit(key, strlen(key)), ptrOut);
            }
            static bool Search(StringHashTable<T>* ptrTableBegin, UINT64 hash, T* ptrOut)
            {
                // Outer search loop
                StringHashTable<T>* ptrHead = ptrTableBegin;
                while (ptrHead->text != nullptr)
                {
                    // Check for match
                    if (ptrHead->hash == hash)
                    {
                        // Set output value
                        *ptrOut = ptrHead->value;
                        // Found
                        return true;
                    }

                    // Next element
                    ptrHead++;
                }

                // Nothing found return false
                return false;
            }

    };
}

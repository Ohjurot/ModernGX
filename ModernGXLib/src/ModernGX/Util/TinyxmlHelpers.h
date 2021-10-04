#pragma once

#include <ModernGX.h>
#include <ModernGX/Util/StrHashTable.h>

#include <exception>

#ifdef MGX_CUSTOM_TINY_XML_INCLUDE
#include MGX_CUSTOM_TINY_XML_INCLUDE
#else
#include <tinyxml2/tinyxml2.h>
#endif

#define MGX_TINYXML_FOREACH(ptrRootElement, ptrOutName, searchString) auto ptrOutName = ptrRootElement->FirstChildElement(searchString); ptrOutName; ptrOutName = ptrOutName->NextSiblingElement(searchString)

#define __MGX_TINYXML_DECL_XmlQueryOrDefault(type, function) \
template<> \
static type QueryOrDefault<type>(const tinyxml2::XMLAttribute* ptrAtt, type* ptrDest, type defaultValue) \
{ \
    /* Default */\
    *ptrDest = defaultValue; \
\
    /* Query*/ \
    if (ptrAtt) \
        ptrAtt->function(ptrDest);\
\
    /* Return */ \
    return *ptrDest; \
}

namespace MGX::Util
{
    class XML
    {
        public:
            // No OOP 
            XML() = delete;
            XML(const XML&) = delete;
            XML& operator=(const XML&) = delete;

            // Search in str hash table
            template<typename T>
            static T QueryOrDefault(const tinyxml2::XMLAttribute* ptrAtt, T* ptrDest, T defaultValue, ::MGX::Serialisation::StringHashTable<T>* ptrTable)
            {
                // Set default
                *ptrDest = defaultValue;

                // Search with attribut text
                if (ptrAtt)
                {
                    ::MGX::Serialisation::StringHashTableOps<T>::Search(ptrTable, ptrAtt->Value(), ptrDest);
                }

                return *ptrDest;
            }

            // Query or default (no query)
            template<typename T>
            static T QueryOrDefault(const tinyxml2::XMLAttribute* ptrAtt, T* ptrDest, T defaultValue)
            {
                throw std::exception("Template argument not valid. XmlQueryOrDefault() is only avalible for INT32, INT64, UINT32, UINT64, DOUBLE, FLOAT and bool!");
            }

            // Implement microsoft (why?) byte type
            template<>
            static BYTE QueryOrDefault(const tinyxml2::XMLAttribute* ptrAtt, BYTE* ptrDest, BYTE defaultValue)
            {
                // Set default
                UINT32 temp = defaultValue;
                // Query
                if (ptrAtt)
                    ptrAtt->QueryUnsignedValue(&temp);

                // Set
                *ptrDest = temp <= UINT8_MAX ? (BYTE)temp : defaultValue;

                // Return value
                return *ptrDest;
            }

            // Implement functions (strait forward)
            __MGX_TINYXML_DECL_XmlQueryOrDefault(INT32, QueryIntValue);
            __MGX_TINYXML_DECL_XmlQueryOrDefault(INT64, QueryInt64Value);
            __MGX_TINYXML_DECL_XmlQueryOrDefault(UINT32, QueryUnsignedValue);
            __MGX_TINYXML_DECL_XmlQueryOrDefault(UINT64, QueryUnsigned64Value);
            __MGX_TINYXML_DECL_XmlQueryOrDefault(DOUBLE, QueryDoubleValue);
            __MGX_TINYXML_DECL_XmlQueryOrDefault(FLOAT, QueryFloatValue);
            __MGX_TINYXML_DECL_XmlQueryOrDefault(bool, QueryBoolValue);
    };
}

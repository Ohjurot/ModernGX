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
type XmlQueryOrDefault<type>(const tinyxml2::XMLAttribute* ptrAtt, type* ptrDest, type defaultValue) \
{ \
    /* Query / Default */ \
    if (!ptrAtt || ptrAtt->function(ptrDest) != tinyxml2::XMLError::XML_SUCCESS) \
        *ptrDest = defaultValue; \
\
    /* Return */ \
    return *ptrDest; \
}

namespace MGX::Util
{
    // Search in str hash table
    template<typename T>
    T XmlQueryOrDefault(const tinyxml2::XMLAttribute* ptrAtt, T* ptrDest, T defaultValue, ::MGX::Serialisation::StringHashTable<T>* ptrTable)
    {
        T value = defaultValue;
        
        // Search with attribut text
        if (ptrAtt)
        {
            ::MGX::Serialisation::StringHashTableOps<T>::Search(ptrTable, ptrAtt->Value(), &value);
        }

        return value;
    }

    // Query or default (no query)
    template<typename T>
    T XmlQueryOrDefault(const tinyxml2::XMLAttribute* ptrAtt, T* ptrDest, T defaultValue)
    {
        throw std::exception("Template argument not valid. XmlQueryOrDefault() is only avalible for INT32, INT64, UINT32, UINT64, DOUBLE, FLOAT and bool!");
    }

    // Implement microsoft (why?) byte type
    template<>
    BYTE XmlQueryOrDefault(const tinyxml2::XMLAttribute* ptrAtt, BYTE* ptrDest, BYTE defaultValue)
    {
        // Set default
        UINT32 temp = defaultValue;
        // Query
        if (ptrAtt)
            ptrAtt->QueryUnsignedValue(&temp);

        // Return value
        return temp <= UINT8_MAX ? (BYTE)temp : defaultValue;
    }

    // Implement functions (strait forward)
    __MGX_TINYXML_DECL_XmlQueryOrDefault(INT32, QueryIntValue);
    __MGX_TINYXML_DECL_XmlQueryOrDefault(INT64, QueryInt64Value);
    __MGX_TINYXML_DECL_XmlQueryOrDefault(UINT32, QueryUnsignedValue);
    __MGX_TINYXML_DECL_XmlQueryOrDefault(UINT64, QueryUnsigned64Value);
    __MGX_TINYXML_DECL_XmlQueryOrDefault(DOUBLE, QueryDoubleValue);
    __MGX_TINYXML_DECL_XmlQueryOrDefault(FLOAT, QueryFloatValue);
    __MGX_TINYXML_DECL_XmlQueryOrDefault(bool, QueryBoolValue);
}

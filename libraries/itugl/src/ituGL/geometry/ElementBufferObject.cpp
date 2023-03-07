#include <ituGL/geometry/ElementBufferObject.h>

ElementBufferObject::ElementBufferObject()
{
    // Nothing to do here, it is done by the base class
}

Data::Type ElementBufferObject::GetSmallestType(unsigned int vertexCount)
{
    Data::Type elementType = Data::Type::UInt;

    if (vertexCount <= 0xFF)
    {
        elementType = Data::Type::UByte;
    }
    else if (vertexCount <= 0xFFFF)
    {
        elementType = Data::Type::UShort;
    }
    return elementType;
}

#ifndef NDEBUG
bool ElementBufferObject::IsSupportedType(Data::Type type)
{
    switch (type)
    {
    case Data::Type::UByte:
    case Data::Type::UShort:
    case Data::Type::UInt:
        return true;
    default:
        return false;
    }
}
#endif

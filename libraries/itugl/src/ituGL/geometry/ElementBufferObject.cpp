#include <ituGL/geometry/ElementBufferObject.h>

ElementBufferObject::ElementBufferObject()
{
    // Nothing to do here, it is done by the base class
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

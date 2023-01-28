#include <ituGL/core/Data.h>

// Get size in bytes for each Type
unsigned int Data::GetTypeSize(Type type)
{
    switch (type)
    {
    case Type::Byte:
    case Type::UByte:
        return 1;
    case Type::Short:
    case Type::UShort:
    case Type::Half:
        return 2;
    case Type::Double:
        return 8;
    default:
        return 4;
    }
}

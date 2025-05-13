// Disable problematic Windows attributes for Linux
#define __stdcall
#define __declspec(x)

// Completely disable the property system
#define PROPERTY(type, name) type m_##name;
#define ARRAY_PROPERTY(type, name, size) type m_##name[size];

// Include the original header with guards against multiple inclusion
#ifndef ORIGINAL_TSMASTER_INCLUDED
#define ORIGINAL_TSMASTER_INCLUDED
#include "TSMaster.h"
#endif

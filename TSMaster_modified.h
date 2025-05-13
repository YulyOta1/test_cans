// TSMaster_modified.h
#ifndef TSMaster_MODIFIED_H
#define TSMaster_MODIFIED_H

// Disable Windows-specific attributes completely
#undef __stdcall
#define __stdcall

#undef __declspec
#define __declspec(x)

// Completely replace the property system with simple member variables
#undef PROPERTY
#define PROPERTY(type, name) \
    private: type m_##name; \
    public: type get_##name() const { return m_##name; } \
    void set_##name(const type& value) { m_##name = value; }

#undef ARRAY_PROPERTY
#define ARRAY_PROPERTY(type, name, size) \
    private: type m_##name[size]; \
    public: type get_##name(int index) const { return m_##name[index]; } \
    void set_##name(int index, const type& value) { m_##name[index] = value; }

// Include the original header with protection against multiple inclusion
#ifndef ORIGINAL_TSMASTER_INCLUDED
#define ORIGINAL_TSMASTER_INCLUDED
#include "TSMaster.h"
#endif

#endif // TSMaster_MODIFIED_H

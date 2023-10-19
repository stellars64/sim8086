#pragma once
#include <cassert>
#include <cstdint>

#include "common.h"

// operations on 16-bit values that are treated as registers / flags

inline u8
lowbyte(u16 value)
{
	return value & 0x00FF;
}

inline u8
highbyte(u16 value)
{
	return (value & 0xFF00) >> 8;
}

inline void
set_lowbyte(u16& value, u8 low)
{
	value = (value & 0xFF00) | low;
}

inline void
set_highbyte(u16& value, u8 high)
{
	value = (value & 0x00FF) | (high << 8);
}

inline u16
has_flags(u16 value, u16 flags)
{
	return value & flags;
}

inline void
set_flags(u16& value, u16 flags)
{
	value |= flags;
}

inline void
clear_flags(u16& value, u16 flags)
{
	value &= ~flags;
}

inline void
set_flags(u16& value, u16 flags, bool set)
{
	set ? set_flags(value, flags) : clear_flags(value, flags);
}

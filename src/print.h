#pragma once
#include <iostream>
#include "common.h"
#include "sim8086.h"

namespace print {
enum num_mode {
	hex,
	dec
};

struct flags {
	u16 flags;
};

struct reg16 {
	u16 reg;
};

struct ip {
	u16 ip;
};
} // namespace io

std::ostream&
operator<<(std::ostream& os, const print::reg16& reg);

std::ostream&
operator<<(std::ostream& os, const print::flags& flags);

std::ostream&
operator<<(std::ostream& os, const print::ip& ip);

std::ostream& 
operator<<(std::ostream& os, const registers& reg);

std::ostream&
operator<<(std::ostream& os, const cpu& cpu);

std::ostream&
operator<<(std::ostream& os, const operand& var);

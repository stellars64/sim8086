#include "print.h"
#include <iomanip>

static std::ostream&
operator<<(std::ostream& os, const print::num_mode mode)
{
	if (mode == print::hex)
		os << std::hex << std::setfill('0') << std::right;
	else
		os << std::dec << std::setfill(' ') << std::right;
	return os;
}

std::ostream&
operator<<(std::ostream& os, const print::reg16& reg)
{
	const u16& r = reg.reg;
	os << print::dec << std::setw(0) << r << ":"
	   << print::hex << std::setw(4) << r;
	return os;
}

std::ostream&
operator<<(std::ostream& os, const print::flags& flags)
{
	const u16& f = flags.flags;
	for (i32 i = 0; i <= 15; i++)
		if (flag_used.at(i) && has_flags(f, 1 << i))
			os << flag_names.at(i);
	return os;
}

std::ostream&
operator<<(std::ostream& os, const print::ip& ip)
{
	return os << print::hex << std::setw(4) << ip.ip
	   << "_" << print::dec << std::setw(0) << ip.ip;
}

std::ostream&
operator<<(std::ostream& os, const registers& reg)
{
	os << "*---Register Values---*" << std::endl;
	for (i32 i = 0; i <= 7; i++) {
		os << "| " << wordreg_names.at(i) << ": "
		   << print::reg16{reg.get(i, regsize_word)}
		   << "   |" << std::endl;
	}
	return os << "*---------------------*";
}

std::ostream&
operator<<(std::ostream& os, const cpu& cpu)
{
	return os << cpu.reg << std::endl
	   << "flags(" << print::flags{cpu.flags} << ")\t "
	   << "ip("    <<  print::ip{cpu.ip}      << ")" << std::endl;
}

std::ostream&
operator<<(std::ostream& os, const operand& var)
{
	switch (var.index()) {
	case operand_index_reg: {
		auto& [w, reg] = std::get<operand_reg>(var);
		return os << (w ? wordreg_names.at(reg) : bytereg_names.at(reg));
	} case operand_index_effaddr: {
		auto& [w, addr, mod, rm] = std::get<operand_effaddr>(var);
		os << print::dec << "[";
		if (mod == 0b00 && rm == 0b110) {
			os << addr.base;
		} else {
			os << effaddr_names.at(rm);
			if (addr.displacement != 0) {
				if (addr.displacement < 0)
					os << " - " << -addr.displacement;
				else
					os << " + " << addr.displacement;
			}
		}
		return os << "]";
	} case operand_index_addr: {
		return os << print::dec << "[" << std::get<operand_addr>(var).addr
		          << "]";
	} case operand_index_imm: {
		return os << print::dec << std::get<operand_imm>(var).value;
	} default:
		return os;
	}
}


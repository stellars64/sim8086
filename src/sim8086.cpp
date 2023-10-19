#include "sim8086.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <source_location>
#include <variant>

#include "common.h"
#include "dispatch.h"
#include "print.h"

void
reset(memory& mem)
{
	for (auto& b : mem.mem) {
		b = 0;
	}
}

void
reset(registers& reg)
{
	for (auto& r : reg.regs) {
		r = 0;
	}
}

void
reset(cpu& c)
{
	reset(c.reg);
	c.flags = 0;
	c.ip = 0;
	reset(c.mem);
}


void fn_assert(
	bool cond, const char* msg, const std::source_location loc)
{
	if (!cond) {
		std::cerr << loc.file_name() << ":" << loc.line() << " in "
			      << loc.function_name() << std::endl;
		throw std::runtime_error(msg);
	}
}

size_t
get_ip(std::ifstream& fs)
{
	return fs.tellg();
}

void
set_ip(std::ifstream& fs, size_t ip)
{
	fs.seekg(ip);
}

void
offset_ip(std::ifstream& fs, i8 offset)
{
	fs.seekg(offset, std::ios_base::cur);
}

template<typename T>
bool
read(std::ifstream& fs, T& dest, size_t num_bytes)
{
	dest = 0;
	// this works because of little endian-ness
	fs.read((char*)&dest, num_bytes);
	return !!fs;
}

u16
must_read(
	std::ifstream& fs,u8 num_bytes,
	const std::source_location loc)
{
	u16 dest = 0;
	fn_assert(read(fs, dest, num_bytes), "Unexpected EOF", loc);
	return dest;
}

operation
pattern_match(u8 byte, const std::array<pattern, 127>& patterns) {
	for (const auto& p: patterns)
		if ((byte & p.mask) == p.pattern)
			return p.op;

	return operation::invalid;
}

u16
memory::get(size_t a, bool w) const
{
	assert((w && a < mem.size() - 1) || (!w && a < mem.size()));

	if (w)
		return (mem.at(a + 1) << 8) | mem.at(a);
	else
		return mem.at(a);
}

u16
memory::get(address_pair ap, bool w) const
{
	size_t addr = ap.base + ap.displacement;
	return get(addr, w);
}

void
memory::set(size_t a, bool w, u16 val)
{
	assert((w && a < mem.size() - 1) || (!w && a < mem.size()));

	mem.at(a) = val & 0x00FF;
	if (w)
		mem.at(a + 1) = (val & 0xFF00) >> 8;
}

void
memory::set(address_pair ap, bool w, u16 val)
{
	size_t addr = ap.base + ap.displacement;
	set(addr, w, val);
}

u16
registers::get(size_t i, bool w) const
{
	assert(i < regs.size());

	if (w)
		return regs.at(i);
	else if (i >= 4)
		return highbyte(regs.at(i - 4));
	else
		return lowbyte(regs.at(i));
}

void
registers::set(size_t i, bool w, u16 val)
{
	assert(i < regs.size());

	if (w)
		regs.at(i) = val;
	else if (i>= 4)
		set_highbyte(regs.at(i- 4), val);
	else
		set_lowbyte(regs.at(i), val);
}

void
registers::add(size_t i, bool w, u16 val)
{
	set(i, w, get(i, w) + val);
}

u16
registers::effaddr_base(u8 rm, u8 mod) const
{
	assert(rm <= 0b111 && mod <= 0b11);

	switch (rm) {
	case 0b000:
		return get(regidx_bx, regsize_word) + get(regidx_si, regsize_word);
	case 0b001:
		return get(regidx_bx, regsize_word) + get(regidx_di, regsize_word);
	case 0b010:
		return get(regidx_bp, regsize_word) + get(regidx_si, regsize_word);
	case 0b011:
		return get(regidx_bp, regsize_word) + get(regidx_di, regsize_word);
	case 0b100:
		return get(regidx_si, regsize_word);
	case 0b101:
		return get(regidx_di, regsize_word);
	case 0b110:
		if (mod == 0b00)
			return 0;
		else
			return get(regidx_bp, regsize_word);
	case 0b111:
		return get(regidx_bx, regsize_word);
	default:
		assert(false);
	}
}

address_pair
read_effaddr(const registers& reg, u8 mod, u8 rm, std::ifstream& fs)
{
	if (mod == 0b00 && rm == 0b110) {
		return address_pair{must_read(fs, 2), 0};
	} else {
		u16 base = reg.effaddr_base(rm, mod);
		u16 disp = must_read(fs, mod);

		// sign extension
		if (mod == 0b01 && disp >= 128)
			disp |= 0xFF00;

		return address_pair{base, disp};
	}
}

void
print_value_change(u16 old_value, u16 new_value)
{
	std::cout << "value(" << print::reg16{old_value}
	          << "->"     << print::reg16{new_value} << ")";
}

void
print_flag_change(u16 old_flags, u16 new_flags)
{
	std::cout << "flags(" << print::flags{old_flags}
	          << "->"   << print::flags{new_flags} << ")";
}

bool
is_even_parity(u8 val)
{
	bool parity = true;
	for (i32 i = 0; i < 4; i++) {
		if (val & 1)
			parity = !parity;
		val >>= 1;
	}
	return parity;
}

bool
is_add_overflow(u16 lval, u16 rval, u16 res, bool w)
{
	u16 msb = w ? 0x8000 : 0x80;
	return ((lval & msb) == (rval & msb)) && ((lval & msb) != (res & msb));
}

bool
is_sub_overflow(u16 lval, u16 rval, u16 res, bool w)
{
	return is_add_overflow(lval, -rval, res, w);
}

bool
is_add_auxcarry(u8 lval, u8 rval)
{
	return ((lval & 0x0F) + (rval & 0x0F)) & 0x10;
}

bool
is_sub_auxcarry(u8 lval, u8 rval)
{
	return ((lval & 0x0F) - (rval & 0x0F)) & 0x10;
}


//----- operand ---------------------------------------------------------------*

u16
get(operand& o, cpu& cpu)
{
	switch (o.index()) {
	case operand_index_reg: {
		auto [w, reg] = std::get<operand_reg>(o);
		return cpu.reg.get(reg, w);
	} case operand_index_effaddr: {
		auto [w, addr, mod, rm] = std::get<operand_effaddr>(o);
		return cpu.mem.get(addr, w);
		break;
	} case operand_index_addr: {
		auto [w, addr] = std::get<operand_addr>(o);
		return cpu.mem.get(addr, w);
		break;
	} case operand_index_imm: {
		return std::get<operand_imm>(o).value;
		break;
	} default: {
		assert(false);
		return 0;
	}}
}

void
set(operand& o, cpu& cpu, u16 val)
{
	switch (o.index()) {
	case operand_index_reg: {
		auto [w, reg] = std::get<operand_reg>(o);
		cpu.reg.set(reg, w, val);
		break;
	} case operand_index_effaddr: {
		auto [w, addr, mod, rm] = std::get<operand_effaddr>(o);
		cpu.mem.set(addr, w, val);
		break;
	} case operand_index_addr: {
		auto [w, addr] = std::get<operand_addr>(o);
		cpu.mem.set(addr, w, val);
		break;
	} case operand_index_imm: {
		break;
	}}
}

bool
is_word(operand& o)
{
	switch (o.index()) {
	case operand_index_reg:
		return std::get<operand_reg>(o).word;
	case operand_index_effaddr:
		return std::get<operand_effaddr>(o).word;
	case operand_index_addr:
		return std::get<operand_addr>(o).word;
	case operand_index_imm:
		return std::get<operand_imm>(o).word;
	default:
		assert(false);
		return false;
	}
}

void
move(operand& dest, operand& src, cpu& cpu)
{
	u16 lval = get(dest, cpu);
	u16 rval = get(src, cpu);
	set(dest, cpu, rval);
	std::cout << "mov " << dest << ", " << src << "; ";
	print_value_change(lval, rval);
}

void
arithmetic(operand& dest, operand& src, cpu& cpu, u8 op)
{
	u16 lval      = get(dest, cpu);
	u16 rval      = get(src, cpu);
	u16 result    = 0;
	switch (op) {
	case arithmetic_adc:
		rval += !!has_flags(cpu.flags, flag_carry);
	case arithmetic_add:
		result = lval + rval;
		break;
	case arithmetic_sbb:
		rval += !!has_flags(cpu.flags, flag_carry);
	case arithmetic_sub:
	case arithmetic_cmp:
		result = lval - rval;
		break;
	case arithmetic_or:
		result = lval | rval;
		break;
	case arithmetic_and:
		result = lval & rval;
		break;
	case arithmetic_xor:
		result = lval ^ rval;
		break;
	default:
		assert(false);
		return;
	}

	u16 old_flags = cpu.flags;
	bool word = is_word(dest);
	u16 msb   = word ? 0x8000  : 0x80;
	u32 carry = word ? 0x10000 : 0x100;

	switch (op) {
	case arithmetic_adc:
	case arithmetic_add: {
		set_flags(cpu.flags, flag_over,
			is_add_overflow(lval, rval, result,word));
		set_flags(cpu.flags, flag_aux, is_add_auxcarry(lval, rval));
		break;
	} case arithmetic_sbb:
	  case arithmetic_sub:
	  case arithmetic_cmp: {
		set_flags(cpu.flags, flag_over,
			is_sub_overflow(lval, rval, result, word));
		set_flags(cpu.flags, flag_aux, is_sub_auxcarry(lval, rval));
		break;
	} default:
		set_flags(cpu.flags, flag_over, false);
		set_flags(cpu.flags, flag_aux, false);
		break;
	}
	set_flags(cpu.flags, flag_carry, result & carry);
	set_flags(cpu.flags, flag_zero, result == 0);
	set_flags(cpu.flags, flag_sign, result & msb);
	set_flags(cpu.flags, flag_parity, is_even_parity(result));

	// cmp doesn't write to dest
	if (op == arithmetic_cmp)
		result = lval;

	set(dest, cpu, result);

	std::cout << arithmetic_names.at(op) << " " << dest << ", " << src << "; ";
	print_value_change(lval, result);
	std::cout << " ";
	print_flag_change(old_flags, cpu.flags);
}

//----- operand end -----------------------------------------------------------*

void
mem_dump(memory& mem)
{
	std::ofstream fs{"dump.bin", std::ios::binary};
	if (!fs.is_open()) {
		std::cerr << "Could not open dump file" << std::endl;
		return;
	}

	for (const u8 byte : mem.mem) {
		fs << byte;
	}
}

bool
run_program(cpu& cpu, const char* file)
{
	std::ifstream fs{file, std::ios::binary};
	if (!fs.is_open()) {
		std::cerr << "Could not open file: " << file << std::endl;
		return false;
	}

	u8	byte_op = 0;
	u16 prev_ip = 0;

	while (read(fs, byte_op, 1)) {
		operation op = pattern_match(byte_op, patterns);
		if (op == operation::invalid) {
			std::cerr << std::hex << std::setfill('0') << "Invalid byte_op: 0x"
					  << std::setw(2) << static_cast<i32>(byte_op) << std::endl;
			return false;
		} else {
			dispatch_operation(cpu, op, byte_op, fs);
		}

		prev_ip = cpu.ip;
		cpu.ip	= get_ip(fs);

		std::cout << " ip(" << print::reg16{prev_ip}
			      << "->"  << print::reg16{cpu.ip} << ") " << std::endl;
		prev_ip = cpu.ip;
	}
	mem_dump(cpu.mem);
	return true;
}

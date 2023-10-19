#pragma once
#include <array>
#include <fstream>
#include <optional>
#include <source_location>
#include <variant>

#include "common.h"
#include "patterns.h"
#include "utility16.h"

const std::array<const char*, 8> bytereg_names{"al", "cl", "dl", "bl",
											   "ah", "ch", "dh", "bh"};

const std::array<const char*, 8> wordreg_names{"ax", "cx", "dx", "bx",
											   "sp", "bp", "si", "di"};

const std::array<const char*, 8> effaddr_names{
	"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx"};
const std::array<const char*, 8> arithmetic_names{"add", "or",	"adc", "sbb",
												  "and", "sub", "xor", "cmp"};

const std::array<const char*, 16> flag_names{
	"C", "", "P", "", "A", "", "Z", "S", "T", "I", "D", "O", "", "", "", ""};

const std::array<const bool, 16> flag_used{
	true, false, true, false, true,	 false, true,  true,
	true, true,	 true, true,  false, false, false, false};

constexpr bool regsize_word = true;
constexpr bool regsize_byte = false;

// general/special register indices for word=1
constexpr size_t regidx_ax = 0;
constexpr size_t regidx_cx = 1;
constexpr size_t regidx_dx = 2;
constexpr size_t regidx_bx = 3;
constexpr size_t regidx_sp = 4;
constexpr size_t regidx_bp = 5;
constexpr size_t regidx_si = 6;
constexpr size_t regidx_di = 7;

// general/special register indices for word=0
constexpr size_t regidx_al = 0;
constexpr size_t regidx_cl = 1;
constexpr size_t regidx_dl = 2;
constexpr size_t regidx_bl = 3;
constexpr size_t regidx_ah = 4;
constexpr size_t regidx_ch = 5;
constexpr size_t regidx_dh = 6;
constexpr size_t regidx_bh = 7;

// flags for flag register
constexpr u16 flag_carry  = 1 << 0;
constexpr u16 flag_parity = 1 << 2;
constexpr u16 flag_aux	  = 1 << 4;
constexpr u16 flag_zero	  = 1 << 6;
constexpr u16 flag_sign	  = 1 << 7;
constexpr u16 flag_trap	  = 1 << 8;
constexpr u16 flag_int	  = 1 << 9;
constexpr u16 flag_dir	  = 1 << 10;
constexpr u16 flag_over	  = 1 << 11;

#include <iostream>

struct mod_reg_rm {
	mod_reg_rm(u8 byte)
	{
		mod = (byte & 0b1100'0000) >> 6;
		reg = (byte & 0b0011'1000) >> 3;
		rm  = (byte & 0b0000'0111);
	}
	mod_reg_rm(char ch_byte)
	{
		mod = (ch_byte & 0b1100'0000) >> 6;
		reg = (ch_byte & 0b0011'1000) >> 3;
		rm  = (ch_byte & 0b0000'0111);
	}
	u8 mod;
	u8 reg;
	u8 rm;
};

struct address_pair {
	uint16_t base;
	uint16_t displacement;
};

//typedef std::array<u8, 1 << 16> memory;

struct memory {
	std::array<u8, 1 << 16> mem;

	u16
	get(size_t addr, bool w) const;

	u16
	get(address_pair addr, bool w) const;

	void
	set(size_t addr, bool w, u16 val);

	void
	set(address_pair addr, bool w, u16 val);
};

struct registers {
	std::array<u16, 8> regs;	

	u16 
	get(size_t i, bool w) const;

	void
	set(size_t i, bool w, u16 val);

	void
	add(size_t i, bool w, u16 val);

	u16
	effaddr_base(u8 rm, u8 mod) const;
};

struct cpu {
	registers reg    = {};
	memory    mem    = {};
	u16       flags  = 0x0000;
	u16       ip     = 0x0000;
};

constexpr size_t operand_index_reg     = 0;
constexpr size_t operand_index_effaddr = 1;
constexpr size_t operand_index_addr    = 2;
constexpr size_t operand_index_imm     = 3;

struct operand_reg {
	bool word;
	u8 reg;
};

struct operand_effaddr {
	bool word;
	address_pair addr;
	u8 mod;
	u8 rm;
};

struct operand_addr {
	bool word;
	u16 addr;
};

struct operand_imm {
	bool word;
	u16 value;
};

typedef std::variant<operand_reg, operand_effaddr, operand_addr,
	operand_imm> operand;

// reset cpu state
void
reset(memory& mem);
void
reset(registers& reg);
void
reset(cpu& cpu);

u16
get(operand& o, cpu& cpu);

void
set(operand& o, cpu& cpu, u16 val);

bool
is_word(operand& o);

void
move(operand& dest, operand& src, cpu& cpu);

void
arithmetic(operand& dest, operand& src, cpu& cpu, u8 op);

void fn_assert(
	bool cond, const char* msg, const std::source_location loc);

size_t
get_ip(std::ifstream& fs);

bool
run_program(cpu& cpu, const char* file);

void
set_ip(std::ifstream& fs, size_t ip);

void
offset_ip(std::ifstream& fs, i8 offset);

template<typename T>
bool
read(std::ifstream& fs, T& dest, size_t num_bytes);

u16
must_read(
	std::ifstream& fs,u8 num_bytes,
	const std::source_location loc = std::source_location::current());

operation
pattern_match(u8 byte, const std::array<pattern, 127>& patterns);

address_pair
read_effaddr(const registers& reg, u8 mod, u8 rm, std::ifstream& fs);

void
print_value_change(u16 old_value, u16 new_value);

void
print_flag_change(u16 old_flags, u16 new_flags);

bool
run_program(cpu& cpu, const char* file);




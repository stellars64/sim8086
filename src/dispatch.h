#pragma once
#include <fstream>
#include "sim8086.h"

void
mov_regmem_tofrom_reg(cpu& cpu, u8 byte_op, std::ifstream& fs);

void
mov_imm_to_regmem(cpu& cpu, u8 byte_op, std::ifstream& fs);

void
mov_imm_to_reg(cpu& cpu, u8 byte_op, std::ifstream& fs);

void
move_accumulator_memory(
	cpu& cpu, u8 byte_op, std::ifstream& fs, bool dest_is_reg);

void
imm_to_regmem(cpu& cpu, u8 byte_op, std::ifstream& fs);

void
regmem_and_reg_to_either(
	cpu& cpu, u8 byte_op, std::ifstream& fs, size_t arithmetic_op);

void
imm_to_acc(cpu& cpu, u8 byte_op, std::ifstream& fs, size_t op);

void
handle_jump(cpu& cpu, u8 byte_op, std::ifstream& fs, operation op);

void
dispatch_operation(cpu& cpu, operation op, u8 byte_op, std::ifstream& fs);

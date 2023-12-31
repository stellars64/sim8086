#pragma once
#include <array>
#include <cstdint>

#include "common.h"
#include "operations.h"

struct pattern
{
	u8		  pattern;
	u8		  mask;
	operation op;
};

operation
pattern_match(const u8 byte, const std::array<pattern, 127>& patterns);

const std::array<pattern, 127> patterns{
	{{0b1000'1000, 0b1111'1100, operation::mov_regmem_tofrom_reg},
	 {0b1100'0110, 0b1111'1110, operation::mov_imm_to_regmem},
	 {0b1011'0000, 0b1111'0000, operation::mov_imm_to_reg},
	 {0b1010'0000, 0b1111'1110, operation::mov_mem_to_acc},
	 {0b1010'0010, 0b1111'1110, operation::mov_acc_to_mem},
	 {0b1000'1110, 0b1111'1111, operation::mov_regmem_to_seg},
	 {0b1000'1100, 0b1111'1111, operation::mov_seg_to_regmem},
	 {0b1111'1111, 0b1111'1111, operation::push_regmem},
	 {0b0101'0000, 0b1111'1000, operation::push_reg},
	 {0b0000'0110, 0b1110'0111, operation::push_seg},
	 {0b1000'1111, 0b1111'1111, operation::pop_regmem},
	 {0b0101'1000, 0b1111'1000, operation::pop_reg},
	 {0b0000'0111, 0b1110'0111, operation::pop_seg},
	 {0b1000'0110, 0b1111'1110, operation::xchg_regmem_with_reg},
	 {0b1001'0000, 0b1111'1000, operation::xchg_reg_with_acc},
	 {0b1110'0100, 0b1111'1110, operation::in_fixed_port},
	 {0b1110'1100, 0b1111'1110, operation::in_variable_port},
	 {0b1110'0110, 0b1111'1110, operation::out_fixed_port},
	 {0b1110'1110, 0b1111'1110, operation::out_variable_port},
	 {0b1101'0111, 0b1111'1111, operation::xlat},
	 {0b1000'1101, 0b1111'1111, operation::lea},
	 {0b1100'0101, 0b1111'1111, operation::lds},
	 {0b1100'0100, 0b1111'1111, operation::les},
	 {0b1001'1111, 0b1111'1111, operation::lahf},
	 {0b1001'1110, 0b1111'1111, operation::sahf},
	 {0b1001'1100, 0b1111'1111, operation::pushf},
	 {0b1001'1101, 0b1111'1111, operation::popf},
	 {0b1000'0000, 0b1111'1100, operation::imm_to_regmem},
	 {0b0000'0000, 0b1111'1100, operation::add_regmem_and_reg_to_either},
	 {0b0000'0100, 0b1111'1110, operation::add_imm_to_acc},
	 {0b0001'0000, 0b1111'1100, operation::adc_regmem_and_reg_to_either},
	 {0b0001'0100, 0b1111'1110, operation::adc_imm_to_acc},
	 {0b1111'1110, 0b1111'1110, operation::inc_regmem},
	 {0b0100'0000, 0b1111'1000, operation::inc_reg},
	 {0b0011'0111, 0b1111'1111, operation::aaa},
	 {0b0010'0111, 0b1111'1111, operation::daa},
	 {0b0010'1000, 0b1111'1100, operation::sub_regmem_and_reg_to_either},
	 {0b0010'1100, 0b1111'1110, operation::sub_imm_to_acc},
	 {0b0001'1000, 0b1111'1100, operation::sbb_regmem_and_reg_to_either},
	 {0b0001'1100, 0b1111'1110, operation::sbb_imm_to_acc},
	 {0b1111'1110, 0b1111'1110, operation::dec_regmem},
	 {0b0100'1000, 0b1111'1000, operation::dec_reg},
	 {0b1111'0110, 0b1111'1110, operation::neg},
	 {0b0011'1000, 0b1111'1100, operation::cmp_regmem_and_reg},
	 {0b0011'1100, 0b1111'1110, operation::cmp_imm_and_acc},
	 {0b0011'1111, 0b1111'1111, operation::aas},
	 {0b0010'1111, 0b1111'1111, operation::das},

	 // I need to merge mul/imul/et. al into one op
	 {0b1111'0110, 0b1111'1110, operation::mul},
	 {0b1111'0110, 0b1111'1111, operation::imul},
	 {0b1101'0100, 0b1111'1111, operation::aam},
	 {0b1111'0110, 0b1111'1110, operation::div},
	 {0b1111'0110, 0b1111'1110, operation::idiv},
	 {0b1101'0101, 0b1111'1111, operation::aad},
	 {0b1001'1000, 0b1111'1111, operation::cbw},
	 {0b1001'1001, 0b1111'1111, operation::cwd},
	 {0b1111'0110, 0b1111'1110, operation::not_}, // not is a keyword

 // these need to be merged into one op as well, 1101'0000
 {0b1101'0000, 0b1111'1100, operation::shl},
	 {0b1101'0000, 0b1111'1100, operation::shr},
	 {0b1101'0000, 0b1111'1100, operation::sar},
	 {0b1101'0000, 0b1111'1100, operation::rol},
	 {0b1101'0000, 0b1111'1100, operation::ror},
	 {0b1101'0000, 0b1111'1100, operation::rcl},
	 {0b1101'0000, 0b1111'1100, operation::rcr},
	 {0b0010'0000, 0b1111'1100, operation::and_regmem_and_reg_to_either},
	 {0b0010'0100, 0b1111'1110, operation::and_imm_to_acc},
	 {0b0001'0000, 0b1111'1100, operation::test_regmem_and_reg},
	 {0b1111'0110, 0b1111'1110, operation::test_imm_and_regmem},
	 {0b1010'1000, 0b1111'1110, operation::test_imm_and_acc},
	 {0b0000'1000, 0b1111'1100, operation::or_regmem_and_reg_to_either},
	 {0b0000'1100, 0b1111'1110, operation::or_imm_to_acc},
	 {0b0011'0000, 0b1111'1100, operation::xor_regmem_and_reg_to_either},
	 {0b0011'0100, 0b1111'1110, operation::xor_imm_to_acc},
	 {0b1111'0010, 0b1111'1110, operation::rep},
	 {0b1010'0101, 0b1111'1110, operation::movs},
	 {0b1010'0110, 0b1111'1110, operation::cmps},
	 {0b1010'1110, 0b1111'1110, operation::scas},
	 {0b1010'1100, 0b1111'1110, operation::lods},
	 {0b1010'1010, 0b1111'1110, operation::stds},

	 // 0b1111'1111 needs to be merged
	 {0b1110'1000, 0b1111'1111, operation::call_direct_within_seg},
	 {0b1111'1111, 0b1111'1111, operation::call_indirect_within_seg},
	 {0b1001'1010, 0b1111'1111, operation::call_direct_interseg},
	 {0b1111'1111, 0b1111'1111, operation::call_indirect_interseg},
	 {0b1110'1001, 0b1111'1111, operation::jmp_direct_within_seg},
	 {0b1110'1011, 0b1111'1111, operation::jmp_direct_within_seg_short},
	 {0b1111'1111, 0b1111'1111, operation::jmp_indirect_within_seg},
	 {0b1110'1010, 0b1111'1111, operation::jmp_direct_interseg},
	 {0b1111'1111, 0b1111'1111, operation::jmp_indirect_interseg},
	 {0b1100'0011, 0b1111'1111, operation::ret_within_seg},
	 {0b1100'0010, 0b1111'1111, operation::ret_within_seg_adding_imm},
	 {0b1100'1011, 0b1111'1111, operation::ret_interseg},
	 {0b1100'1010, 0b1111'1111, operation::ret_interseg_adding_imm},

	 {0b0111'0100, 0b1111'1111, operation::je},
	 {0b0111'1100, 0b1111'1111, operation::jl},
	 {0b0111'1110, 0b1111'1111, operation::jle},
	 {0b0111'0010, 0b1111'1111, operation::jb},
	 {0b0111'0110, 0b1111'1111, operation::jbe},
	 {0b0111'1010, 0b1111'1111, operation::jp},
	 {0b0111'0000, 0b1111'1111, operation::jo},
	 {0b0111'1000, 0b1111'1111, operation::js},
	 {0b0111'0101, 0b1111'1111, operation::jne},
	 {0b0111'1101, 0b1111'1111, operation::jnl},
	 {0b0111'1111, 0b1111'1111, operation::jnle},
	 {0b0111'0011, 0b1111'1111, operation::jnb},
	 {0b0111'0111, 0b1111'1111, operation::jnbe},
	 {0b0111'1011, 0b1111'1111, operation::jnp},
	 {0b0111'0001, 0b1111'1111, operation::jno},
	 {0b0111'1001, 0b1111'1111, operation::jns},
	 {0b1110'0010, 0b1111'1111, operation::loop},
	 {0b1110'0001, 0b1111'1111, operation::loopz},
	 {0b1110'0000, 0b1111'1111, operation::loopnz},
	 {0b1110'0011, 0b1111'1111, operation::jcxz},

	 {0b1100'1101, 0b1111'1111, operation::int_type_specified},
	 {0b1100'1100, 0b1111'1111, operation::int_type_3},
	 {0b1100'1110, 0b1111'1111, operation::into},
	 {0b1100'1111, 0b1111'1111, operation::iret},

	 {0b1111'1000, 0b1111'1111, operation::clc},
	 {0b1111'0101, 0b1111'1111, operation::cmc},
	 {0b1111'1001, 0b1111'1111, operation::stc},
	 {0b1111'1100, 0b1111'1111, operation::cld},
	 {0b1111'1101, 0b1111'1111, operation::std},
	 {0b1111'1010, 0b1111'1111, operation::cli},
	 {0b1111'1011, 0b1111'1111, operation::sti},
	 {0b1111'0100, 0b1111'1111, operation::hlt},
	 {0b1001'1011, 0b1111'1111, operation::wait},
	 {0b1101'1000, 0b1111'1000, operation::esc},
	 {0b1111'0000, 0b1111'1111, operation::lock},
	 {0b0010'0110, 0b1110'0111, operation::segment}}
};

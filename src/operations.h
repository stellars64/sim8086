#pragma once

#include <cstdint>

#include "common.h"

enum struct operation {
	invalid				  = -1,
	mov_regmem_tofrom_reg = 0,
	mov_imm_to_regmem,
	mov_imm_to_reg,
	mov_mem_to_acc,
	mov_acc_to_mem,
	mov_regmem_to_seg,
	mov_seg_to_regmem,

	push_regmem,
	push_reg,
	push_seg,

	pop_regmem,
	pop_reg,
	pop_seg,

	xchg_regmem_with_reg,
	xchg_reg_with_acc,
	in_fixed_port,
	in_variable_port,

	out_fixed_port,
	out_variable_port,

	xlat,
	lea,
	lds,
	les,
	lahf,
	sahf,
	pushf,
	popf,

	// covers 8 cases
	imm_to_regmem,

	add_regmem_and_reg_to_either,
	add_imm_to_acc,

	adc_regmem_and_reg_to_either,
	adc_imm_to_acc,

	inc_regmem,
	inc_reg,

	aaa,
	daa,

	sub_regmem_and_reg_to_either,
	sub_imm_to_acc,

	sbb_regmem_and_reg_to_either,
	sbb_imm_to_acc,

	dec_regmem,
	dec_reg,

	neg,

	cmp_regmem_and_reg,
	cmp_imm_and_acc,

	aas,
	das,
	mul,
	imul,
	aam,
	div,
	idiv,
	aad,
	cbw,
	cwd,

	not_,  // not is a keyword
	shl,
	shr,
	sar,
	rol,
	ror,
	rcl,
	rcr,

	and_regmem_and_reg_to_either,
	and_imm_to_acc,

	test_regmem_and_reg,
	test_imm_and_regmem,
	test_imm_and_acc,

	or_regmem_and_reg_to_either,
	or_imm_to_acc,

	xor_regmem_and_reg_to_either,
	xor_imm_to_acc,

	rep,
	movs,
	cmps,
	scas,
	lods,
	stds,

	call_direct_within_seg,
	call_indirect_within_seg,
	call_direct_interseg,
	call_indirect_interseg,

	jmp_direct_within_seg,
	jmp_direct_within_seg_short,
	jmp_indirect_within_seg,
	jmp_direct_interseg,
	jmp_indirect_interseg,

	ret_within_seg,
	ret_within_seg_adding_imm,
	ret_interseg,
	ret_interseg_adding_imm,

	je,
	jl,
	jle,
	jb,
	jbe,
	jp,
	jo,
	js,
	jne,
	jnl,
	jnle,
	jnb,
	jnbe,
	jnp,
	jno,
	jns,
	loop,
	loopz,
	loopnz,
	jcxz,

	int_type_specified,
	int_type_3,

	into,
	iret,

	clc,
	cmc,
	stc,
	cld,
	std,
	cli,
	sti,
	hlt,
	wait,
	esc,
	lock,
	segment,

	number_of_operations
};

constexpr size_t arithmetic_add = 0b000;
constexpr size_t arithmetic_or	= 0b001;
constexpr size_t arithmetic_adc = 0b010;
constexpr size_t arithmetic_sbb = 0b011;
constexpr size_t arithmetic_and = 0b100;
constexpr size_t arithmetic_sub = 0b101;
constexpr size_t arithmetic_xor = 0b110;
constexpr size_t arithmetic_cmp = 0b111;

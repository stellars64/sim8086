#include <iostream>
#include <iomanip>

#include "dispatch.h"
#include "sim8086.h"

#define PRINT_DISPATCH_INSTR 1

void
dispatch_operation(cpu& cpu, operation op, u8 byte_op, std::ifstream& fs)
{
	auto print_dispatch_instr = [&](const char* instr) {
#if PRINT_DISPATCH_INSTR
		std::cout <<"\033[1;36m";
		std::cout << std::hex << std::setfill('0') << std::right
		          << "; (0x" << std::setw(2) << static_cast<i32>(byte_op)
				  << ") " << instr;
		std::cout << "\033[0m\n\t";
#endif
	};

//= INVALID ===================================================================*
	switch (op) {
	case operation::invalid: {
		std::cout << ";invalid opcode, opcode is "
		          << std::hex << static_cast<int>(byte_op);
		break;
	}
//= MOVE OPERATIONS ===========================================================*
	case operation::mov_regmem_tofrom_reg: {
		print_dispatch_instr("mov_regmem_tofrom_reg");

		bool d = (byte_op & 0b0000'0010) >> 1;
		bool w = (byte_op & 0b0000'0001);
		u8 mrr = must_read(fs, 1);
		auto [mod, reg, rm] = mod_reg_rm{mrr};

		operand dest = operand_reg{w, reg};

		operand src;
		if (mod == 0b11) {
			src = operand_reg{w, rm};
		} else {
			auto addr = read_effaddr(cpu.reg, mod, rm, fs);
			src = operand_effaddr{w, addr, mod, rm};
		}

		if (!d)
			std::swap(dest, src);

		move(dest, src, cpu);
		break;
	}
//------------------------------------------------------------------------------
	case operation::mov_imm_to_regmem: {
		print_dispatch_instr("mov_imm_to_regmem");

		bool w = byte_op & 0b0000'0001;
		u8 mrr = must_read(fs, 1);
		auto [mod, _, rm] = mod_reg_rm{mrr};

		operand dest;
		if (mod == 0b11) {
			dest = operand_reg{w, rm};
		} else {
			auto addr = read_effaddr(cpu.reg, mod, rm, fs);
			dest = operand_effaddr{w, addr, mod, rm};
		}
		operand src = operand_imm{w, must_read(fs, w ? 2 : 1)};

		move(dest, src, cpu);
		break;
	}
//------------------------------------------------------------------------------
	case operation::mov_imm_to_reg: {
		print_dispatch_instr("mov_imm_to_reg");

		bool w = (byte_op & 0b0000'1000) >> 3;
		u8 reg = (byte_op & 0b0000'0111);
		operand dest = operand_reg{w, reg};
		operand src  = operand_imm{w, must_read(fs, w ? 2 : 1)};
		move(dest, src, cpu);
		break;
	}
//------------------------------------------------------------------------------
	case operation::mov_mem_to_acc: {
		print_dispatch_instr("mov_mem_to_acc");

		bool w = (byte_op & 0b0000'0001);
		operand dest = operand_reg{w, regidx_ax};
		operand src  = operand_addr{w, must_read(fs, 2)};
		move(dest, src, cpu);
		break;
	}
//------------------------------------------------------------------------------
	case operation::mov_acc_to_mem: {
		print_dispatch_instr("mov_acc_to_mem");

		bool w = (byte_op & 0b0000'0001);
		operand dest = operand_addr{w, must_read(fs, 2)};
		operand src  = operand_reg{w, regidx_ax};
		move(dest, src, cpu);
		break;
	}
//------------------------------------------------------------------------------
	case operation::mov_regmem_to_seg:
		std::cout << "Not Implemented: mov_regmem_to_seg";
		break;
//------------------------------------------------------------------------------
	case operation::mov_seg_to_regmem:
		std::cout << "Not Implemented: mov_seg_to_regmem";
		break;
//- VARIOUS THINGS -------------------------------------------------------------
	case operation::push_regmem:
		std::cout << "Not Implemented: push_regmem";
		break;
	case operation::push_reg:
		std::cout << "Not Implemented: push_reg";
		break;
	case operation::push_seg:
		std::cout << "Not Implemented: push_seg";
		break;
	case operation::pop_regmem:
		std::cout << "Not Implemented: pop_regmem";
		break;
	case operation::pop_reg:
		std::cout << "Not Implemented: pop_reg";
		break;
	case operation::pop_seg:
		std::cout << "Not Implemented: pop_seg";
		break;
	case operation::xchg_regmem_with_reg:
		std::cout << "Not Implemented: xchg_regmem_with_reg";
		break;
	case operation::xchg_reg_with_acc:
		std::cout << "Not Implemented: xchg_reg_with_acc";
		break;
	case operation::in_fixed_port:
		std::cout << "Not Implemented: in_fixed_port";
		break;
	case operation::in_variable_port:
		std::cout << "Not Implemented: in_variable_port";
		break;
	case operation::out_fixed_port:
		std::cout << "Not Implemented: out_fixed_port";
		break;
	case operation::out_variable_port:
		std::cout << "Not Implemented: out_variable_port";
		break;
	case operation::xlat:
		std::cout << "Not Implemented: xlat";
		break;
	case operation::lea:
		std::cout << "Not Implemented: lea";
		break;
	case operation::lds:
		std::cout << "Not Implemented: lds";
		break;
	case operation::les:
		std::cout << "Not Implemented: les";
		break;
	case operation::lahf:
		std::cout << "Not Implemented: lahf";
		break;
	case operation::sahf:
		std::cout << "Not Implemented: sahf";
		break;
	case operation::pushf:
		std::cout << "Not Implemented: pushf";
		break;
	case operation::popf:
		std::cout << "Not Implemented: popf";
		break;
//= ARITHMETIC GROUP 1 =========================================================
	case operation::imm_to_regmem: {
		print_dispatch_instr("imm_to_regmem");

		bool s = (byte_op & 0b0000'0010) >> 1;
		bool w = (byte_op & 0b0000'0001);
		u8 mrr = must_read(fs, 1);
		auto [mod, arithmetic_op, rm] = mod_reg_rm{mrr};

		operand dest;
		if (mod == 0b11) {
			dest = operand_reg{w, rm};
		} else {
			auto addr = read_effaddr(cpu.reg, mod, rm, fs);
			dest = operand_effaddr{w, addr, mod, rm};
		}

		operand src = operand_imm{w, must_read(fs, w && !s ? 2 : 1)};
		arithmetic(dest, src, cpu, arithmetic_op);
		break;
	}
// TODO: Locationize this
//------------------------------------------------------------------------------
	case operation::add_regmem_and_reg_to_either:
	case operation::or_regmem_and_reg_to_either:
	case operation::adc_regmem_and_reg_to_either:
	case operation::sbb_regmem_and_reg_to_either:
	case operation::and_regmem_and_reg_to_either:
	case operation::sub_regmem_and_reg_to_either:
	case operation::xor_regmem_and_reg_to_either:
	case operation::cmp_regmem_and_reg: {

		switch (op) {
		case operation::add_regmem_and_reg_to_either:
			print_dispatch_instr("add_regmem_and_reg_to_either"); break;
		case operation::or_regmem_and_reg_to_either:
			print_dispatch_instr("or_regmem_and_reg_to_either"); break;
		case operation::adc_regmem_and_reg_to_either:
			print_dispatch_instr("adc_regmem_and_reg_to_either"); break;
		case operation::sbb_regmem_and_reg_to_either:
			print_dispatch_instr("sbb_regmem_and_reg_to_either"); break;
		case operation::and_regmem_and_reg_to_either:
			print_dispatch_instr("and_regmem_and_reg_to_either"); break;
		case operation::sub_regmem_and_reg_to_either:
			print_dispatch_instr("sub_regmem_and_reg_to_either"); break;
		case operation::xor_regmem_and_reg_to_either:
			print_dispatch_instr("xor_regmem_and_reg_to_either"); break;
		case operation::cmp_regmem_and_reg:
			print_dispatch_instr("cmp_regmem_and_reg"); break;
		default:
			print_dispatch_instr("unknown"); break;
		}

		size_t arithmetic_op = (byte_op & 0b0011'1000) >> 3;
		bool d = (byte_op & 0b0000'0010) >> 1;
		bool w = (byte_op & 0b0000'0001);
		u8 mrr = must_read(fs, 1);
		auto [mod, reg, rm] = mod_reg_rm{mrr};

		operand dest = operand_reg{w, reg};
		operand src;
		if (mod != 0b11) {
			auto addr = read_effaddr(cpu.reg, mod, rm, fs);
			src = operand_effaddr{w, addr, mod, rm};
		} else {
			src = operand_reg{w, rm};
		}

		if (!d)
			std::swap(dest, src);

		arithmetic(dest, src, cpu, arithmetic_op);
		break;
	}
//------------------------------------------------------------------------------
	case operation::add_imm_to_acc:
	case operation::adc_imm_to_acc:
	case operation::sub_imm_to_acc:
	case operation::sbb_imm_to_acc:
	case operation::cmp_imm_and_acc:
	case operation::and_imm_to_acc:
	case operation::or_imm_to_acc:
	case operation::xor_imm_to_acc: {
		switch (op) {
		case operation::add_imm_to_acc:
			print_dispatch_instr("add_imm_to_acc"); break;
		case operation::adc_imm_to_acc:
			print_dispatch_instr("adc_imm_to_acc"); break;
		case operation::sub_imm_to_acc:
			print_dispatch_instr("sub_imm_to_acc"); break;
		case operation::sbb_imm_to_acc:
			print_dispatch_instr("sbb_imm_to_acc"); break;
		case operation::cmp_imm_and_acc:
			print_dispatch_instr("cmp_imm_and_acc"); break;
		case operation::and_imm_to_acc:
			print_dispatch_instr("and_imm_to_acc"); break;
		case operation::or_imm_to_acc:
			print_dispatch_instr("or_imm_to_acc"); break;
		case operation::xor_imm_to_acc:
			print_dispatch_instr("xor_imm_to_acc"); break;
		default:
			print_dispatch_instr("unknown"); break;
		}

		size_t arithmetic_op = (byte_op & 0b0011'1000) >> 3;
		bool w = (byte_op & 0b0000'0001);
		operand dest = operand_reg{w, regidx_ax};
		operand src= operand_imm{w, must_read(fs, w ? 2 : 1)};
		arithmetic(dest, src, cpu, arithmetic_op);
		break;
	}
	case operation::inc_regmem:
		std::cout << "Not Implemented: inc_regmem";
		break;
	case operation::inc_reg:
		std::cout << "Not Implemented: inc_reg";
		break;
	case operation::aaa:
		std::cout << "Not Implemented: aaa";
		break;
	case operation::daa:
		std::cout << "Not Implemented: daa";
		break;
	case operation::dec_regmem:
		std::cout << "Not Implemented: dec_regmem";
		break;
	case operation::dec_reg:
		std::cout << "Not Implemented: dec_reg";
		break;
	case operation::neg:
		std::cout << "Not Implemented: neg";
		break;
	case operation::aas:
		std::cout << "Not Implemented: aas";
		break;
	case operation::das:
		std::cout << "Not Implemented: das";
		break;
	case operation::mul:
		std::cout << "Not Implemented: mul";
		break;
	case operation::imul:
		std::cout << "Not Implemented: imul";
		break;
	case operation::aam:
		std::cout << "Not Implemented: aam";
		break;
	case operation::div:
		std::cout << "Not Implemented: div";
		break;
	case operation::idiv:
		std::cout << "Not Implemented: idiv";
		break;
	case operation::aad:
		std::cout << "Not Implemented: aad";
		break;
	case operation::cbw:
		std::cout << "Not Implemented: cbw";
		break;
	case operation::cwd:
		std::cout << "Not Implemented: cwd";
		break;
	case operation::not_:
		std::cout << "Not Implemented: not";
		break;
	case operation::shl:
		std::cout << "Not Implemented: shl";
		break;
	case operation::shr:
		std::cout << "Not Implemented: shr";
		break;
	case operation::sar:
		std::cout << "Not Implemented: sar";
		break;
	case operation::rol:
		std::cout << "Not Implemented: rol";
		break;
	case operation::ror:
		std::cout << "Not Implemented: ror";
		break;
	case operation::rcl:
		std::cout << "Not Implemented: rcl";
		break;
	case operation::rcr:
		std::cout << "Not Implemented: rcr";
		break;
	case operation::test_regmem_and_reg:
		std::cout << "Not Implemented: test_regmem_and_reg";
		break;
	case operation::test_imm_and_regmem:
		std::cout << "Not Implemented: test_imm_and_regmem";
		break;
	case operation::test_imm_and_acc:
		std::cout << "Not Implemented: test_imm_and_acc";
		break;
	case operation::rep:
		std::cout << "Not Implemented: rep";
		break;
	case operation::movs:
		std::cout << "Not Implemented: movs";
		break;
	case operation::cmps:
		std::cout << "Not Implemented: cmps";
		break;
	case operation::scas:
		std::cout << "Not Implemented: scas";
		break;
	case operation::lods:
		std::cout << "Not Implemented: lods";
		break;
	case operation::stds:
		std::cout << "Not Implemented: stds";
		break;
	case operation::call_direct_within_seg:
		std::cout << "Not Implemented: call_direct_within_seg";
		break;
	case operation::call_indirect_within_seg:
		std::cout << "Not Implemented: call_indirect_within_seg";
		break;
	case operation::call_direct_interseg:
		std::cout << "Not Implemented: call_direct_interseg";
		break;
	case operation::call_indirect_interseg:
		std::cout << "Not Implemented: call_indirect_interseg";
		break;
	case operation::jmp_direct_within_seg:
		std::cout << "Not Implemented: jmp_direct_within_seg";
		break;
	case operation::jmp_direct_within_seg_short:
		std::cout << "Not Implemented: jmp_direct_within_seg_short" <<std::endl;
		break;
	case operation::jmp_indirect_within_seg:
		std::cout << "Not Implemented: jmp_indirect_within_seg";
		break;
	case operation::jmp_direct_interseg:
		std::cout << "Not Implemented: jmp_direct_interseg";
		break;
	case operation::jmp_indirect_interseg:
		std::cout << "Not Implemented: jmp_indirect_interseg";
		break;
	case operation::ret_within_seg:
		std::cout << "Not Implemented: ret_within_seg";
		break;
	case operation::ret_within_seg_adding_imm:
		std::cout << "Not Implemented: ret_within_seg_adding_imm";
		break;
	case operation::ret_interseg:
		std::cout << "Not Implemented: ret_interseg";
		break;
	case operation::ret_interseg_adding_imm:
		std::cout << "Not Implemented: ret_interseg_adding_imm";
		break;
	case operation::je:
	case operation::jl:
	case operation::jle:
	case operation::jb:
	case operation::jbe:
	case operation::jp:
	case operation::jo:
	case operation::jne:
	case operation::jnl:
	case operation::jnle:
	case operation::jnb:
	case operation::jnbe:
	case operation::jnp:
	case operation::jno:
	case operation::jns:
	case operation::loop:
	case operation::loopz:
	case operation::loopnz:
	case operation::jcxz: {
		bool do_jump      = false;
		const char* opstr = nullptr;
		i8 ip_offset      = must_read(fs, 1);

		switch (op) {
		case operation::je:
			opstr	= "je";
			do_jump = has_flags(cpu.flags, flag_zero);
			break;
		case operation::jb:
			opstr	= "jb";
			do_jump = has_flags(cpu.flags, flag_carry);
			break;
		case operation::jne:
			opstr	= "jne";
			do_jump = !has_flags(cpu.flags, flag_zero);
			break;
		case operation::jnb:
			opstr	= "jne";
			do_jump = !has_flags(cpu.flags, flag_carry);
			break;
		case operation::jp:
			opstr	= "jp";
			do_jump = has_flags(cpu.flags, flag_parity);
			break;
		case operation::loop:
			opstr = "loop";
			cpu.reg.add(regidx_cx, regsize_word, -1);
			do_jump = (cpu.reg.get(regidx_cx, regsize_word) != 0);
			break;
		case operation::loopz:
			opstr = "loopz";
			cpu.reg.add(regidx_cx, regsize_word, -1);
			do_jump = (cpu.reg.get(regidx_cx, regsize_word) != 0)
					  && has_flags(cpu.flags, flag_zero);
			break;
		case operation::loopnz:
			opstr = "loopnz";
			cpu.reg.add(regidx_cx, regsize_word, -1);
			do_jump = (cpu.reg.get(regidx_cx, regsize_word) != 0)
					  && !has_flags(cpu.flags, flag_zero);
			break;
		default:
			do_jump = false;
			break;
		}
		print_dispatch_instr(opstr);

		std::cout << opstr << "  $" << std::dec << static_cast<i32>(ip_offset)
				  << "; ";

		if (do_jump)
			offset_ip(fs, ip_offset);
		break;
	}
	case operation::int_type_specified:
		std::cout << "Not Implemented: int_type_specified";
		break;
	case operation::int_type_3:
		std::cout << "Not Implemented: int_type_3";
		break;
	case operation::into:
		std::cout << "Not Implemented: into";
		break;
	case operation::iret:
		std::cout << "Not Implemented: iret";
		break;
	case operation::clc:
		std::cout << "Not Implemented: clc";
		break;
	case operation::cmc:
		std::cout << "Not Implemented: cmc";
		break;
	case operation::stc:
		set_flags(cpu.flags, flag_carry);
		break;
	case operation::cld:
		std::cout << "Not Implemented: cld";
		break;
	case operation::std:
		std::cout << "Not Implemented: std";
		break;
	case operation::cli:
		std::cout << "Not Implemented: cli";
		break;
	case operation::sti:
		std::cout << "Not Implemented: sti";
		break;
	case operation::hlt:
		std::cout << "Not Implemented: hlt";
		break;
	case operation::wait:
		std::cout << "Not Implemented: wait";
		break;
	case operation::esc:
		std::cout << "Not Implemented: esc";
		break;
	case operation::lock:
		std::cout << "Not Implemented: lock";
		break;
	case operation::segment:
		std::cout << "Not Implemented: segment";
		break;
	default:
		std::cout << "unknown operation: " << std::hex << (int)op;
		break;
	}

	return;
}

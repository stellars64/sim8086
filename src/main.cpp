//=============================================================================*
// Name:    sim8086
// Desc:    8086 cpu sim for C.Muratori's Performance Aware Course
// Author:  Drew Davis (stellars)
// Started: 2023-07-13
//
// You are completely free to use this code for any purpose!
//=============================================================================*

#include <fstream>
#include <iomanip>
#include <iostream>

#include "common.h"
#include "sim8086.h"
#include "utility16.h"
#include "print.h"

void
run_all_tests(cpu& c)
{
	auto print_header = [](const char* text) {
		std::cout << "\t\t\033[95m" << text << "\033[95m\n";
	};

	print_header("running tests/bin/mov1...");
	c.reg.set(regidx_cx, 1, 0x1234);
	run_program(c, "tests/bin/mov1");
	assert(c.reg.get(regidx_ax, 1) == 0x0034);
	assert(c.reg.get(regidx_bx, 1) == 0x3434);
	assert(c.reg.get(regidx_cx, 1) == 0x1234);
	assert(c.reg.get(regidx_dx, 1) == 0x1234);
	reset(c);

	print_header("running tests/bin/mov2...");
	c.reg.set(regidx_cx, 1, 0x1234);
	c.reg.set(regidx_si, 1, 0x0005);
	run_program(c, "tests/bin/mov2");
	assert(c.reg.get(regidx_cx, 1) == 0x1234);
	assert(c.reg.get(regidx_si, 1) == 0x0005);
	assert(c.reg.get(regidx_ax, 1) == 0x1234);
	assert(c.reg.get(regidx_dx, 1) == 0x1234);
	assert(c.mem.get(1000, 1) == 0x1234);
	assert(c.mem.get(1005, 1) == 0x1234);
	reset(c);

	
	print_header("running tests/bin/mov3...");
	c.reg.set(regidx_cx, 1, 0x1234);
	c.reg.set(regidx_si, 1, 0x0005);
	run_program(c, "tests/bin/mov3");
	assert(c.reg.get(regidx_ax, 1) == 0x3434);
	assert(c.reg.get(regidx_bx, 1) == 0x0034);
	assert(c.reg.get(regidx_cx, 1) == 0x1234);
	assert(c.reg.get(regidx_dx, 1) == 0x3400);
	assert(c.mem.get(1000, 0) == 0x34);
	assert(c.mem.get(1005, 0) == 0x34);
	reset(c);

	print_header("running tests/bin/mov4...");
	run_program(c, "tests/bin/mov4");
	assert(c.reg.get(regidx_ax, 1) == 0x12ff);
	assert(c.reg.get(regidx_bx, 1) == 0xff45);
	assert(c.reg.get(regidx_cx, 1) == 0x3400);
	assert(c.reg.get(regidx_dx, 1) == 0x0067);
	assert(c.reg.get(regidx_si, 1) == 0x0002);
	assert(c.mem.get(0x1000, 1) == 0xf0f0);
	assert(c.mem.get(0x2000, 0) == 0x0f);
	assert(c.mem.get(0xff50, 1) == 0xf0f0);
	assert(c.mem.get(0xffa1, 0) == 0x0f);
	reset(c);

	print_header("running tests/bin/mov5...");
	c.reg.set(regidx_ax, 1, 0x1234);
	run_program(c, "tests/bin/mov5");
	assert(c.reg.get(regidx_ax, 1) == 0x3412);
	reset(c);

	/*
	print_header("running tests/bin/add1...");
	c.reg.set(regidx_ax, 1, 0x00ea);
	c.reg.set(regidx_bx, 1, 0x01d4);
	run_program(c, "tests/bin/add1");
	assert(c.reg.get(regidx_ax, 1) == 0x02be);
	assert(c.reg.get(regidx_bx, 1) == 0x0492);
	assert(c.reg.get(regidx_dx, 1) == 0x9204);
	reset(c);

	print_header("running tests/bin/add2...");
	c.reg.set(regidx_ax, 1, 0x00ea);
	c.reg.set(regidx_bx, 1, 0x01d4);
	c.reg.set(regidx_si, 1, 27);
	run_program(c, "tests/bin/add2");
	assert(c.reg.get(regidx_ax, 1) == 0x00ea);
	assert(c.reg.get(regidx_bx, 1) == 0x02be);
	assert(c.reg.get(regidx_dx, 1) == 0x0750);
	assert(c.reg.get(regidx_si, 1) == 27);
	assert(c.mem.get(0x1000, 1) == 0x00ea);
	assert(c.mem.get(128, 1) == 0x03a8);
	reset(c);

	print_header("running tests/bin/add3...");
	run_program(c, "tests/bin/add3");
	assert(c.reg.get(regidx_ax, 1) == 0x0121);
	assert(c.reg.get(regidx_bx, 1) == 0x0065);
	assert(c.reg.get(regidx_cx, 1) == 0x02cc);
	assert(c.reg.get(regidx_si, 1) == 0x0539);
	assert(c.mem.get(0x999a, 1) == 0x014f);
	assert(c.mem.get(1337, 1) == 16);
	assert(c.mem.get(1345, 0) == 0x00);
	*/

	print_header("running tests/bin/arithmetic1_reg16_reg16...");
	c.reg.set(regidx_ax, 1, 50177);
	c.reg.set(regidx_bx, 1, 50177);
	c.reg.set(regidx_cx, 1, 50177);
	c.reg.set(regidx_dx, 1, 50177);
	c.reg.set(regidx_si, 1, 9131);
	run_program(c, "tests/bin/arithmetic1_reg16_reg16");
	assert(c.reg.get(regidx_ax, 1) == 59308);
	assert(c.reg.get(regidx_bx, 1) == 41046);
	assert(c.reg.get(regidx_cx, 1) == 59309);
	assert(c.reg.get(regidx_dx, 1) == 41045);
	reset(c);

	print_header("running tests/bin/arithmetic1_reg8_reg8...");
	c.reg.set(regidx_ax, 1, 0x2247);
	c.reg.set(regidx_bx, 1, 0x2247);
	c.reg.set(regidx_dl, 0, 0x11);
	run_program(c, "tests/bin/arithmetic1_reg8_reg8");
	assert(c.reg.get(regidx_al, 0) == 0x58);
	assert(c.reg.get(regidx_bl, 0) == 0x36);
	assert(c.reg.get(regidx_ah, 0) == 0x34);
	assert(c.reg.get(regidx_bh, 0) == 0x10);
	reset(c);

	print_header("running tests/bin/arithmetic1_dmem_reg16...");
	c.reg.set(regidx_si, 1, 9131);
	c.mem.set(1000, 1, 50177);
	c.mem.set(1002, 1, 50177);
	c.mem.set(1004, 1, 50177);
	c.mem.set(1006, 1, 50177);
	run_program(c, "tests/bin/arithmetic1_dmem_reg16");
	assert(c.mem.get(1000, 1) == 59308);
	assert(c.mem.get(1002, 1) == 41046);
	assert(c.mem.get(1004, 1) == 59309);
	assert(c.mem.get(1006, 1) == 41045);
	reset(c);
}

int
main(int argc, char** argv)
{
	if (argc < 2) {
		std::cout << "usage: sim8086 <program_name>  -r" << std::endl;
		return 1;
	}

	bool print_registers = false;
	bool test_mode = false;

	if (argc > 2) {
		std::string arg = argv[2];
		if (arg.front() == '-')	{
			for (auto& c : arg) {
				std::cout << "Letter being examined: " << c << std::endl;
				switch (c) {
				case 'r': print_registers = true; break;
				case 't': test_mode = true; break;
				}
			}
		}
	}

	cpu c8086;

	if (test_mode) {
		run_all_tests(c8086);
	} else {
		run_program(c8086, argv[1]);
	}

	if (print_registers) {
		std::cout << c8086 << std::endl;
	}
	return 0;
}

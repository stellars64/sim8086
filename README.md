# sim8086

This is the 8086 CPU simulator I made for Casey Muratori's Performance Aware
course. I took a bit of a break working on it -- last I remember, I was working
on writing tests for it.

## Building & Usage

Run **make** to build. This should create the executable **sim**.

To run, the format is **./sim <executable> -rt**.

**-r**: Print the state of the registers after running.
**-t**: Run the program in test mode. Unfortunately, you will still need to give
some executable name. (It doesn't have to exist, and I will fix later!)

**Test Mode**: Runs all of the test programs in the **tests** directory.

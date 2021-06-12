SIM = icarus
TOPLEVEL_LANG = verilog
VERILOG_SOURCES = $(shell pwd)/../../src/bram_dual_port.v
TOPLEVEL = bram_dual_port
MODULE = test_bram_dual_port

include $(shell cocotb-config --makefiles)/Makefile.sim

clean_all: clean
	rm -rf __pycache__ results.xml
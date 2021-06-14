export COCOTB_REDUCED_LOG_FMT=1
SIM = icarus
TOPLEVEL_LANG = verilog
VERILOG_SOURCES = $(shell pwd)/../../src/bram_single_port.v
TOPLEVEL = bram_single_port
MODULE = test_bram_single_port

include $(shell cocotb-config --makefiles)/Makefile.sim

clean_all: clean
	rm -rf __pycache__ results.xml
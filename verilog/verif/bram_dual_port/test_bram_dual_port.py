import random
import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge
from cocotb.triggers import FallingEdge

@cocotb.test()
async def test_ram(dut):
    clock_a = Clock(dut.a_clk, 10, units="us") # 10us period clock
    clock_b = Clock(dut.b_clk, 9, units="us") # 10us period clock
    cocotb.fork(clock_a.start())
    cocotb.fork(clock_b.start())

    dut.a_clk <= 0b0
    dut.a_wr <= 0b0
    dut.a_addr <= 0b0000000000
    dut.a_din <= 0x00000000

    dut.b_clk <= 0b0
    dut.b_wr <= 0b0
    dut.b_addr <= 0b0000000001
    dut.b_din <= 0x00000000

    await RisingEdge(dut.a_clk)

    dut.a_wr <= 0b1
    dut.a_din <= 0x90a0060f

    await RisingEdge(dut.a_clk)

    dut.a_wr <= 0b0
    assert dut.a_dout.value.binstr == "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", "Result should not be known yet"

    await RisingEdge(dut.a_clk)

    dut.a_wr <= 0b0
    assert dut.a_dout.value.binstr == "10010000101000000000011000001111", "Should be able to read 1 cycle after writing"

    await RisingEdge(dut.a_clk)
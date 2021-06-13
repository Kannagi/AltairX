import random
import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge
from cocotb.triggers import FallingEdge

@cocotb.test()
async def test_ram(dut):
    clock = Clock(dut.clk, 10, units="us") # 10us period clock
    
    cocotb.fork(clock.start())

    dut.clk <= 0b0
    dut.wr <= 0b0
    dut.addr <= 0b0000000000
    dut.din <= 0x00000000

    await RisingEdge(dut.clk)

    dut.wr <= 0b1
    dut.din <= 0xaba4268b

    await RisingEdge(dut.clk)

    dut.wr <= 0b0
    assert dut.dout.value.binstr == "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", "Result should not be known yet"

    await RisingEdge(dut.clk)

    dut.wr <= 0b0
    assert dut.dout.value.binstr == "10101011101001000010011010001011", "Should be able to read 1 cycle after writing"

    await RisingEdge(dut.clk)
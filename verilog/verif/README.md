# Verification of the Verilog code

In this directory you will find verification related content

Every directory is named after the verilog file it depends on, each one have they dedicated Makefile.

## Dependencies

### Debian/Ubuntu
Under linux / Debian based distribution (Like Ubuntu) you can:
```bash
./configure
```

### Others
For other OS and distribution you will need:
 - ICarus verilog (iverilog)
 - Cocotb (`sudo python3 -m pip install cocotb`)
 - Pytest (`sudo python3 -m pip install pytest`)
 - Optional: Verilator
You can then run simulations:
`cd bram_dual_port/ && make`

## Hardware requirement

There is no special hardware requirement, it should run on any machine which can run python.
# Table of Contents

* [I) General opcode structure](#I-General-opcode-structure)
  * [I.1) Basic opcode structure](#I-1-Basic-opcode-structure)
* [II) Compute units opcodes](#II-Compute-units-opcodes)
  * [II.1) AGU and BRU](#II-1-AGU-and-BRU)
  * [II.2) LSU](#II-2-LSU)
    * [II.2.1) LDM/STM](#II-2-1-LDM-STM)

# I) General opcode structure

Opcodes are 32-bits little-endian unsigned integers.
In this documents, all bitsets are written in natural order (i.e. big-endian, 31 -> 15 -> 7 -> 0) because it's easier and the most common.
Bitfiels bounds are inclusive (3 - 0 means both bits 3, 2, 1 and 0)

## I.1) Compute units

Altair K1 has the following compute units:

* 4 ALU
* 2 VFPU 
* 1 FDIV 
* 1 DIV 
* 1 BRU 
* 1 LSU 
* 1 AGU

Each cycle 2 or 4 opcodes are decoded and executed. Each opcode as an index in the range [0; 3], which is its index in the 2 (or 4) uint32 opcodes buffer. Compute units are bound to some index, you can only use some compute units on some indices.

| Opcode index | Available compute units for index                           |
| :----------: | :---------------------------------------------------------: |
| 0            | ALU#1 or LSU#1 (load-only) or VFPU#1 or VDIV or BRU or XCHG |
| 1            | ALU#2 or LSU#2 or VFPU#2 or AGU                             |
| 2            | ALU#3                                                       |
| 3            | ALU#4                                                       |

## I.2) Basic opcode structure

The following table is applicable to **ALL** opcodes

| 31 - 2    | 1 - 0          |
| :-------: | :------------: |
| Dependent | *Compute Unit* |

| *Compute Unit* value | Compute Unit |
| :------------------: | :----------: |
| 0                    | AGU or BRU   |
| 1                    | LSU          |
| 2                    | ALU          |
| 3                    | VFPU or VDIV |

Depending on the value of *Compute Unit*, and of opcode index, the decoding steps will differ.
Here is a table with all *Compute Unit* and opcode index possible values:

| *Compute Unit* | Opcode index | Compute Unit |
| :------------: | :----------: | :----------: |
| 0              | 0            | BRU          |
| 0              | 1            | LSU          |
| 0              | 2            | ALU or XCHG  |
| 0              | 3            | VFPU or VDIV |
| 1              | 0            | AGU          |
| 1              | 1            | LSU          |
| 1              | 2            | ALU          |
| 1              | 3            | VFPU         |
| 2              | 0            | Illegal      |
| 2              | 1            | Illegal      |
| 2              | 2            | ALU          |
| 2              | 3            | Illegal      |
| 3              | 0            | Illegal      |
| 3              | 1            | Illegal      |
| 3              | 2            | ALU          |
| 3              | 3            | Illegal      |

# II) Compute units opcodes

## II.1) BRU

| 31 - 4    | 3 - 2   | 1 - 0 |
| :-------: | :-----: | :---: |
| Dependent | *Type*  | 0     |

| *Type* value | Instruction                  |
| :----------: | :--------------------------: |
| 0            | REG-REG comparison or Branch |
| 1            | CMPI                         |
| 2            | FCMPI                        |
| 3            | DCMPI                        |

Depending on the value of *Type* the decoding steps will differ.

### II.1.1) REG-REG comparison or Branch

| 31 - 6    | 5 - 4         | 3 - 2 | 1 - 0 |
| :-------: | :-----------: | :---: | :---: |
| Dependent | *Instruction* | 0     | 0     |

| *Instruction* value | Instruction |
| :-----------------: | :---------: |
| 0                   | CMP         |
| 1                   | FCMP        |
| 2                   | DCMP        |
| 3                   | Branching   |

Depending on the value of *Instruction* the decoding steps will differ.

#### II.1.1.1) CMP

Update the comparison flags based on integer comparison.

| 31 - 26    | 25 - 20     | 19 - 10 | 9 - 8  | 7 - 6 | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :---------: | :-----: | :----: | :---: | :---: | :---: | :---: |
| *Source 2* | *Source 1*  | 0       | *Size* | 0     | 0     | 0     | 0     |

* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Source 1*: The first register to compare with *Source 2* (right operand)
* *Source 2*: The second register (left operand)

#### II.1.1.2) FCMP

Update the comparison flags base on single-precision float comparison.

| 31 - 25    | 24 - 18    | 17 - 6 | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :--------: | :----: | :---: | :---: | :---: |
| *Source 2* | *Source 1* | 0      | 1     | 0     | 0     |

* *Source 1*: The first register to compare with *Source 2* (right operand)
* *Source 2*: The second register (left operand)

#### II.1.1.3) DCMP

Update the comparison flags base on double-precision float comparison.

| 31 - 26    | 25 - 20    | 19 - 6 | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :--------: | :----: | :---: | :---: | :---: |
| *Source 2* | *Source 1* | 0      | 2     | 0     | 0     |

* *Source 1*: The first register to compare with *Source 2* (right operand)
* *Source 2*: The second register (left operand)

#### II.1.1.4) Branching

| 31 - 8    | 7 - 6      | 5 - 4 | 3 - 2 | 1 - 0 |
| :-------: | :--------: | :---: | :---: | :---: |
| Dependent | *Category* | 3     | 0     | 0     |

| *Category* value | Category       |
| :--------------: | :------------: |
| 0                | Branch         |
| 1                | Illegal        |
| 2                | Jumps or Calls |
| 3                | RET            |

Depending on the value of *Category* the decoding steps will differ.

##### II.1.1.4.1) Branch

Jumps to the specified label if conditional flag is true for the specified comparator. Otherwise it is no-op.

| 31 - 26 | 25 - 12 | 11 - 8       | 7 - 6 | 5 - 4 | 3 - 2 | 1 - 0 |
| :-----: | :-----: | :----------: | :---: | :---: | :---: | :---: |
| 0       | *Label* | *Comparator* | 0     | 3     | 0     | 0     |

| *Comparator* value | Comparator       | Notes         | Mnemonic |
| :----------------: | :--------------: | :-----------: | :------: |
| 0                  | Not equal        |               | BNE      |
| 1                  | Equal            |               | BEQ      |
| 2                  | Less             |               | BL       |
| 3                  | Less or equal    |               | BLE      |
| 4                  | Greater          |               | BG       |
| 5                  | Greater or equal |               | BGE      |
| 6                  | Less             | Sign-extended | BLS      |
| 7                  | Less or equal    | Sign-extended | BLES     |
| 8                  | Greater          | Sign-extended | BGS      |
| 9                  | Greater or equal | Sign-extended | BGES     |
| 10-15              | Illegal          |               |          |

* *Label*: the address of the instruction to jump to in ISRAM
* *Comparator*: the logical operation to perform 

###### II.1.1.4.1.1) Float branch

`fcmp` and `dcmp` modifies the signed (`S`) and equality (`Z`) flags.
`fbcc` and `dbcc` generates `bccs` instruction, where `cc` is the comparator (`L`, `LE`, `G`, `GE`)

##### II.1.1.4.2) Jumps or Calls

Jumps to the specified label, based on absolute or relative address.

| 31 - 26 | 25 - 12 | 11 - 10 | 9 - 8     | 7 - 6 | 5 - 4 | 3 - 2 | 1 - 0 |
| :-----: | :-----: | :-----: | :-------: | :---: | :---: | :---: | :---: |
| 0       | *Label* | 0       | *Subtype* | 2     | 3     | 0     | 0     |

| *Subtype* value | Instruction   | Mnemonic |
| :-------------: | :-----------: | :------: |
| 0               | Call          | CALL     |
| 1               | Jump          | JMP      |
| 2               | Call relative | CALLR    |
| 3               | Jump relative | JMPR     |

* *Label*: the address, absolute or relative, of the instruction to jump to in ISRAM
* *Subtype*: the logical operation to perform 

##### II.1.1.4.3) RET

Jump to the next instruction following the last call.

| 31 - 8 | 7 - 6 | 5 - 4 | 3 - 2 | 1 - 0 |
| :----: | :---: | :---: | :---: | :---: |
| 0      | 3     | 3     | 0     | 0     |

### II.1.2) CMPI

Update the comparison flags based on integer comparison.

| 31 - 26  | 25 - 6      | 5 - 4  | 3 - 2 | 1 - 0 |
| :------: | :---------: | :----: | :---: | :---: |
| *Source* | *Immediate* | *Size* | 1     | 0     |

* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Immediate*: The value to compare with *Source*
* *Source*: The register to be compared with

### II.1.3) FCMPI

Update the comparison flags based on integer comparison.

| 31 - 25  | 24 - 4      | 3 - 2 | 1 - 0 |
| :------: | :---------: | :---: | :---: |
| *Source* | *Immediate* | 2     | 0     |

* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Immediate*: The value to compare with *Source* (21-bits).
* *Source*: The register to be compared with.

*Immediate* has a non-standard IEEE-754 format:

| 20     | 19 - 12    | 11 - 0     |
| :----: | :--------: | :--------: |
| *Sign* | *Exponent* | *Mantissa* |

*Mantissa*: 12-bits mantissa
*Exponent*: 8-bits exponent 
*Sign*: 0 is positive, 1 is negative

To make a full single-precision ieee-754 floating point value the *Immediate* value simply need to be copied in the higher bits of the floating point value.

### II.1.4) DCMPI

Update the comparison flags based on integer comparison.

| 31 - 26  | 25 - 4      | 3 - 2 | 1 - 0 |
| :------: | :---------: | :---: | :---: |
| *Source* | *Immediate* | 3     | 0     |

* *Immediate*: The value to compare with *Source* (22-bits).
* *Source*: The register to be compared with.

*Immediate* has a non-standard IEEE-754 format:

| 21     | 20 - 10    | 9 - 0      |
| :----: | :--------: | :--------: |
| *Sign* | *Exponent* | *Mantissa* |

*Mantissa*: 10-bits mantissa
*Exponent*: 11-bits exponent 
*Sign*: 0 is positive, 1 is negative

To make a full double-precision ieee-754 floating point value the *Immediate* value simply need to be copied in the higher bits of the floating point value.

## II.2) LSU

| 31 - 4    | 3 - 2   | 1 - 0 |
| :-------: | :-----: | :---: |
| Dependent | *Type*  | 1     |

Depending on the value of *Type* the decoding steps will differ.

| *Type* value | Instruction        |
| :----------: | :----------------: |
| 0            | LDM/STM            |
| 1            | *Subtypes*         |
| 2            | LDC/STC            |
| 3            | LDF/STF or LDD/STD |

### II.2.1) LDM/STM

Load or store memory in DSRAM

| 31 - 26       | 25 - 20   | 19 - 8      | 7 - 6   | 5       | 4                | 3 - 2   | 1 - 0 |
| :-----------: | :-------: | :---------: | :-----: | :-----: | :--------------: | :-----: | :---: |
| *Register*    | *Source*  | *Immediate* | *Size*  | *Store* | *Incrementation* | 0       | 1     |

* *Incrementation*: if 1 then the value of *Source* will be incremented after the operation.
* *Store*: if 1 then the operation is STM (store-memory), otherwise it's LDM (load-memory).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Immediate*: base address (12-bits) of the memory to be loaded or the base address of the memory destination in DSRAM.
* *Source*: a register containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
ldm r3, 128(r60+) -> 0000'1111 1100'0000 1000'0000 1101'0001
stm.w r4, 2(r30)  -> 0001'0001 1110'0000 0000'0010 0110'0001
```

### II.2.2) *Subtypes*

| 31 - 6    | 5 - 4     | 3 - 2   | 1 - 0 |
| :-------: | :-------: | :-----: | :---: |
| Dependent | *Subtype* | 2       | 1     |

| *Subtype* value      | Instruction      |
| :------------------: | :--------------: |
| 0                    | LDMX/STMX        |
| 1                    | IN/OUT           |
| 2                    | INI/OUTI         |
| 3                    | LDV/STV          |

Depending on the value of *Subtype* the decoding steps will differ.

#### II.2.2.1) LDMX/STMX

Load or store memory in DSRAM (extended)

| 31 - 26    | 25       | 24 - 9      | 8 - 7  | 6       | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :------: | :---------: | :----: | :-----: | :---: | :---: | :---: |
| *Register* | *Source* | *Immediate* | *Size* | *Store* | 0     | 1     | 1     |

* *Store*: if 1 then the operation is STMX (store-memory), otherwise it's LDMX (load-memory).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Immediate*: base address (16-bits) of the memory to be loaded or the base address of the memory destination in DSRAM.
* *Source*: a register, either r62 (0) or r63 (1), containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
ldmx.b r1,$3FFF[r63] -> 0000'0100 0111'1111 1111'1110 0000'0101
stmx.w r3,$0FFF[r62] -> 0000'1110 0001'1111 1111'1110 1100'0101
```

#### II.2.2.2) IN/OUT

Load or store memory in IOSRAM

| 31 - 26    | 25 - 24 | 23 - 16   | 15 - 9 | 8 - 7  | 6       | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :-----: | :-------: | :----: | :----: | :-----: | :---: | :---: | :---: |
| *Register* | 0       | *Address* | 0      | *Size* | *Store* | 1     | 1     | 1     |

* *Store*: if 1 then the operation is OUT (store-memory), otherwise it's IN (load-memory).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Address*: base address (8-bits) of the memory to be loaded or the base address of the memory destination in IOSRAM.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
in.b 42, r5 -> 0001'0100 0010'1010 0000'0000 0001'0101
out.w 2, r2 -> 0000'1000 0000'0010 0000'0000 1101'0101
```

#### II.2.2.3) OUTI

Store memory in IOSRAM (immediate)

| 31 - 24   | 23 - 8      | 7      | 6   | 5 - 4 | 3 - 2 | 1 - 0 |
| :-------: | :---------: | :----: | :-: | :---: | :---: | :---: |
| *Address* | *Immediate* | *Size* | 0   | 2     | 1     | 1     |

* *Size*: if 0, then one byte is stored, if 1, then 2 bytes are stored.
* *Immediate*: a value (16-bits), it represents the value to be written. 
* *Address*: base address (8-bits) of the memory to be loaded or the base address of the memory destination in IOSRAM.

Examples:
```
outi.w 4, $03FF -> 0000'0100 0000'0011 1111'1111 1001'0101
```

#### II.2.2.4) LDV/STV

Load or store memory in SDRAM (vec4f (4 * ieee-single-precision) version)

To be determined

### II.2.3) LDC/STC

Load or store memory in cache

| 31 - 26    | 25 - 20  | 19 - 8      | 7 - 6  | 5       | 4                | 3 - 2 | 1 - 0 |
| :--------: | :------: | :---------: | :----: | :-----: | :--------------: | :---: | :---: |
| *Register* | *Source* | *Immediate* | *Size* | *Store* | *Incrementation* | 2     | 1     |

* *Incrementation*: if 1 then the value of *Source* will be incremented after the operation.
* *Store*: if 1 then the operation is STC (store-cache), otherwise it's LDM (load-cache).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Immediate*: base address of the memory to be loaded or the base address of the memory destination in cache.
* *Source*: a register containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
ldc r3, $80[r60+]  -> 0000'1111 1100'0000 1000'0000 1101'1001
stc.w r4, $02[r30] -> 0001'0001 1110'0000 0000'0010 0110'1001
```

### II.2.4) LDF/STF and LDD/STD

Load or store memory in SDRAM (float (ieee-single-precision) and double (ieee-double-precision) version)

To be determined

## II.3) ALU

| 31 - 4    | 3 - 2      | 1 - 0 |
| :-------: | :--------: | :---: |
| Dependent | *Category* | 2     |

| *Category* value | Instruction category       |
| :--------------: | :------------------------: |
| 0                | NOP or XCHG or REG-REG-REG |
| 1                | REG-REG-IMM                |
| 2                | REG-IMM                    |
| 3                | MOVEI                      |

Depending on the value of *Category* the decoding steps will differ.

### II.3.1) NOP and XCHG

| 31 - 7    | 6 - 4  | 3 - 2 | 1 - 0 |
| :-------: | :---:  | :---: | :---: |
| Dependent | *Type* | 0     | 2     |

| *Type* value  | Instruction |
| :-----------: | :---------: |
| 0             | REG-REG-REG |
| 1             | Illegal     |
| 2             | XCHG        |
| 3 - 5         | Illegal     |
| 6             | NOP         |
| 7             | Illegal     |

Depending on the value of *Type* the decoding steps will differ.

#### II.3.1.1) XCHG

Change the amount of instructions decoded each cycle. This instruction flip the XCHG bit.
If the XCHG bit is 1, 4 instructions are decoded each cycle, if it is 0, 2 instructions are decoded each cycle.
This is useful to avoid too many "nop" and therefore not waste unnecessary space in the ISRAM.

| 31 - 7 | 6 - 4 | 3 - 2 | 1 - 0 |
| :----: | :---: | :---: | :---: |
| 0      | 2     | 0     | 2     |

#### II.3.1.2) NOP

Does nothing. May be used to add cycle-accurate delay and also used to indicate end-of-program (mov.e).

| 31 - 8 | 7     | 6 - 4 | 3 - 2 | 1 - 0 |
| :----: | :---: | :---: | :---: | :---: |
| 0      | *End* | 6     | 0     | 2     |

* *End*: if 1 then we reached the end of the program, otherwise this does nothing.

### II.3.2) Arithmetic and bitwise instructions

All arithmetic and bitwise instructions can be used with one of the 3 operand configurations.

##### REG-REG-REG

`OP R1, R2, R3` where `OP` the operation (see below), `R1`, `R2` and `R3` are general-use registers.

`R1` is where the result will be stored.
`R2` and `R3` are the operands. 
Pseudo-code equivalent: `R1 <- R2 OP R3`

| 31 - 26       | 25 - 20    | 19 - 14    | 13 - 12 | 11 - 8 | 7 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :--------: | :--------: | :-----: | :----: | :---: | :---: | :---: |
| *Destination* | *Source 2* | *Source 1* | *Size*  | *OP*   | 0     | 0     | 2     |

* *OP*: The operation (see below)
* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Source 1*: the first operand register
* *Source 2*: the second operand register
* *Destination*: the register in which the result is stored

##### REG-REG-IMM

`OP R1, R2, I` where OP the operation (see below), `R1`, `R2` are general-use registers, `I` is a value.
All REG-REG-IMM instructions suffixed by `i` (`OPi`).

`R1` is where the result will be stored. 
`R2` and `I` are the operands.
Pseudo-code equivalent: `R1 <- R2 OP I`

| 31 - 26       | 25 - 20    | 19 - 10     | 9 - 8  | 7 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :--------: | :---------: | :----: | :---: | :---: | :---: |
| *Destination* | *Source 2* | *Immediate* | *Size* | *OP*  | 1     | 2     |

* *OP*: The operation (see below)
* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Immediate*: the first operand value (10-bits)
* *Source 2*: the second operand register
* *Destination*: the register in which the result is stored

##### REG-IMM

`OP R, I` where OP the operation (see below), `R` is a general-use register, `I` is a value.
All REG-IMM instructions are suffixed by `q` (`OPq`).

`R` is where the result will be stored.
`R` and `I` is the second operand.
Pseudo-code equivalent: `R <- R OP I`

| 31 - 26       | 25 - 10     | 9 - 8  | 7 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :---------: | :----: | :---: | :---: | :---: |
| *Destination* | *Immediate* | *Size* | *OP*  | 2     | 2     |

* *OP*: The operation (see below)
* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Immediate*: the first operand value (16-bits)
* *Destination*: the register in which the result is stored

##### Operations

| Opcode | OP   | Name                           | Notes         |
| :----: | :--: | :----------------------------: | :-----------: |
| 0      | ADD  | Addition                       |               |
| 1      | SUB  | Substraction                   |               |
| 2      | MULS | Multiplication                 | Sign-extended |
| 3      | MULU | Multiplication                 |               |
| 4      | DIVS | Division                       | Sign-extended |
| 5      | DIVU | Division                       |               |
| 6      | AND  | Bitwise AND                    |               |
| 7      | OR   | Bitwise OR                     |               |
| 8      | XOR  | Bitwise XOR                    |               |
| 9      | ASL  | Arithmetic bitwise left shift  | Sign-extended |
| 10     | LSL  | Logical bitwise left shift     |               |
| 11     | ASR  | Arithmetic bitwise right shift | Sign-extended |
| 12     | LSR  | Logical bitwise right shift    |               |
| 13     | ILL  | Illegal                        |               |
| 14     | ILL  | Illegal                        |               |
| 15     | ILL  | Illegal                        |               |

### II.3.3) MOVEI

Write a value in a register.

| 31 - 26        | 25 - 4      | 3 - 2 | 1 - 0 |
| :------------: | :---------: | :---: | :---: |
| *Destination*  | *Immediate* | 3     | 2     |

* *Immediate*: the value to be written (22-bits)
* *Destination*: the register in which the value is stored

### II.3.4) MOVE

Write the value of a register in another one.

`move r1, r2` is a syntaxic sugar. It generate the same opcode as `addi r1, r2, 0`.

## II.4) AGU

| 31 - 3    | 2          | 1 - 0 |
| :-------: | :--------: | :---: |
| Dependent | *Category* | 2     |

| *Category* value | Instruction category |
| :--------------: | :------------------: |
| 0                | Direct load/store    |
| 1                | Load/store list      |

Depending on the value of *Category* the decoding steps will differ.

### II.4.1) Direct load/store

Direct transfer between DSRAM and RAM

| 31 - 20      | 19 - 8     | 7               | 6 - 5         | 4      | 3       | 2   | 1 - 0 |
| :----------: | :--------: | :-------------: | :-----------: | :----: | :-----: | :-: | :---: |
| *DSRAM Base* | *RAM Base* | *DSRAM address* | *RAM address* | *Size* | *Store* | 0   | 2     |

* *Store*: if 0, then it is a transfer from RAM to SDRAM, if 1, then it is a tranfers from SDRAM to RAM.
* *Size*: if 0, then transfer 32 bytes, if 1, then transfer 64 bytes.
* *RAM address*: a register, this value is added to *RAM Base* to compute the final address in RAM.
* *DSRAM address*: a register, this value is added to *DSRAM Base* to compute the final address in DSRAM.
* *RAM Base*: a value, it is added to *RAM address* to compute the final address in RAM.
* *DSRAM Base*: a value, it is added to *DSRAM address* to compute the final address in DSRAM.

The address in RAM is multiplied by 32, so the final address, in bytes, is `(RAM address + RAM Base) * 32`.
The address in DSRAM is multiplied by 32, so the final address, in bytes, is `(DSRAM address + DSRAM Base) * 32`

### II.4.2) Load/store list

| 31 - 8    | 7 - 4  | 3       | 2   | 1 - 0 |
| :-------: | :----: | :-----: | :-: | :---: |
| Dependent | *Type* | *Store* | 1   | 2     |

| *Type* value | Instruction   |
| :----------: | :-----------: |
| 0            | LDDMAR/STDMAR |
| 1            | DMAIR         |
| 2 - 14       | Illegal       |
| 15           | WAIT          |

Depending on the value of *Type* the decoding steps will differ.

* *Store*: meaning and possible values of this variable differ depending on the value of *Type*.

### II.4.2.1) LDDMAR/STDMAR

Transfer blocs of data between RAM and DSRAM.

| 31 - 26       | 25 - 20  | 19 - 8 | 7 - 4  | 3       | 2   | 1 - 0 |
| :-----------: | :------: | :----: | :----: | :-----: | :-: | :---: |
| *Destination* | *Source* | *Size* | 0      | *Store* | 1   | 2     |

* *Store*: if 0, then it is a transfer from RAM to SDRAM, if 1, then it is a tranfers from SDRAM to RAM.
* *Size*: the amount of data to transfer, the total size in bytes is `32 * Size`.
* *Source*: a register, its value is the base address in the source memory (if *Store* is 0 source memory is RAM, otherwise it is DSRAM), the final address is `32 * Source` in bytes.
* *Destination*: a register, its value is the base address in the destination memory (if *Store* is 0 source memory is DSRAM, otherwise it is RAM), the final address is `32 * Destination` in bytes.

### II.4.2.2) DMAIR

Transfer blocs of data from RAM and ISRAM.

| 31 - 26       | 25 - 20  | 19 - 8 | 7 - 4  | 3       | 2   | 1 - 0 |
| :-----------: | :------: | :----: | :----: | :-----: | :-: | :---: |
| *Destination* | *Source* | *Size* | 0      | *Store* | 1   | 2     |

* *Store*: must be 0 (1 is illegal), it is always a transfer from RAM to ISRAM.
* *Size*: the amount of data to transfer, the total size in bytes is `32 * Size`.
* *Source*: a register, its value is the base address in the RAM, the final address is `32 * Source` in bytes.
* *Destination*: a register, its value is the base address in the ISRAM, the final address is `32 * Destination` in bytes.

### II.4.2.3) WAIT

Does nothing (it is the nop-like of the AGU)

| 31 - 8 | 7 - 4 | 3   | 2   | 1 - 0 |
| :----: | :---: | :-: | :-: | :---: |
| 0      | 15    | 0   | 1   | 2     |

## II.5) VFPU

## II.6) VDIV
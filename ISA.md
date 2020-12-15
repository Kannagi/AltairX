# Table of Contents

* [I) General opcode structure](#i-general-opcode-structure)
  * [I.1) Compute units](#i1-compute-units)
  * [I.2) Basic opcode structure](#i2-basic-opcode-structure)
* [II) Compute units opcodes](#ii-compute-units-opcodes)
  * [II.1) BRU](#ii1-bru)
  * [II.2) LSU](#ii2-lsu)
  * [II.3) ALU](#ii3-alu)
  * [II.4) AGU](#ii4-agu)
  * [II.5) VFPU](#ii5-vfpu)

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

Each cycle 2 or 4 opcodes are decoded. Each opcode as an index in the range [0; 3], which is its index in the 2 (or 4) uint32 opcodes buffer. Compute units are bound to some index, you can only use some compute units on some indices.

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

* *Label*: the address, relative to current program counter, of the instruction to jump to in ISRAM. The total incrementation of the program counter, in bytes, is `Label * 8`. This value is sign-extended.
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

* *Label*: the address, absolute or relative, of the instruction to jump to in ISRAM. The final address, in bytes, is `Label * 8`. If the jump is relative, the value is sign-extended.
* *Subtype*: the logical operation to perform.

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

| *Type* value | Instruction                                      |
| :----------: | :----------------------------------------------: |
| 0            | DSRAM load/store                                 |
| 1            | *Subtype*                                        |
| 2            | Cache load/store                                 |
| 3            | DSRAM or cache load/store (for floating points)  |

Depending on the value of *Type* and *Subtype* the decoding steps will differ.

### II.2.1) DSRAM load/store

Load or store memory from/in DSRAM

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

### II.2.2) *Subtype*

| 31 - 6    | 5 - 4     | 3 - 2   | 1 - 0 |
| :-------: | :-------: | :-----: | :---: |
| Dependent | *Subtype* | 2       | 1     |

| *Subtype* value | Instruction                              |
| :-------------: | :--------------------------------------: |
| 0               | DSRAM load/store extended                |
| 1               | IN/OUT                                   |
| 2               | OUTI                                     |
| 3               | DSRAM or cache load/store (for vectors)  |

Depending on the value of *Subtype* the decoding steps will differ.

#### II.2.2.1) DSRAM load/store extended

Load or store memory from/in DSRAM (extended version)

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

Load or store memory from/in IOSRAM

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

#### II.2.2.4) DSRAM or cache load/store (for vectors)

Load or store memory from/in SDRAM or cache (vectors (4x IEEE 32-bits float) version)

| 31 - 27    | 26 - 24   | 23 - 9         | 8                | 7       | 6       | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :-------: | :------------: | :--------------: | :-----: | :-----: | :---: | :---: | :---: |
| *Register* | *Source*  | *Base address* | *Incrementation* | *Cache* | *Store* | 3     | 1     | 1     |

* *Store*: if 1 then the operation is STMV or STCV (store-memory), otherwise it's LDMV or LDCV (load-memory).
* *Cache*: if 1 then the transfer occurs between cache and register, if 0, the transfer occurs between SDRAM and register.
* *Incrementation*: if 1, then *Address* will be incremented after this operation
* *Base address*: base address (15-bits) of the memory to be loaded or the base address of the memory destination in DSRAM or in cache.
* *Source*: a register, in range r56-63, containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written.

### II.2.3) Cache load/store

Load or store memory from/in cache

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

### II.2.4) DSRAM or cache load/store (for floating points)

Load or store memory from/in SDRAM or cache.

| 31 - 5    | 4      | 3 - 2 | 1 - 0 |
| :-------: | :----: | :---: | :---: |
| Dependent | *Size* | 2     | 1     |

| *Size* value | Instruction                     |
| :----------: | :-----------------------------: |
| 0            | LDMF/STMF or LDCF/STCF (float)  |
| 1            | LDMD/STMD or LDCD/STCD (double) |

Depending on the value of *Size* the decoding steps will differ.

#### II.2.4.1) LDMF/STMF or LDCF/STCF

Load or store memory from/in SDRAM or cache (float (IEEE 32-bits) version).

| 31 - 25    | 24 - 23  | 22 - 8         | 7                | 6       | 5       | 4   | 3 - 2 | 1 - 0 |
| :--------: | :------: | :------------: | :--------------: | :-----: | :-----: | :-: | :---: | :---: |
| *Register* | *Source* | *Base address* | *Incrementation* | *Store* | *Cache* | 0   | 2     | 1     |

* *Cache*: if 1 then the transfer occurs between cache and register, if 0, the transfer occurs between SDRAM and register.
* *Store*: if 1 then the operation is STMF or STCF (store-memory), otherwise it's LDMF or LDCF (load-memory).
* *Incrementation*: if 1, then *Address* will be incremented after this operation
* *Base address*: base address (15-bits) of the memory to be loaded or the base address of the memory destination in DSRAM or in cache.
* *Source*: a register, in range r60-63, containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written.

#### II.2.4.2) LDMD/STMD or LDCD/STCD

Load or store memory from/in SDRAM or cache (double (IEEE 64-bits) version).

| 31 - 26    | 25 - 24  | 23 - 8         | 7                | 6       | 5       | 4   | 3 - 2 | 1 - 0 |
| :--------: | :------: | :------------: | :--------------: | :-----: | :-----: | :-: | :---: | :---: |
| *Register* | *Source* | *Base address* | *Incrementation* | *Store* | *Cache* | 1   | 2     | 1     |

* *Cache*: if 1 then the transfer occurs between cache and register, if 0, the transfer occurs between SDRAM and register.
* *Store*: if 1 then the operation is STMD or STCD (store-memory), otherwise it's LDMD or LDCD (load-memory).
* *Incrementation*: if 1, then *Address* will be incremented after this operation
* *Base address*: base address (16-bits) of the memory to be loaded or the base address of the memory destination in DSRAM or in cache.
* *Source*: a register, in range r60-63, containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written.

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

### II.4.1) Direct load/store (LDDMA/STDMA)

Direct transfer between DSRAM and RAM

| 31 - 20    | 19 - 8       | 7             | 6 - 5           | 4      | 3       | 2   | 1 - 0 |
| :--------: | :----------: | :-----------: | :-------------: | :----: | :-----: | :-: | :---: |
| *RAM Base* | *DSRAM Base* | *RAM address* | *DSRAM address* | *Size* | *Store* | 0   | 2     |

* *Store*: if 0, then it is a transfer from RAM to SDRAM, if 1, then it is a tranfers from SDRAM to RAM.
* *Size*: if 0, then transfer 32 bytes, if 1, then transfer 64 bytes.
* *DSRAM address*: a register, this value is added to *DSRAM Base* to compute the final address in DSRAM. The real register is `rX` where `X = 60 + DSRAM address`.
* *RAM address*: a register, this value is added to *RAM Base* to compute the final address in RAM. The real register is `rX` where `X = 58 + RAM address`.
* *DSRAM Base*: a value, it is added to *DSRAM address* to compute the final address in DSRAM.
* *RAM Base*: a value, it is added to *RAM address* to compute the final address in RAM.

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

#### II.4.2.1) LDDMAR/STDMAR

Transfer blocs of data between RAM and DSRAM.

| 31 - 26         | 25 - 20       | 19 - 8 | 7 - 4  | 3       | 2   | 1 - 0 |
| :-------------: | :-----------: | :----: | :----: | :-----: | :-: | :---: |
| *DSRAM address* | *RAM address* | *Size* | 0      | *Store* | 1   | 2     |

* *Store*: if 0, then it is a transfer from RAM to SDRAM, if 1, then it is a tranfers from SDRAM to RAM.
* *Size*: the amount of data to transfer, the total size in bytes is `32 * Size`.
* *RAM address*: a register, its value is the base address in RAM, the final address, in bytes, is `32 * RAM address`.
* *DSRAM address*: a register, its value is the base address in DSRAM, the final address, in bytes, is `32 * DSRAM address`.

#### II.4.2.2) DMAIR

Transfer blocs of data from RAM and ISRAM.

| 31 - 26         | 25 - 20       | 19 - 8 | 7 - 4  | 3       | 2   | 1 - 0 |
| :-------------: | :-----------: | :----: | :----: | :-----: | :-: | :---: |
| *ISRAM address* | *RAM address* | *Size* | 1      | *Store* | 1   | 2     |

* *Store*: must be 0 (1 is illegal), it is always a transfer from RAM to ISRAM.
* *Size*: the amount of data to transfer, the total size in bytes is `32 * Size`.
* *RAM address*: a register, its value is the base address in the RAM, the final address, in bytes, is `32 * RAM address`.
* *ISRAM address*: a register, its value is the base address in the ISRAM, the final address, in bytes, is `32 * ISRAM address`.

#### II.4.2.3) WAIT

Blocks execution until the end of previous transfers.

| 31 - 8 | 7 - 4 | 3   | 2   | 1 - 0 |
| :----: | :---: | :-: | :-: | :---: |
| 0      | 15    | 0   | 1   | 2     |

## II.5) VFPU

| 31 - 4    | 3 - 2   | 1 - 0 |
| :-------: | :-----: | :---: |
| Dependent | *Type*  | 3     |

| *Type* value | Instruction |
| :----------: | :---------: |
| 0            | *Subtype*   |
| 1            | MOVEFI      |
| 2            | MOVEDI      |
| 3            | MOVEVI      |

Depending on the value of *Type* and *Subtype* the decoding steps will differ.

### II.5.1) *Subtype*

| 31 - 6    | 5 - 4     | 3 - 2 | 1 - 0 |
| :-------: | :-------: | :---: | :---: |
| Dependent | *Subtype* | 0     | 3     |

| *Subtype* value | Instruction                  |
| :-------------: | :--------------------------: |
| 0               | Arithmetic                   |
| 1               | Moves                        |
| 2               | Float conversion             |
| 3               | Float/int conversion or VDIV |

Depending on the value of *Subtype* the decoding steps will differ.

#### II.5.1.1) Arithmetic

| 31 - 10   | 9 - 8      | 7 - 6     | 5 - 4 | 3 - 2 | 1 - 0 |
| :-------: | :--------: | :-------: | :---: | :---: | :---: |
| Dependent | *Category* | Dependent | 0     | 0     | 3     |

| *Category* value | Instruction                           |
| :--------------: | :-----------------------------------: |
| 0                | vector/vector ADD/SUB/MUL/MULADD      |
| 1                | vector/float ADD/SUB/MUL/MULADD       |
| 2                | vector accumulator MUL/MULADD or FIPR |
| 3                | double/double ADD/SUB/MUL/MULADD      |

Depending on the value of *Category* the decoding steps will differ.

##### Float operations

Each operation set, except vector accumulator MUL/MULADD, support addition, substraction, multiplication, and multiply-add 

| *Operation* value | Instruction | Notes         |
| :---------------: | :---------: | :-----------: |
| 0                 | FADD        | v0 =  v1 + v2 |
| 1                 | FSUB        | v0 =  v1 - v2 |
| 2                 | FMUL        | v0 =  v1 * v2 |
| 3                 | FMULADD     | v0 += v1 * v2 |

##### II.5.1.1.1) vector/vector ADD/SUB/MUL/MULADD

These operations are applied using the following pattern: 
* `x0 = x1 OP x2`
* `y0 = y1 OP y2`
* `z0 = z1 OP z2`
* `w0 = w1 OP w2`

| 31 - 27       | 26 - 22    | 21 - 17    | 16 - 12 | 11 - 10     | 9 - 8 | 7 - 6  | 5 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :--------: | :--------: | :-----: | :---------: | :---: | :----: | :---: | :---: | :---: |
| *Destination* | *Source 1* | *Source 2* | 0       | *Operation* | 0     | *Size* | 0     | 0     | 3     |

* *Size*: this operations affects the *Size* first components of both vectors. `0 = x | 1 = xy | 2 = xyz | 3 = xyzw`.
* *Operation*: the arithmetic *Operation* to be done (see [Float operations](#float-operations)).
* *Source 2*: the second operand, a vector register.
* *Source 1*: the first operand, a vector register.
* *Destination*: the destination, a vector register.

##### II.5.1.1.2) vector/float ADD/SUB/MUL/MULADD

These operations are applied using the following pattern: 
* `x0 = x1 OP f2`
* `y0 = y1 OP f2`
* `z0 = z1 OP f2`
* `w0 = w1 OP f2`

| 31 - 27       | 26 - 22    | 21 - 15    | 14 - 12 | 11 - 10     | 9 - 8 | 7 - 6  | 5 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :--------: | :--------: | :-----: | :---------: | :---: | :----: | :---: | :---: | :---: |
| *Destination* | *Source 1* | *Source 2* | 0       | *Operation* | 1     | *Size* | 0     | 0     | 3     |

* *Size*: this operations affects the *Size* first components of *Source 1* vector. `0 = x | 1 = xy | 2 = xyz | 3 = xyzw`.
* *Operation*: the arithmetic *Operation* to be done (see [Float operations](#float-operations)).
* *Source 2*: the second operand, a float register.
* *Source 1*: the first operand, a vector register.
* *Destination*: the destination, a vector register.

##### II.5.1.1.3) vector accumulator MUL/MULADD and FIPR

The 4 following operations does **not** follow the [Float operations](#float-operations).

| 31 - 27    | 26 - 22    | 21 - 15 | 14 - 12 | 11 - 10      | 9 - 8 | 7 - 6  | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :--------: | :-----: | :-----: | :----------: | :---: | :----: | :---: | :---: | :---: |
| *Vector 2* | *Vector 1* | *Float* | 0       | *Operation2* | 2     | *Size* | 0     | 0     | 3     |

* *Size*: this operations affects the *Size* first components of vectors. `0 = x | 1 = xy | 2 = xyz | 3 = xyzw`.
* *Float*: a float register.
* *Vector 1*: a vector register.
* *Vector 2*: a vector register.

| *Operation2* value | Instruction  | Expression                                         | *Destination* |
| :----------------: | :----------: | :------------------------------------------------: | :-----------: |
| 0                  | FMULVA       | *Accumulator* = *Vector 1* \* *Float*              | *Accumulator* |
| 1                  | FMULADDVA    | *Accumulator* += *Vector 1* \* *Float*             | *Accumulator* |
| 2                  | FMULADDVAO   | *Vector 2* = *Accumulator* + *Vector 1* \* *Float* | *Vector 2*    |
| 3                  | FIPR         | *Float* =  *Vector 1* · *Vector 2*                 | *Float*       |

##### II.5.1.1.4) double/double ADD/SUB/MUL/MULADD

These operations are applied using the following pattern: 
* `d0 = d1 OP d2`

| 31 - 26       | 25 - 20    | 19 - 14    | 16 - 10 | 9 - 8 | 7 - 6       | 5 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :--------: | :--------: | :-----: | :---: | :---------: | :---: | :---: | :---: |
| *Destination* | *Source 1* | *Source 2* | 0       | 3     | *Operation* | 0     | 0     | 3     |

* *Operation*: the arithmetic *Operation* to be done (see [Float operations](#float-operations)).
* *Source 2*: the second operand, a double register.
* *Source 1*: the first operand, a double register.
* *Destination*: the destination, a double register.

#### II.5.1.2) Moves

To be determined.

#### II.5.1.3) Float conversion

| 31 - 26  | 25 - 19 | 18 - 7 | 6           | 5 - 4 | 3 - 2 | 1 - 0 |
| :------: | :-----: | :----: | :---------: | :---: | :---: | :---: |
| *Double* | *Float* | 0      | *Direction* | 2     | 0     | 3     |

* *Direction*: if 0, convert *Float* to *Double*, if 1, convert *Double* to *Float*.
* *Float*: the float register.
* *Double*: the double register.

#### II.5.1.4) Float/Int conversion or VDIV

| 31 - 10   | 9 - 8       | 7 - 6     | 5 - 4 | 3 - 2 | 1 - 0 |
| :-------: | :---------: | :-------: | :---: | :---: | :---: |
| Dependent | *Operation* | Dependent | 3     | 0     | 3     |

| *Operation* value | Instruction                           |
| :---------------: | :-----------------------------------: |
| 0                 | Fixed-point conversion                |
| 1                 | Float/int conversion                  |
| 2                 | Double/int conversion                 |
| 3                 | Float/Double division and square-root |

Depending on the value of *Operation* the decoding steps will differ.

##### II.5.1.4.1) Fixed-point conversion

Fixed-point values from/to floating-point values.

| 31 - 26    | 25 - 21  | 20 - 15 | 14 - 12       | 11 - 10 | 9 - 8  | 7 - 6  | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :------: | :-----: | :-----------: | :-----: | :----: | :----: | :---: | :---: | :---: |
| *Register* | *Vector* | 0       | *Instruction* | 0       | 0      | *Size* | 3     | 0     | 3     |

| *Instruction* value | Instruction | Notes                                              |
| :-----------------: | :---------: | :------------------------------------------------: |
| 0                   | ITOF0       | Convert 16.0 fixed-point to 32-bits floating-point |
| 1                   | ITOF4       | Convert 12.4 fixed-point to 32-bits floating-point |
| 2                   | ITOF8       | Convert 8.8 fixed-point to 32-bits floating-point  |
| 3                   | ITOF15      | Convert 1.15 fixed-point to 32-bits floating-point |
| 4                   | FTOI0       | Convert 32-bits floating-point to 16.0 fixed-point |
| 5                   | FTOI4       | Convert 32-bits floating-point to 12.4 fixed-point |
| 6                   | FTOI8       | Convert 32-bits floating-point to 8.8 fixed-point  |
| 7                   | FTOI15      | Convert 32-bits floating-point to 1.15 fixed-point |

* *Size*: this operations affects the *Size* first components of *Vector*. `0 = x | 1 = xy | 2 = xyz | 3 = xyzw`.
* *Vector*: the floating-point values, as a vector register.
* *Register*: the fixed-point values, as a register.

##### II.5.1.4.2) Float/int conversion

Convert floating-point to integer, or integer to floating-point.

| 31 - 26    | 25 - 19 | 18 - 11 | 10          | 9 - 8 | 7 - 6  | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :-----: | :-----: | :---------: | :---: | :----: | :---: | :---: | :---: |
| *Register* | *Float* | 0       | *Direction* | 1     | *Size* | 3     | 0     | 3     |

* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Direction*: if 0, then convert *Register* to *Float*, if 1, then convert *Float* to *Register*.
* *Float*: a float register.
* *Register*: a register.

##### II.5.1.4.3) Double/int conversion

Convert floating-point to integer, or integer to floating-point.

| 31 - 26    | 25 - 20  | 19 - 11 | 10          | 9 - 8 | 7 - 6  | 5 - 4 | 3 - 2 | 1 - 0 |
| :--------: | :------: | :-----: | :---------: | :---: | :----: | :---: | :---: | :---: |
| *Register* | *Double* | 0       | *Direction* | 2     | *Size* | 3     | 0     | 3     |

* *Size*: if 0, then the operation is done on 1 byte, if 1, then the operation is done on 2 byte, if 2 then the operation is done on 4 byte, if 3, then the operation is done on 8 byte.
* *Direction*: if 0, then convert *Register* to *Double*, if 1, then convert *Double* to *Register*.
* *Double*: a double register.
* *Register*: a register.

##### II.5.1.4.4) Float division and square-root

| 31 - 10   | 9 - 8 | 7      | 6             | 5 - 4 | 3 - 2 | 1 - 0 |
| :-------: | :---: | :----: | :-----------: | :---: | :---: | :---: |
| Dependent | 3     | *Size* | *Instruction* | 3     | 0     | 3     |

* *Instruction*: if 0, then it is FDIV or DDIV, if 1, it is FSQRT or DSQRT.
* *Size*: if 0 does either FDIV or FSQRT, if 1, does either DDIV or DSQRT.

Depending on the value of *Operation* the decoding steps will differ.

###### FDIV or FSQRT

| 31 - 25       | 24 - 18    | 17 - 11    | 9 - 8 | 7      | 6             | 5 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :--------: | :--------: | :---: | :----: | :-----------: | :---: | :---: | :---: |
| *Destination* | *Source 2* | *Source 1* | 3     | 0      | *Instruction* | 3     | 0     | 3     |

* *Instruction*: if 0, then it is FDIV, if 1, it is FSQRT.
* *Source 1*: a float register, the left operand.
* *Source 2*: a float register, the right operand.
* *Destination*: a float register, where the result of the operation is gonne be written.

###### DDIV or DSQRT

| 31 - 26       | 25 - 20    | 19 - 14    | 9 - 8 | 7      | 6             | 5 - 4 | 3 - 2 | 1 - 0 |
| :-----------: | :--------: | :--------: | :---: | :----: | :-----------: | :---: | :---: | :---: |
| *Destination* | *Source 2* | *Source 1* | 3     | 1      | *Instruction* | 3     | 0     | 3     |

* *Instruction*: if 0, then it is DDIV, if 1, it is DSQRT.
* *Source 1*: a double register, the left operand.
* *Source 2*: a double register, the right operand.
* *Destination*: a double register, where the result of the operation is gonne be written.

### II.5.2) MOVEFI

Write an immediate value to a float register, useful for common values such as 0, 1...

| 31 - 25       | 24 - 4      | 3 - 2 | 1 - 0 |
| :-----------: | :---------: | :---: | :---: |
| *Destination* | *Immediate* | 1     | 3     |

* *Immediate*: the value to be written in *Destination*, 1 bit of sign, 8 bits of exponent, 12 bits of mantissa.
* *Destination*: a float register.

### II.5.3) MOVEDI

Write an immediate value to a double register, useful for common values such as 0, 1...

| 31 - 26       | 25 - 4      | 3 - 2 | 1 - 0 |
| :-----------: | :---------: | :---: | :---: |
| *Destination* | *Immediate* | 2     | 3     |

* *Immediate*: the value to be written in *Destination*, 1 bit of sign, 11 bits of exponent, 10 bits of mantissa.
* *Destination*: a double register.

### II.5.4) MOVEVI

Write an immediate value to a vector register, useful for common values such as 0, 1...

| 31 - 27       | 26 - 4      | 3 - 2 | 1 - 0 |
| :-----------: | :---------: | :---: | :---: |
| *Destination* | *Immediate* | 3     | 3     |

* *Immediate*: the value to be written in each component of *Destination*, 1 bit of sign, 8 bits of exponent, 14 bits of mantissa.
* *Destination*: a vector register.

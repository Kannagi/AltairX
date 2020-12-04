# I) General opcode structure

Opcodes are 32-bits little-endian unsigned integers.
In this documents, all bitsets are written in natural order (i.e. big-endian, 31 -> 15 -> 7 -> 0) because it's easier and the more common.
Bits fiels bounds are inclusive (3 <-> 0 means both bits 3, 2, 1 and 0)

## I.1) Basic opcode structure

The following table is applicable to **ALL** opcodes

| 31 <-> 2  | 1 <-> 0        |
| :-------: | :------------: |
| Dependent | *Compute Unit* |

Depending on the value of *Compute Unit* the decoding steps will differ.

| *Compute Unit* value | Compute Unit |
| :------------------: | :----------: |
| 0                    | AGU or BRU   |
| 1                    | LSU          |
| 2                    | ALU          |
| 3                    | VFPU         |

***

# II) Compute units opcodes

## II.1) AGU and BRU

<!--to be done-->

## II.2) LSU

| 31 <-> 4  | 3 <-> 2 | 1 <-> 0        |
| :-------: | :-----: | :------------: |
| Dependent | *Type*  | 1              |

Depending on the value of *Type* the decoding steps will differ.

| *Type* value         | Instruction        |
| :------------------: | :----------------: |
| 0                    | LDM/STM            |
| 1                    | LDC/STC            |
| 2                    | *Subtypes*         |
| 3                    | LDF/STF or LDD/STD |

### II.2.1) LDM/STM

Load or store memory in DSRAM

| 31 <-> 26     | 25 <-> 20 | 19 <-> 8    | 7 <-> 6 | 5       | 4                | 3 <-> 2 | 1 <-> 0        |
| :-----------: | :-------: | :---------: | :-----: | :-----: | :--------------: | :-----: | :------------: |
| *Register*    | *Source*  | *Immediate* | *Size*  | *Store* | *Incrementation* | 0       | 1              |

* *Incrementation*: if 1 then the value of *Source* will be incremented after the operation.
* *Store*: if 1 then the operation is STM (store-memory), otherwise it's LDM (load-memory).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Immediate*: base address of the memory to be loaded or the base address of the memory destination in DSRAM.
* *Source*: a register containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
ldm r3, 128(r60+) -> 0000'1111 1100'0000 1000'0000 1101'0001
stm.w r4, 2(r30)  -> 0001'0001 1110'0000 0000'0010 0110'0001
```

### II.2.2) LDC/STC

Load or store memory in cache

| 31 <-> 26     | 25 <-> 20 | 19 <-> 8    | 7 <-> 6 | 5       | 4                | 3 <-> 2 | 1 <-> 0        |
| :-----------: | :-------: | :---------: | :-----: | :-----: | :--------------: | :-----: | :------------: |
| *Register*    | *Source*  | *Immediate* | *Size*  | *Store* | *Incrementation* | 1       | 1              |

* *Incrementation*: if 1 then the value of *Source* will be incremented after the operation.
* *Store*: if 1 then the operation is STC (store-cache), otherwise it's LDM (load-cache).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Immediate*: base address of the memory to be loaded or the base address of the memory destination in cache.
* *Source*: a register containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
ldc r3, $80[r60+] -> 0000'1111 1100'0000 1000'0000 1101'0101
stc.w r4, $02[r30]  -> 0001'0001 1110'0000 0000'0010 0110'0101
```

### II.2.3) *Subtypes*

| 31 <-> 6  | 5 <-> 4   | 3 <-> 2 | 1 <-> 0        |
| :-------: | :-------: | :-----: | :------------: |
| Dependent | *Subtype* | 2       | 1              |

| *Subtype* value      | Instruction      |
| :------------------: | :--------------: |
| 0                    | LDMX/STMX        |
| 1                    | IN/OUT           |
| 2                    | INI/OUTI         |
| 3                    | LDV/STV          |

Depending on the value of *Subtype* the decoding steps will differ.

#### II.2.3.1) LDMX/STMX

Load or store memory in DSRAM (extended)

| 31 <-> 26     | 25        | 24 <-> 9    | 8 <-> 7 | 6       | 5 <-> 4 | 3 <-> 2 | 1 <-> 0        |
| :-----------: | :-------: | :---------: | :-----: | :-----: | :-----: | :-----: | :------------: |
| *Register*    | *Source*  | *Immediate* | *Size*  | *Store* | 0       | 2       | 1              |

* *Store*: if 1 then the operation is STMX (store-memory), otherwise it's LDMX (load-memory).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Immediate*: base address of the memory to be loaded or the base address of the memory destination in DSRAM.
* *Source*: a register, either r62 (0) or r63 (1), containing the address of the memory to be loaded or the address of the memory destination. The value in this register will be added to *Immediate* to compute the final address.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
ldmx.b r1,$3FFF[r63] -> 0000'0100 0111'1111 1111'1110 0000'1001
stmx.w r3,$0FFF[r62] -> 0000'1110 0001'1111 1111'1110 1100'1001
```

#### II.2.3.2) IN/OUT

Load or store memory in IOSRAM

| 31 <-> 26     | 25 <-> 24 | 23 <-> 16   | 15 <-> 9 | 8 <-> 7 | 6       | 5 <-> 4 | 3 <-> 2 | 1 <-> 0        |
| :-----------: | :-------: | :---------: | :------: | :-----: | :-----: | :-----: | :-----: | :------------: |
| *Register*    | 0         | *Address*   | 0        | *Size*  | *Store* | 1       | 2       | 1              |

* *Store*: if 1 then the operation is OUT (store-memory), otherwise it's IN (load-memory).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored, if 2 then 4 bytes are loaded or stored, if 3, then 8 bytes are loaded or stored.
* *Address*: base address of the memory to be loaded or the base address of the memory destination in IOSRAM.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
in.b 42, r5 -> 0001'0100 0010'1010 0000'0000 0001'1001
out.w 2, r2 -> 0000'1000 0000'0010 0000'0000 1101'1001
```

#### II.2.3.3) INI/OUTI

Load or store memory in IOSRAM (immediate)

| 31 <-> 26     | 25 <-> 24 | 23 <-> 16   | 15 <-> 9 | 7      | 6       | 5 <-> 4 | 3 <-> 2 | 1 <-> 0        |
| :-----------: | :-------: | :---------: | :------: | :-----: | :-----: | :-----: | :-----: | :------------: |
| *Register*    | 0         | *Address*   | 0        | *Size*  | *Store* | 1       | 2       | 1              |

* *Store*: if 1 then the operation is OUT (store-memory), otherwise it's IN (load-memory).
* *Size*: if 0, then one byte is loaded or stored, if 1, then 2 bytes are loaded or stored.
* *Address*: base address of the memory to be loaded or the base address of the memory destination in IOSRAM.
* *Register*: a register, in case of load it is where the memory load will be stored, in case of store it represents the value to be written. 

Examples:
```
ini.b 42, r5 -> 0001'0100 0010'1010 0000'0000 0001'1001
outi.w 2, r2 -> 0000'1000 0000'0010 0000'0000 1101'1001
```

#### II.2.3.4) LDV/STV

### II.2.4) LDF/STF and LDD/STD
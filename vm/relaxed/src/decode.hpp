#ifndef ALTAIR_DECODE_INTERNAL_H_DEFINED
#define ALTAIR_DECODE_INTERNAL_H_DEFINED

#include "vm_relaxed.hpp"

int32_t extendSign(uint32_t value, uint32_t bits);

uint32_t readbits(uint32_t buffer, uint32_t index, uint32_t size);

int decodeBRU(uint32_t br, uint32_t pc, uint32_t opcode, ArOperation* AR_RESTRICT output);
int decodeLSU(uint32_t opcode, ArOperation* AR_RESTRICT output);
int decodeALU(uint32_t opcode, ArOperation* AR_RESTRICT output);
int decodeAGU(uint32_t opcode, ArOperation* AR_RESTRICT output);
int decodeVPU(uint32_t opcode, ArOperation* AR_RESTRICT output);

int decode(uint32_t index, uint32_t br, uint32_t pc, uint32_t opcode, ArOperation* AR_RESTRICT output);

#endif

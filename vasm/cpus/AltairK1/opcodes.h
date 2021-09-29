
  //DMA Unit 4 (100)
  "lddma",    {OP_REG,OP_REG,OP_REG},  {K1,(0b00001000)},
  "stdma",    {OP_REG,OP_REG,OP_REG},  {K1,(0b00011000)},
  "lddmacl",  {OP_REG,OP_REG,      },  {K1,(0b00101000)},
  "stdmacl",  {OP_REG,OP_REG,      },  {K1,(0b00111000)},
  "dmai",     {OP_REG,OP_REG,OP_REG},  {K1,(0b01001000)},

  "lddmai",   {OP_REG,OP_REG,OP_IMM},  {K1,(0b10001000)},
  "stdmai",   {OP_REG,OP_REG,OP_IMM},  {K1,(0b10011000)},
  "lddmacli", {OP_REG,OP_IMM,      },  {K1,(0b10101000)},
  "stdmacli", {OP_REG,OP_IMM,      },  {K1,(0b10111000)},
  "dmaii",    {OP_REG,OP_REG,OP_IMM},  {K1,(0b11001000)},

  "wait",     {                    },  {K1,(0b11111000)},

  //LSU SPM Unit 2 (010)
  "ldm",      {OP_REG,OP_RRG,      },  {K1,(0b00000100)},
  "stm",      {OP_REG,OP_RRG,      },  {K1,(0b00010100)},
  "ldmv",     {OP_VRG,OP_RRG,      },  {K1,(0b00100100)},
  "stmv",     {OP_VRG,OP_RRG,      },  {K1,(0b00110100)},

  "ldmi",     {OP_REG,OP_IMR,      },  {K1,(0b01000100)},
  "stmi",     {OP_REG,OP_IMR,      },  {K1,(0b01010100)},
  "ldmvi",    {OP_VRG,OP_IMR,      },  {K1,(0b01100100)},
  "stmvi",    {OP_VRG,OP_IMR,      },  {K1,(0b01110100)},

  "ldml",     {OP_REG,OP_IMM,      },  {K1,(0b10000100)},
  "stml",     {OP_REG,OP_IMM,      },  {K1,(0b10010100)},
  "ldmvl",    {OP_VRG,OP_IMM,      },  {K1,(0b10100100)},
  "stmvl",    {OP_VRG,OP_IMM,      },  {K1,(0b10110100)},

  //LSU Cache Unit 3 (011)
  "ldc",      {OP_REG,OP_RRG,      },  {K1,(0b00000110)},
  "stc",      {OP_REG,OP_RRG,      },  {K1,(0b00010110)},
  "ldcv",     {OP_VRG,OP_RRG,      },  {K1,(0b00100110)},
  "stcv",     {OP_VRG,OP_RRG,      },  {K1,(0b00110110)},

  "ldci",     {OP_REG,OP_IMR,      },  {K1,(0b01000110)},
  "stci",     {OP_REG,OP_IMR,      },  {K1,(0b01010110)},
  "ldcvi",    {OP_VRG,OP_IMR,      },  {K1,(0b01100110)},
  "stcvi",    {OP_VRG,OP_IMR,      },  {K1,(0b01110110)},

  "ldcl",     {OP_REG,OP_IMM,      },  {K1,(0b10000110)},
  "stcl",     {OP_REG,OP_IMM,      },  {K1,(0b10010110)},
  "ldcvl",    {OP_VRG,OP_IMM,      },  {K1,(0b10100110)},
  "stcvl",    {OP_VRG,OP_IMM,      },  {K1,(0b10110110)},


  "prefetch", {OP_IMR,             },  {K1,(0b11000110)},
  "clearc",   {OP_IMR,             },  {K1,(0b11010110)},
  "prefetch", {OP_RRG,             },  {K1,(0b11100110)},
  "clearc",   {OP_RRG,             },  {K1,(0b11110110)},

  //ALU-A Unit 0 (000)
  "nop",      {                    },  {K1,(0b00000000)},
  "sext",     {OP_REG,OP_REG,      },  {K1,(0b00010000)},
  "bool",     {OP_REG,OP_REG,      },  {K1,(0b00100000)},
  //"___",     {OP_REG,      ,      },  {K1,(0b00110000)},

  "slts",     {OP_REG,OP_REG,OP_REG},  {K1,(0b01000000)},
  "sltu",     {OP_REG,OP_REG,OP_REG},  {K1,(0b01010000)},
  "sltsi",    {OP_REG,OP_REG,OP_IMM},  {K1,(0b01100000)},
  "sltui",    {OP_REG,OP_REG,OP_IMM},  {K1,(0b01110000)},

  "smove",    {OP_REG,OP_IMM,      },  {K1,(0b10000000)},
  "smoveu",   {OP_REG,OP_IMM,      },  {K1,(0b10010000)},
  "movei",    {OP_REG,OP_IML,      },  {K1,(0b10100000)},
  "moveiu",   {OP_REG,OP_IML,      },  {K1,(0b10110000)},

  "moveins",  {OP_REG,             },  {K1,(0b11000000)},
  "movecyc",  {OP_REG,             },  {K1,(0b11010000)},

  "move",     {OP_REG,OP_RFR,      },  {K1,(0b11100000)},
  "move",     {OP_REG,OP_RBR,      },  {K1,(0b11100000)},
  "move",     {OP_REG,OP_RLR,      },  {K1,(0b11100000)},
  "move",     {OP_REG,OP_RIR,      },  {K1,(0b11100000)},

  "move",     {OP_RFR,OP_REG,      },  {K1,(0b11110000)},
  "move",     {OP_RBR,OP_REG,      },  {K1,(0b11110000)},
  "move",     {OP_RLR,OP_REG,      },  {K1,(0b11110000)},
  "move",     {OP_RIR,OP_REG,      },  {K1,(0b11110000)},

  //ALU-B Unit 1 (001)
  "move",     {OP_REG,OP_REG,      },  {K1,(0b10000110010)},

  "add",      {OP_REG,OP_REG,OP_REG},  {K1,(0b00000010)},
  "sub",      {OP_REG,OP_REG,OP_REG},  {K1,(0b00010010)},
  "xor",      {OP_REG,OP_REG,OP_REG},  {K1,(0b00100010)},
  "or",       {OP_REG,OP_REG,OP_REG},  {K1,(0b00110010)},
  "and",      {OP_REG,OP_REG,OP_REG},  {K1,(0b01000010)},
  "lsl",      {OP_REG,OP_REG,OP_REG},  {K1,(0b01010010)},
  "asr",      {OP_REG,OP_REG,OP_REG},  {K1,(0b01100010)},
  "lsr",      {OP_REG,OP_REG,OP_REG},  {K1,(0b01110010)},
  "muls",     {OP_RGP,OP_REG,OP_REG},  {K1,(0b10000010)},
  "mulu",     {OP_RGP,OP_REG,OP_REG},  {K1,(0b10010010)},
  "divs",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0b10100010)},
  "divu",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0b10110010)},
  "rems",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0b11000010)},
  "remu",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0b11010010)},

  "addi",     {OP_REG,OP_REG,OP_IMS},  {K1,(0b10000000010)},
  "subi",     {OP_REG,OP_REG,OP_IMS},  {K1,(0b10000010010)},
  "xori",     {OP_REG,OP_REG,OP_IMS},  {K1,(0b10000100010)},
  "ori",      {OP_REG,OP_REG,OP_IMS},  {K1,(0b10000110010)},
  "andi",     {OP_REG,OP_REG,OP_IMS},  {K1,(0b10001000010)},
  "lsli",     {OP_REG,OP_REG,OP_IMS},  {K1,(0b10001010010)},
  "asri",     {OP_REG,OP_REG,OP_IMS},  {K1,(0b10001100010)},
  "lsri",     {OP_REG,OP_REG,OP_IMS},  {K1,(0b10001110010)},
  "mulsi",    {OP_RGP,OP_REG,OP_IMS},  {K1,(0b10010000010)},
  "mului",    {OP_RGP,OP_REG,OP_IMS},  {K1,(0b10010010010)},
  "divsi",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0b10010100010)},
  "divui",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0b10010110010)},
  "remsi",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0b10011000010)},
  "remui",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0b10011010010)},

  //BRU Unit 5 (101)
  "bne",      {OP_IMB,             },  {K1,(0b00001010)},
  "beq",      {OP_IMB,             },  {K1,(0b00011010)},

  "bl",       {OP_IMB,             },  {K1,(0b00101010)},
  "ble",      {OP_IMB,             },  {K1,(0b00111010)},

  "bg",       {OP_IMB,             },  {K1,(0b01001010)},
  "bge",      {OP_IMB,             },  {K1,(0b01011010)},

  "bls",      {OP_IMB,             },  {K1,(0b01101010)},
  "bles",     {OP_IMB,             },  {K1,(0b01111010)},

  "bgs",      {OP_IMB,             },  {K1,(0b10001010)},
  "bges",     {OP_IMB,             },  {K1,(0b10011010)},

  "bra",      {OP_IMB,             },  {K1,(0b10100100)},

  "jmp",      {OP_IMM,             },  {K1,(0b10111010)},
  "jmpbr",    {OP_IMM,             },  {K1,(0b11001010)},
  "call",     {OP_IMM,             },  {K1,(0b11011010)},
  "callbr",   {OP_IMM,             },  {K1,(0b11101010)},


  //CMP Unit 4 (010)
  "cmp",      {OP_REG,OP_REG,      },  {K1,(0b00000100)},
  "cmp",      {OP_RFR,OP_IMM,      },  {K1,(0b00010100)},
  "cmpi",     {OP_REG,OP_IML,      },  {K1,(0b00100100)},
  "cmpiu",    {OP_REG,OP_IML,      },  {K1,(0b00110100)},

  "fcmp",     {OP_VRG,OP_VRG,      },  {K1,(0b01000100)},
  "dcmp",     {OP_VRG,OP_VRG,      },  {K1,(0b01010100)},
  "fcmpi",    {OP_VRG,OP_IMF,      },  {K1,(0b01100100)},
  "dcmpi",    {OP_VRG,OP_IMF,      },  {K1,(0b01110100)},

  "endp",     {                    },  {K1,(0b10000100)},
  "syscall",  {OP_IMM,             },  {K1,(0b10010100)},
  "int",      {OP_IMM,             },  {K1,(0b10100100)},
  //"__",     {OP_IMM,             },  {K1,(0b10110100)},

  "ret",      {                    },  {K1,(0b11000100)},
  "reti",     {                    },  {K1,(0b11010100)},
  "retc",     {                    },  {K1,(0b11100100)},
  //"__",     {                    },  {K1,(0b11110100)},

  //VFPU-A Unit 6 (110)
  "fadd",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00001100)},
  "fsub",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00011100)},
  "fmul",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00101100)},
  "fmadd",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00111100)},

  "vfadd",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01001100)},
  "vfsub",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01011100)},
  "vfmul",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01101100)},
  "vfmadd",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01111100)},

  "vfadds",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10001100)},
  "vfsubs",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10011100)},
  "vfmuls",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10101100)},
  "vfmadds",  {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10111100)},

  "fmsub",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11001100)},
  "vfmsub",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11011100)},
  "vfmsubs",  {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11101100)},
  "vfshuffle",{OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11111100)},

  //VFPU-B Unit 7 (111)
  "fmove",    {OP_VRG,OP_VRG,      },  {K1,(0b00001110)},
  "vmove",    {OP_VRG,OP_VRG,      },  {K1,(0b00011110)},
  "fmovei",   {OP_VRG,OP_IMF,      },  {K1,(0b00101110)},
  "vfmovei",  {OP_VRG,OP_IMF,      },  {K1,(0b00111110)},

  "fneg",     {OP_VRG,OP_VRG,      },  {K1,(0b01001110)},
  "fabs",     {OP_VRG,OP_VRG,      },  {K1,(0b01011110)},
  "vfneg",    {OP_VRG,OP_VRG,      },  {K1,(0b01101110)},
  "vfabs",    {OP_VRG,OP_VRG,      },  {K1,(0b01111110)},

  "vftoh",    {OP_VRG,OP_VRG,      },  {K1,(0b10001110)},
  "vhtof",    {OP_VRG,OP_VRG,      },  {K1,(0b10011110)},
  "vftoi",    {OP_VRG,OP_VRG,      },  {K1,(0b10101110)},
  "vitof",    {OP_VRG,OP_VRG,      },  {K1,(0b10111110)},

  "vftod",    {OP_VRG,OP_VRG,      },  {K1,(0b11001110)},
  "vdtof",    {OP_VRG,OP_VRG,      },  {K1,(0b11011110)},
  "fmin",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11101110)},
  "fmax",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11111110)},

  //EFU Unit 6 (110)
  "fdiv",     {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0b00001100)},
  "fsqrt",    {OP_RGQ,OP_VRG,      },  {K1,(0b00011100)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00101100)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00111100)},

  "fatan",    {OP_RGQ,OP_VRG,      },  {K1,(0b01001100)},
  "fatan2",   {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0b01011100)},
  "fexp",     {OP_RGQ,OP_VRG,      },  {K1,(0b01101100)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01111100)},

  "fsum",     {OP_RGP,OP_VRG,OP_VRG},  {K1,(0b10001100)},
  "fipr",     {OP_RGP,OP_VRG,OP_VRG},  {K1,(0b10011100)},
  "fsin",     {OP_RGP,OP_VRG,      },  {K1,(0b10101100)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10111100)},

  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11001100)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11011100)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11101100)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11111100)},

  //FPU-D Unit 7 (111)
  "dmovei",   {OP_VRG,OP_IMF,      },  {K1,(0b00001110)},
  "dadd",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00011110)},
  "dsub",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00101110)},
  "dmul",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00111110)},

  "dabs",     {OP_VRG,OP_VRG,      },  {K1,(0b01001110)},
  "dneg",     {OP_VRG,OP_VRG,      },  {K1,(0b01011110)},
  "dmin",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01101110)},
  "dmax",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01111110)},

  "ddiv",     {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0b10001110)},
  "dsqrt",    {OP_RGQ,OP_VRG,      },  {K1,(0b10011110)},
  "dsin",     {OP_RGP,OP_VRG,      },  {K1,(0b10101110)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10111110)},

  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11001110)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11011110)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11101110)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11111110)},


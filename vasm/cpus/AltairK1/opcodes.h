
  //DMA
  "lddma",    {OP_REG,OP_REG,OP_REG},  {K1,(0b00000000)},
  "stdma",    {OP_REG,OP_REG,OP_REG},  {K1,(0b00010000)},
  "lddmacl",  {OP_REG,OP_REG,      },  {K1,(0b00100000)},
  "stdmacl",  {OP_REG,OP_REG,      },  {K1,(0b00110000)},
  "dmai",     {OP_REG,OP_REG,OP_REG},  {K1,(0b01000000)},

  "lddmai",   {OP_REG,OP_REG,OP_IMM},  {K1,(0b10000000)},
  "stdmai",   {OP_REG,OP_REG,OP_IMM},  {K1,(0b10010000)},
  "lddmacli", {OP_REG,OP_IMM,      },  {K1,(0b10100000)},
  "stdmacli", {OP_REG,OP_IMM,      },  {K1,(0b10110000)},
  "dmaii",    {OP_REG,OP_REG,OP_IMM},  {K1,(0b11000000)},

  "wait",     {                    },  {K1,(0b11110000)},

  //LSU
  "ldm",      {OP_REG,OP_RRG,      },  {K1,(0b00000001)},
  "stm",      {OP_REG,OP_RRG,      },  {K1,(0b00010001)},
  "ldc",      {OP_REG,OP_RRG,      },  {K1,(0b00100001)},
  "stc",      {OP_REG,OP_RRG,      },  {K1,(0b00110001)},

  "ldmi",     {OP_REG,OP_IMR,      },  {K1,(0b10000001)},
  "stmi",     {OP_REG,OP_IMR,      },  {K1,(0b10010001)},
  "ldci",     {OP_REG,OP_IMR,      },  {K1,(0b10100001)},
  "stci",     {OP_REG,OP_IMR,      },  {K1,(0b10110001)},
  
  "ldmv",     {OP_VRG,OP_RRG,      },  {K1,(0b01000001)},
  "stmv",     {OP_VRG,OP_RRG,      },  {K1,(0b01010001)},
  "ldcv",     {OP_VRG,OP_RRG,      },  {K1,(0b01100001)},
  "stcv",     {OP_VRG,OP_RRG,      },  {K1,(0b01110001)},

  "ldmvi",    {OP_VRG,OP_IMR,      },  {K1,(0b11000001)},
  "stmvi",    {OP_VRG,OP_IMR,      },  {K1,(0b11010001)},
  "ldcvi",    {OP_VRG,OP_IMR,      },  {K1,(0b11100001)},
  "stcvi",    {OP_VRG,OP_IMR,      },  {K1,(0b11110001)},

  "ldml",     {OP_REG,OP_IMM,      },  {K1,(0b00000101)},
  "stml",     {OP_REG,OP_IMM,      },  {K1,(0b00010101)},
  "ldcl",     {OP_REG,OP_IMM,      },  {K1,(0b00100101)},
  "stcl",     {OP_REG,OP_IMM,      },  {K1,(0b00110101)},

  "ldmvl",    {OP_VRG,OP_IMM,      },  {K1,(0b01000101)},
  "stmvl",    {OP_VRG,OP_IMM,      },  {K1,(0b01010101)},
  "ldcvl",    {OP_VRG,OP_IMM,      },  {K1,(0b01100101)},
  "stcvl",    {OP_VRG,OP_IMM,      },  {K1,(0b01110101)}, 

  "prefetch", {OP_IMR,             },  {K1,(0b10000101)},
  "clearc",   {OP_IMR,             },  {K1,(0b10010101)},
  "prefetch", {OP_RRG,             },  {K1,(0b10100101)},
  "clearc",   {OP_RRG,             },  {K1,(0b10110101)},

  //ALU
  "nop",      {                    },  {K1,(0b00001010)},


  "moveins",  {OP_REG,             },  {K1,(0b01101010)},
  "movecyc",  {OP_REG,             },  {K1,(0b01111010)},

  "move",     {OP_REG,OP_RFR,      },  {K1,(0b10001010)},
  "move",     {OP_REG,OP_RBR,      },  {K1,(0b10011010)},
  "move",     {OP_REG,OP_RLR,      },  {K1,(0b10101010)},
  "move",     {OP_REG,OP_RIR,      },  {K1,(0b10111010)},

  "move",     {OP_RFR,OP_REG,      },  {K1,(0b11001010)},
  "move",     {OP_RBR,OP_REG,      },  {K1,(0b11011010)},
  "move",     {OP_RLR,OP_REG,      },  {K1,(0b11101010)},
  "move",     {OP_RIR,OP_REG,      },  {K1,(0b11111010)},

  "move",     {OP_REG,OP_REG,      },  {K1,(0b00110010)},
  "movei",    {OP_REG,OP_IMH,      },  {K1,(0b00001110)},

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

  "addi",     {OP_REG,OP_REG,OP_IMM},  {K1,(0b00000110)},
  "subi",     {OP_REG,OP_REG,OP_IMM},  {K1,(0b00010110)},
  "xori",     {OP_REG,OP_REG,OP_IMM},  {K1,(0b00100110)},
  "ori",      {OP_REG,OP_REG,OP_IMM},  {K1,(0b00110110)},
  "andi",     {OP_REG,OP_REG,OP_IMM},  {K1,(0b01000110)},
  "lsli",     {OP_REG,OP_REG,OP_IMM},  {K1,(0b01010110)},
  "asri",     {OP_REG,OP_REG,OP_IMM},  {K1,(0b01100110)},
  "lsri",     {OP_REG,OP_REG,OP_IMM},  {K1,(0b01110110)},
  "mulsi",    {OP_RGP,OP_REG,OP_IMM},  {K1,(0b10000110)},
  "mului",    {OP_RGP,OP_REG,OP_IMM},  {K1,(0b10010110)},
  "divsi",    {OP_RGQ,OP_REG,OP_IMM},  {K1,(0b10100110)},
  "divui",    {OP_RGQ,OP_REG,OP_IMM},  {K1,(0b10110110)},
  "remsi",    {OP_RGQ,OP_REG,OP_IMM},  {K1,(0b11000110)},
  "remui",    {OP_RGQ,OP_REG,OP_IMM},  {K1,(0b11010110)},

  //BRU
  "bne",      {OP_IMB,             },  {K1,(0b00000000)},
  "beq",      {OP_IMB,             },  {K1,(0b00010000)},

  "bl",       {OP_IMB,             },  {K1,(0b00100000)},
  "ble",      {OP_IMB,             },  {K1,(0b00110000)},

  "bg",       {OP_IMB,             },  {K1,(0b01000000)},
  "bge",      {OP_IMB,             },  {K1,(0b01010000)},

  "bls",      {OP_IMB,             },  {K1,(0b01100000)},
  "bles",     {OP_IMB,             },  {K1,(0b01110000)},

  "bgs",      {OP_IMB,             },  {K1,(0b10000000)},
  "bges",     {OP_IMB,             },  {K1,(0b10010000)},

  "bra",      {OP_IMB,             },  {K1,(0b10100000)},

  "jmp",      {OP_IMM,             },  {K1,(0b10110000)},
  "jmpbr",    {OP_IMM,             },  {K1,(0b11000000)},
  "call",     {OP_IMM,             },  {K1,(0b11010000)},
  "callbr",   {OP_IMM,             },  {K1,(0b11100000)},


  //CMP
  "cmp",      {OP_REG,OP_REG,      },  {K1,(0b00000100)},

  "fcmp",     {OP_VRG,OP_VRG,      },  {K1,(0b01000100)},
  "dcmp",     {OP_VRG,OP_VRG,      },  {K1,(0b01010100)},
  "fcmpi",    {OP_VRG,OP_IMF,      },  {K1,(0b01100100)},
  "dcmpi",    {OP_VRG,OP_IMF,      },  {K1,(0b01110100)},

  "endp",     {                    },  {K1,(0b10000100)},
  "ret",      {                    },  {K1,(0b10010100)},
  "int",      {OP_REG,OP_IMM,      },  {K1,(0b10100100)},
  "waiti",    {                    },  {K1,(0b10110100)},
  

  "cmpi",     {OP_REG,OP_IMH,      },  {K1,(0b00001100)},

  //VFPU
  "fadd",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00000011)},
  "fsub",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00010011)},
  "fmul",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00100011)},
  "fmadd",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00110011)},

  "vfadd",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01000011)},
  "vfsub",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01010011)},
  "vfmul",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01100011)},
  "vfmadd",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01110011)},

  "vfadds",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10000011)},
  "vfsubs",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10010011)},
  "vfmuls",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10100011)},
  "vfmadds",  {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10110011)},

  "fmsub",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11000011)},
  "vfmsub",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11010011)},
  "vfmsubs",  {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11100011)},
  "vfshuffle",{OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11110011)},

  //-----------------------
  "fmove",    {OP_VRG,OP_VRG,      },  {K1,(0b00000111)},
  "vmove",    {OP_VRG,OP_VRG,      },  {K1,(0b00010111)},
  "fmovei",   {OP_VRG,OP_IMF,      },  {K1,(0b00100111)},
  "vfmovei",  {OP_VRG,OP_IMF,      },  {K1,(0b00110111)},

  "fneg",     {OP_VRG,OP_VRG,      },  {K1,(0b01000111)},
  "fabs",     {OP_VRG,OP_VRG,      },  {K1,(0b01010111)},
  "vfneg",    {OP_VRG,OP_VRG,      },  {K1,(0b01100111)},
  "vfabs",    {OP_VRG,OP_VRG,      },  {K1,(0b01110111)},

  "vftoh",    {OP_VRG,OP_VRG,      },  {K1,(0b10000111)},
  "vhtof",    {OP_VRG,OP_VRG,      },  {K1,(0b10010111)},
  "vftoi",    {OP_VRG,OP_VRG,      },  {K1,(0b10100111)},
  "vitof",    {OP_VRG,OP_VRG,      },  {K1,(0b10110111)},

  "vftod",    {OP_VRG,OP_VRG,      },  {K1,(0b11000111)},
  "vdtof",    {OP_VRG,OP_VRG,      },  {K1,(0b11010111)},
  "fmin",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11100111)},
  "fmax",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11110111)},
  //-----------------------
  "fdiv",     {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0b00001011)},
  "fsqrt",    {OP_RGQ,OP_VRG,      },  {K1,(0b00011011)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00101011)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00111011)},

  "fatan",    {OP_RGQ,OP_VRG,      },  {K1,(0b01001011)},
  "fatan2",   {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0b01011011)},
  "fexp",     {OP_RGQ,OP_VRG,      },  {K1,(0b01101011)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01111011)},

  "fsum",     {OP_RGP,OP_VRG,OP_VRG},  {K1,(0b10001011)},
  "fipr",     {OP_RGP,OP_VRG,OP_VRG},  {K1,(0b10011011)},
  "fsin",     {OP_RGP,OP_VRG,      },  {K1,(0b10101011)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10111011)},

  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11001011)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11011011)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11101011)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11111011)},

  //-----------------------
  "dmovei",   {OP_VRG,OP_IMF,      },  {K1,(0b00001111)},
  "dadd",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00011111)},
  "dsub",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00101111)},
  "dmul",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b00111111)},

  "dabs",     {OP_VRG,OP_VRG,      },  {K1,(0b01001111)},
  "dneg",     {OP_VRG,OP_VRG,      },  {K1,(0b01011111)},
  "dmin",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01101111)},
  "dmax",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b01111111)},

  "ddiv",     {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0b10001111)},
  "dsqrt",    {OP_RGQ,OP_VRG,      },  {K1,(0b10011111)},
  "dsin",     {OP_RGP,OP_VRG,      },  {K1,(0b10101111)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b10111111)},

  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11001111)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11011111)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11101111)},
  //"____",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0b11111111)},


  //-----------------------
  "swt",      {                    },  {K1,(0b00001100)},

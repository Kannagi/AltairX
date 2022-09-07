  //ALU-A Unit 0 (000)
  "nop",      {                    },  {K1,(0x00<<1)},
  "sext",     {OP_REG,OP_REG,      },  {K1,(0x01<<1)},
  "bool",     {OP_REG,OP_REG,      },  {K1,(0x02<<1)},
  "smove",    {OP_REG,OP_IMM,      },  {K1,(0x03<<1)},

  "slts",     {OP_REG,OP_REG,OP_REG},  {K1,(0x04<<1)},
  "sltu",     {OP_REG,OP_REG,OP_REG},  {K1,(0x05<<1)},
  "sltsi",    {OP_REG,OP_REG,OP_IMM},  {K1,(0x06<<1)},
  "sltui",    {OP_REG,OP_REG,OP_IMM},  {K1,(0x07<<1)},

  "moven",    {OP_REG,OP_IML,      },  {K1,(0x08<<1)},
  "moveu",    {OP_REG,OP_IML,      },  {K1,(0x09<<1)},
  //"___",    {OP_REG,OP_IML,      },  {K1,(0x0A<<1)},
  //"___",    {OP_REG,OP_IML,      },  {K1,(0x0B<<1)},
  //-----------
  
  "moveins",  {OP_REG,             },  {K1,(0x0C<<1)},
  "movecyc",  {OP_REG,             },  {K1,(0x0D<<1)},

  "move",     {OP_REG,OP_RFR,      },  {K1,(0x0E<<1)},
  "move",     {OP_REG,OP_RBR,      },  {K1,(0x0E<<1)},
  "move",     {OP_REG,OP_RLR,      },  {K1,(0x0E<<1)},
  "move",     {OP_REG,OP_RIR,      },  {K1,(0x0E<<1)},

  "move",     {OP_RFR,OP_REG,      },  {K1,(0x0F<<1)},
  "move",     {OP_RBR,OP_REG,      },  {K1,(0x0F<<1)},
  "move",     {OP_RLR,OP_REG,      },  {K1,(0x0F<<1)},
  "move",     {OP_RIR,OP_REG,      },  {K1,(0x0F<<1)}, 

  //ALU-B Unit 1 (001)
  "move",     {OP_REG,OP_REG,      },  {K1,(0x213<<1)}, //ori

  "add",      {OP_REG,OP_REG,OP_REG},  {K1,(0x10<<1)},
  "sub",      {OP_REG,OP_REG,OP_REG},  {K1,(0x11<<1)},
  "xor",      {OP_REG,OP_REG,OP_REG},  {K1,(0x12<<1)},
  "or",       {OP_REG,OP_REG,OP_REG},  {K1,(0x13<<1)},
  "and",      {OP_REG,OP_REG,OP_REG},  {K1,(0x14<<1)},
  "lsl",      {OP_REG,OP_REG,OP_REG},  {K1,(0x15<<1)},
  "asr",      {OP_REG,OP_REG,OP_REG},  {K1,(0x16<<1)},
  "lsr",      {OP_REG,OP_REG,OP_REG},  {K1,(0x17<<1)},

  "divs",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0x18<<1)},
  "divu",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0x19<<1)},
  "rems",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0x1A<<1)},
  "remu",     {OP_RGQ,OP_REG,OP_REG},  {K1,(0x1B<<1)},
  "mul",      {OP_RGP,OP_REG,OP_REG},  {K1,(0x1C<<1)},

  "addi",     {OP_REG,OP_REG,OP_IMS},  {K1,(0x210<<1)},
  "subi",     {OP_REG,OP_REG,OP_IMS},  {K1,(0x211<<1)},
  "xori",     {OP_REG,OP_REG,OP_IMS},  {K1,(0x212<<1)},
  "ori",      {OP_REG,OP_REG,OP_IMS},  {K1,(0x213<<1)},
  "andi",     {OP_REG,OP_REG,OP_IMS},  {K1,(0x214<<1)},
  "lsli",     {OP_REG,OP_REG,OP_IMS},  {K1,(0x215<<1)},
  "asri",     {OP_REG,OP_REG,OP_IMS},  {K1,(0x216<<1)},
  "lsri",     {OP_REG,OP_REG,OP_IMS},  {K1,(0x217<<1)},

  "divsi",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0x218<<1)},
  "divui",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0x219<<1)},
  "remsi",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0x21A<<1)},
  "remui",    {OP_RGQ,OP_REG,OP_IMS},  {K1,(0x21B<<1)},
  "muli",     {OP_RGP,OP_REG,OP_IMS},  {K1,(0x21C<<1)},

  //LSU Unit 2 (010)
  "ldl",      {OP_REG,OP_IMM,      },  {K1,(0x20<<1)},
  "stl",      {OP_REG,OP_IMM,      },  {K1,(0x21<<1)},
  "ldvl",     {OP_VRG,OP_IMM,      },  {K1,(0x22<<1)},
  "stvl",     {OP_VRG,OP_IMM,      },  {K1,(0x23<<1)},

  "ldl1",     {OP_REG,OP_IMM,      },  {K1,(0x24<<1)},
  "stl1",     {OP_REG,OP_IMM,      },  {K1,(0x25<<1)},
  "ldvl1",    {OP_VRG,OP_IMM,      },  {K1,(0x26<<1)},
  "stvl1",    {OP_VRG,OP_IMM,      },  {K1,(0x27<<1)},
  
  "ld",       {OP_REG,OP_RRG,      },  {K1,(0x28<<1)},
  "st",       {OP_REG,OP_RRG,      },  {K1,(0x29<<1)},
  "ldv",      {OP_VRG,OP_RRG,      },  {K1,(0x2A<<1)},
  "stv",      {OP_VRG,OP_RRG,      },  {K1,(0x2B<<1)},

  "ldi",      {OP_REG,OP_IMR,      },  {K1,(0x2C<<1)},
  "sti",      {OP_REG,OP_IMR,      },  {K1,(0x2D<<1)},
  "ldvi",     {OP_VRG,OP_IMR,      },  {K1,(0x2E<<1)},
  "stvi",     {OP_VRG,OP_IMR,      },  {K1,(0x2F<<1)},

  //LSU-DMA (010)

  "iprefetch",{OP_ILB,             },  {K1,(0x30<<1)},
  "iflush",   {OP_ILB,             },  {K1,(0x31<<1)},
  "iprefetch",{OP_IMR,             },  {K1,(0x32<<1)},
  "iflush",   {OP_IMR,             },  {K1,(0x33<<1)},

  "prefetch", {OP_IMR,             },  {K1,(0x34<<1)},
  "flush",    {OP_IMR,             },  {K1,(0x35<<1)},
  "prefetch", {OP_RRG,             },  {K1,(0x36<<1)},
  "flush",    {OP_RRG,             },  {K1,(0x37<<1)},

  //---------
  "lddma",    {OP_REG,OP_REG,OP_REG},  {K1,(0x38<<1)},
  "stdma",    {OP_REG,OP_REG,OP_REG},  {K1,(0x39<<1)},
  "lddmai",   {OP_REG,OP_REG,OP_IMM},  {K1,(0x3A<<1)},
  "stdmai",   {OP_REG,OP_REG,OP_IMM},  {K1,(0x3B<<1)},

  //CMP Unit 4 (100)
  "cmp",      {OP_REG,OP_REG,      },  {K1,(0x40<<1)},
  "cmp",      {OP_RFR,OP_IMM,      },  {K1,(0x41<<1)},
  "cmpn",     {OP_REG,OP_IML,      },  {K1,(0x42<<1)},
  "cmpu",     {OP_REG,OP_IML,      },  {K1,(0x43<<1)},

  "fcmp",     {OP_VRG,OP_VRG,      },  {K1,(0x44<<1)},
  "dcmp",     {OP_VRG,OP_VRG,      },  {K1,(0x45<<1)},
  "fcmpi",    {OP_VRG,OP_IMF,      },  {K1,(0x46<<1)},
  "dcmpi",    {OP_VRG,OP_IMF,      },  {K1,(0x47<<1)},

  "endp",     {                    },  {K1,(0x48<<1)},
  "exe",      {OP_REG,             },  {K1,(0x49<<1)},
  "wait",     {                    },  {K1,(0x4A<<1)},
  //"_",      {OP_REG,             },  {K1,(0b10111000)},

  "ret",      {                    },  {K1,(0x4C<<1)},
  "reti",     {                    },  {K1,(0x4D<<1)},
  "syscall",  {                    },  {K1,(0x4E<<1)},
  "int",      {                    },  {K1,(0x4F<<1)},

  //BRU Unit 5 (101)
  "bne",      {OP_IMB,             },  {K1,(0x50<<1)},
  "beq",      {OP_IMB,             },  {K1,(0x51<<1)},

  "bl",       {OP_IMB,             },  {K1,(0x52<<1)},
  "ble",      {OP_IMB,             },  {K1,(0x53<<1)},

  "bg",       {OP_IMB,             },  {K1,(0x54<<1)},
  "bge",      {OP_IMB,             },  {K1,(0x55<<1)},

  "bls",      {OP_IMB,             },  {K1,(0x56<<1)},
  "bles",     {OP_IMB,             },  {K1,(0x57<<1)},

  "bgs",      {OP_IMB,             },  {K1,(0x58<<1)},
  "bges",     {OP_IMB,             },  {K1,(0x59<<1)},

  "bra",      {OP_IMB,             },  {K1,(0x5A<<1)},
  "loop",     {OP_IMB,             },  {K1,(0x5B<<1)},

  "jmp",      {OP_ILB,             },  {K1,(0x5C<<1)},
  "jmpbr",    {OP_ILB,             },  {K1,(0x5D<<1)},
  "call",     {OP_ILB,             },  {K1,(0x5E<<1)},
  "callbr",   {OP_ILB,             },  {K1,(0x5F<<1)},

  //VFPU-A Unit 6 (110)
  "fadd",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x60<<1)},
  "fsub",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x61<<1)},
  "fmul",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x62<<1)},
  "fmadd",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x63<<1)},

  "vfadd",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x64<<1)},
  "vfsub",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x65<<1)},
  "vfmul",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x66<<1)},
  "vfmadd",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x67<<1)},

  "vfadds",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x68<<1)},
  "vfsubs",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x69<<1)},
  "vfmuls",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x6A<<1)},
  "vfmadds",  {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x6B<<1)},

  "fmsub",    {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x6C<<1)},
  "vfmsub",   {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x6D<<1)},
  "vfmsubs",  {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x6E<<1)},
  "vfshuffle",{OP_VRG,OP_VRG,OP_VRG},  {K1,(0x6F<<1)},

  //VFPU-B Unit 7 (111)
  "fmove",    {OP_VRG,OP_VRG,      },  {K1,(0x70<<1)},
  "vmove",    {OP_VRG,OP_VRG,      },  {K1,(0x71<<1)},
  "fmovei",   {OP_VRG,OP_IMF,      },  {K1,(0x72<<1)},
  "vfmovei",  {OP_VRG,OP_IMF,      },  {K1,(0x73<<1)},

  "fneg",     {OP_VRG,OP_VRG,      },  {K1,(0x74<<1)},
  "fabs",     {OP_VRG,OP_VRG,      },  {K1,(0x75<<1)},
  "vfneg",    {OP_VRG,OP_VRG,      },  {K1,(0x76<<1)},
  "vfabs",    {OP_VRG,OP_VRG,      },  {K1,(0x77<<1)},

  "vftoh",    {OP_VRG,OP_VRG,      },  {K1,(0x78<<1)},
  "vhtof",    {OP_VRG,OP_VRG,      },  {K1,(0x79<<1)},
  "vftoi",    {OP_VRG,OP_VRG,      },  {K1,(0x7A<<1)},
  "vitof",    {OP_VRG,OP_VRG,      },  {K1,(0x7B<<1)},


  "fmin",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x7E<<1)},
  "fmax",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x7F<<1)},

  //EFU Unit 6 (110)
  "fdiv",     {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0x40<<1)},
  "fsqrt",    {OP_RGQ,OP_VRG,      },  {K1,(0x41<<1)},
  "ddiv",     {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0x42<<1)},
  "dsqrt",    {OP_RGQ,OP_VRG,      },  {K1,(0x43<<1)},

  "fatan",    {OP_RGQ,OP_VRG,      },  {K1,(0x44<<1)},
  "fatan2",   {OP_RGQ,OP_VRG,OP_VRG},  {K1,(0x45<<1)},
  "fexp",     {OP_RGQ,OP_VRG,      },  {K1,(0x46<<1)},
  //"___",    {OP_RGQ,OP_VRG,      },  {K1,(0x40<<1)},

  "fsum",     {OP_RGP,OP_VRG,OP_VRG},  {K1,(0x48<<1)},
  "fipr",     {OP_RGP,OP_VRG,OP_VRG},  {K1,(0x49<<1)},
  "fsin",     {OP_RGP,OP_VRG,      },  {K1,(0x4A<<1)},
  "dsin",     {OP_RGP,OP_VRG,      },  {K1,(0x4B<<1)},


  //FPU-D Unit 7 (111)
  "dmovei",   {OP_VRG,OP_IMF,      },  {K1,(0x50<<1)},
  "dadd",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x51<<1)},
  "dsub",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x52<<1)},
  "dmul",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x53<<1)},

  "dabs",     {OP_VRG,OP_VRG,      },  {K1,(0x54<<1)},
  "dneg",     {OP_VRG,OP_VRG,      },  {K1,(0x55<<1)},
  "dmin",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x56<<1)},
  "dmax",     {OP_VRG,OP_VRG,OP_VRG},  {K1,(0x57<<1)},
  
  "vftod",    {OP_VRG,OP_VRG,      },  {K1,(0x58<<1)},
  "vdtof",    {OP_VRG,OP_VRG,      },  {K1,(0x59<<1)},

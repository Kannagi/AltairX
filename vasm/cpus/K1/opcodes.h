
  //AGU
  "lddma",   {OP_IMR,OP_IMR,      },{K1,(0x0)+(0x00<<2)},
  "stdma",   {OP_IMR,OP_IMR,      },{K1,(0x0)+(0x02<<2)},


  "lddmar",  {OP_REG,OP_REG,OP_IMM},{K1,(0x0)+(0x01<<2)},
  "stdmar",  {OP_REG,OP_REG,OP_IMM},{K1,(0x0)+(0x03<<2)},

  "dmair",   {OP_REG,OP_REG,OP_IMM},{K1,(0x4)+(0x01<<4)},

  "lddmal",  {OP_REG,OP_IMM,      },{K1,(0x4)+(0x02<<4)},
  "stdmal",  {OP_REG,OP_IMM,      },{K1,(0x4)+(0x03<<4)},

  "prefetch",{OP_IMR,             },{K1,(0x4)+(0x04<<4)},
  "clearc",  {                    },{K1,(0x4)+(0x05<<4)},

  "wait",    {                    },{K1,(0x4)+(0x0F<<4)},


  //LSU
  "ldm",    {OP_REG,OP_IMR,      },{K1,(0x1)+(0x00<<2)},
  "stm",    {OP_REG,OP_IMR,      },{K1,(0x1)+(0x08<<2)},

  "ldc",    {OP_REG,OP_IMR,      },{K1,(0x1)+((0x02)<<2)},
  "stc",    {OP_REG,OP_IMR,      },{K1,(0x1)+((0x0A)<<2)},

  "ldmx",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x001<<2)},
  "stmx",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x011<<2)},

  "in",     {OP_IMM,OP_REG,      },{K1,(0x1)+(0x005<<2)},
  "out",    {OP_IMM,OP_REG,      },{K1,(0x1)+(0x015<<2)},

  "outi",   {OP_IMM,OP_IMM,      },{K1,(0x1)+(0x009<<2)},


  "ldmv",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x0D<<2)},
  "stmv",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x1D<<2)},

  "ldcv",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x2D<<2)},
  "stcv",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x3D<<2)},

  "ldmf",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x03<<2)},
  "stmf",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x13<<2)},

  "ldcf",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x0B<<2)},
  "stcf",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x1B<<2)},


  "ldmd",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x07<<2)},
  "stmd",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x17<<2)},

  "ldcd",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x0F<<2)},
  "stcd",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x1F<<2)},

  //ALU
  "nop",    {                    },{K1,(0x62)},
  "xchg",   {                    },{K1,(0x22)},

  "move",   {OP_REG,OP_REG,      },{K1,(0x2)+((0x01)<<2)},
  "movei",  {OP_REG,OP_IMM,      },{K1,(0x2)+((0x03)<<2)},

  "add",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)},
  "addi",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)},
  "addq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)},

  "sub",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x01)<<8)},
  "subi",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x01)<<4)},
  "subq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x01)<<4)},

  "muls",   {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x02)<<8)},
  "mulsi",  {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x02)<<4)},
  "mulsq",  {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x02)<<4)},

  "mulu",   {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x03)<<8)},
  "mului",  {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x03)<<4)},
  "muluq",  {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x03)<<4)},

  "divs",   {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x04)<<8)},
  "divsi",  {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x04)<<4)},
  "divsq",  {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x04)<<4)},

  "divu",   {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x05)<<8)},
  "divui",  {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x05)<<4)},
  "divuq",  {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x05)<<4)},

  "and",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x06)<<8)},
  "andi",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x06)<<4)},
  "andq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x06)<<4)},

  "or",     {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x07)<<8)},
  "ori",    {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x07)<<4)},
  "orq",    {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x07)<<4)},

  "xor",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x08)<<8)},
  "xori",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x08)<<4)},
  "xorq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x08)<<4)},

  "asl",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x09)<<8)},
  "asli",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x09)<<4)},
  "aslq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x09)<<4)},

  "lsl",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x0A)<<8)},
  "lsli",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x0A)<<4)},
  "lslq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x0A)<<4)},

  "asr",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x0B)<<8)},
  "asri",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x0B)<<4)},
  "asrq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x0B)<<4)},
  
  "lsr",    {OP_REG,OP_REG,OP_REG},{K1,(0x2)+((0x00)<<2)+((0x0C)<<8)},
  "lsri",   {OP_REG,OP_REG,OP_IMM},{K1,(0x2)+((0x01)<<2)+((0x0C)<<4)},
  "lsrq",   {OP_REG,OP_IMM,      },{K1,(0x2)+((0x02)<<2)+((0x0C)<<4)},

  //BRU
  "bne",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x00)<<8)},
  "beq",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x01)<<8)},

  "bl",     {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x02)<<8)},
  "ble",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x03)<<8)},

  "bg",     {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x04)<<8)},
  "bge",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x05)<<8)},

  "bls",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x06)<<8)},
  "bles",   {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x07)<<8)},

  "bgs",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x08)<<8)},
  "bges",   {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x09)<<8)},

  "bra",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x0A)<<8)},

  "cmp",    {OP_REG,OP_REG,      },{K1,(0x0)+((0+0x00)<<2)},
  "cmpi",   {OP_REG,OP_IMM,      },{K1,(0x0)+((0+0x01)<<2)},

  "fcmp",   {OP_VF ,OP_VF ,      },{K1,(0x0)+((0+0x04)<<2)},
  "fcmpi",  {OP_VF ,OP_IMM,      },{K1,(0x0)+((0+0x02)<<2)},

  "dcmp",   {OP_VD ,OP_VD ,      },{K1,(0x0)+((0+0x08)<<2)},
  "dcmpi",  {OP_VD ,OP_IMM,      },{K1,(0x0)+((0+0x03)<<2)},

  "jmp",    {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x00)<<8)},
  "call",   {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x01)<<8)},
  "jmpr",   {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x02)<<8)},
  "callr",  {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x03)<<8)},

  "ret",    {                    },{K1,(0x0)+((0+0x3C)<<2)},

  //VFPU
  "fadd",   {OP_VT ,OP_VT ,OP_VT },{K1,(0x3)+((0x00)<<10)},
  "fsub",   {OP_VT ,OP_VT ,OP_VT },{K1,(0x3)+((0x01)<<10)},
  "fmul",   {OP_VT ,OP_VT ,OP_VT },{K1,(0x3)+((0x02)<<10)},
  "fmuladd",{OP_VT ,OP_VT ,OP_VT },{K1,(0x3)+((0x03)<<10)},

  "dadd",   {OP_VD ,OP_VD ,OP_VD },{K1,(0x3)+((0x00)<<6)+(0x30)},
  "dsub",   {OP_VD ,OP_VD ,OP_VD },{K1,(0x3)+((0x01)<<6)+(0x30)},
  "dmul",   {OP_VD ,OP_VD ,OP_VD },{K1,(0x3)+((0x02)<<6)+(0x30)},
  "dmuladd",{OP_VD ,OP_VD ,OP_VD },{K1,(0x3)+((0x03)<<6)+(0x30)},

  "faddv",   {OP_VT ,OP_VT ,OP_VF },{K1,(0x3)+((0x00)<<10)+(0x20)},
  "fsubv",   {OP_VT ,OP_VT ,OP_VF },{K1,(0x3)+((0x01)<<10)+(0x20)},
  "fmulv",   {OP_VT ,OP_VT ,OP_VF },{K1,(0x3)+((0x02)<<10)+(0x20)},
  "fmuladdv",{OP_VT ,OP_VT ,OP_VF },{K1,(0x3)+((0x03)<<10)+(0x20)},

  "fmulva",    {OP_VT ,OP_VF ,      },{K1,(0x3)+((0x00)<<10)+(0x10)},
  "fmuladdva", {OP_VT ,OP_VF ,      },{K1,(0x3)+((0x01)<<10)+(0x10)},
  "fmuladdvao",{OP_VT ,OP_VT ,OP_VF },{K1,(0x3)+((0x02)<<10)+(0x10)},
  "fipr",      {OP_VF ,OP_VT ,OP_VT },{K1,(0x3)+((0x03)<<10)+(0x10)},


  "movefi", {OP_VF ,OP_IMM,      },{K1,(0x3)+((0x01)<<2)},
  "movedi", {OP_VD ,OP_IMM,      },{K1,(0x3)+((0x02)<<2)},
  "movevi", {OP_VT ,OP_IMM,      },{K1,(0x3)+((0x03)<<2)},

  "movev",  {OP_VT ,OP_VT ,      },{K1,(0x3)+((0x01)<<4)},
  "movefd", {OP_VD ,OP_VF ,      },{K1,(0x3)+((0x02)<<4)},
  "movedf", {OP_VF ,OP_VD ,      },{K1,(0x3)+((0x12)<<4)},


  "itof0",  {OP_VT ,OP_REG,      },{K1,(0x3)+((0x03)<<4)+((0x00)<<12)},
  "itof4",  {OP_VT ,OP_REG,      },{K1,(0x3)+((0x03)<<4)+((0x01)<<12)},
  "itof8",  {OP_VT ,OP_REG,      },{K1,(0x3)+((0x03)<<4)+((0x02)<<12)},
  "itof15", {OP_VT ,OP_REG,      },{K1,(0x3)+((0x03)<<4)+((0x03)<<12)},

  "ftoi0",  {OP_REG,OP_VT ,      },{K1,(0x3)+((0x03)<<4)+((0x04)<<12)},
  "ftoi4",  {OP_REG,OP_VT ,      },{K1,(0x3)+((0x03)<<4)+((0x05)<<12)},
  "ftoi8",  {OP_REG,OP_VT ,      },{K1,(0x3)+((0x03)<<4)+((0x06)<<12)},
  "ftoi15", {OP_REG,OP_VT ,      },{K1,(0x3)+((0x03)<<4)+((0x07)<<12)},

  "itof",   {OP_VF ,OP_REG,      },{K1,(0x3)+((0x03)<<4)+((0x01)<<8)},
  "ftoi",   {OP_REG,OP_VF ,      },{K1,(0x3)+((0x03)<<4)+((0x05)<<8)},
  "itod",   {OP_VD ,OP_REG,      },{K1,(0x3)+((0x03)<<4)+((0x02)<<8)},
  "dtoi",   {OP_REG,OP_VD ,      },{K1,(0x3)+((0x03)<<4)+((0x06)<<8)},

  //FDIV
  "fdiv",   {OP_VT ,OP_VT ,OP_VT },{K1,(0x33)+((0x0C)<<6)},
  "fsqrt",  {OP_VT ,OP_VT ,OP_VT },{K1,(0x33)+((0x0D)<<6)},

  "ddiv",   {OP_VT ,OP_VT ,OP_VT },{K1,(0x33)+((0x0E)<<6)},
  "dsqrt",  {OP_VT ,OP_VT ,OP_VT },{K1,(0x33)+((0x0F)<<6)},

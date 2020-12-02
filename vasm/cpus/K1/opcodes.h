
  //AGU
  "lddma", {OP_IMR,OP_IMR,      },{K1,(0x0)+(0x00<<2)},
  "stdma", {OP_IMR,OP_IMR,      },{K1,(0x0)+(0x01<<2)},

  //LSU
  "ldm",    {OP_REG,OP_IMR,      },{K1,(0x1)+(0x00<<2)},
  "stm",    {OP_REG,OP_IMR,      },{K1,(0x1)+(0x08<<2)},

  "ldc",    {OP_REG,OP_IMR,      },{K1,(0x1)+((0x02)<<2)},
  "stc",    {OP_REG,OP_IMR,      },{K1,(0x1)+((0x0A)<<2)},

  "ldmx",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x001<<2)},
  "stmx",   {OP_REG,OP_IMR,      },{K1,(0x1)+(0x011<<2)},

  "in",     {OP_IMM,OP_REG,      },{K1,(0x1)+(0x005<<2)},
  "out",    {OP_IMM,OP_REG,      },{K1,(0x1)+(0x015<<2)},

  "ini",    {OP_IMM,OP_IMM,      },{K1,(0x1)+(0x009<<2)},
  "outi",   {OP_IMM,OP_IMM,      },{K1,(0x1)+(0x019<<2)},


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
  "fbne",   {OP_IMM,             },{K1,(0x0)+((0+0x1C)<<2)+((0x00)<<8)},
  "fbeq",   {OP_IMM,             },{K1,(0x0)+((0+0x1C)<<2)+((0x01)<<8)},

  "fbl",    {OP_IMM,             },{K1,(0x0)+((0+0x1C)<<2)+((0x02)<<8)},
  "fble",   {OP_IMM,             },{K1,(0x0)+((0+0x1C)<<2)+((0x03)<<8)},

  "fbg",    {OP_IMM,             },{K1,(0x0)+((0+0x1C)<<2)+((0x04)<<8)},
  "fbge",   {OP_IMM,             },{K1,(0x0)+((0+0x1C)<<2)+((0x05)<<8)},

  "bne",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x00)<<8)},
  "beq",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x01)<<8)},

  "blu",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x02)<<8)},
  "bleu",   {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x03)<<8)},

  "bgu",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x04)<<8)},
  "bgeu",   {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x05)<<8)},

  "bls",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x06)<<8)},
  "bles",   {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x07)<<8)},

  "bgs",    {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x08)<<8)},
  "bges",   {OP_IMM,             },{K1,(0x0)+((0+0x0C)<<2)+((0x09)<<8)},

  "cmp",    {OP_REG,OP_REG,      },{K1,(0x0)+((0+0x00)<<2)},
  "cmpi",   {OP_REG,OP_IMM,      },{K1,(0x0)+((0+0x01)<<2)},

  "fcmp",   {OP_VF ,OP_VF ,      },{K1,(0x0)+((0+0x04)<<2)},
  "fcmpi",  {OP_VF ,OP_IMM,      },{K1,(0x0)+((0+0x02)<<2)},

  "dcmp",   {OP_VF ,OP_VF ,      },{K1,(0x0)+((0+0x08)<<2)},
  "dcmpi",  {OP_VF ,OP_IMM,      },{K1,(0x0)+((0+0x03)<<2)},

  "jmp",    {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x00)<<8)},
  "call",   {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x01)<<8)},
  "jmpr",   {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x02)<<8)},
  "callr",  {OP_IMM,             },{K1,(0x0)+((0+0x2C)<<2)+((0x03)<<8)},

  "ret",    {                    },{K1,(0x0)+((0+0x3C)<<2)},
/*
  //VFPU 1 
  "fadd",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x00)<<2)},
  "fsub",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x01)<<2)},
  "fmul",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x02)<<2)},
  "fmuladd",{OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x03)<<2)},

  "dadd",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x04+0x00)<<2)},
  "dsub",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x04+0x01)<<2)},
  "dmul",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x04+0x02)<<2)},
  "dmuladd",{OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x04+0x03)<<2)},

  "faddv",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x00)<<2)},
  "fsubv",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x01)<<2)},
  "fmulv",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x02)<<2)},
  "fmuladdv",{OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x03)<<2)},

  "fipr",    {OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x04)<<2)},
  "fmula",   {OP_VF ,OP_VF ,      },{K1,(0x0)+((0x00+0x05)<<2)},
  "fmuladda",{OP_VF ,OP_VF ,      },{K1,(0x0)+((0x00+0x06)<<2)},
  "fmuladda",{OP_VF ,OP_VF ,OP_VF },{K1,(0x0)+((0x00+0x07)<<2)},

  "movev",   {OP_VF ,OP_VF ,      },{K1,(0x0)+((0x00+0x01)<<2)},
  "movev",   {OP_REG,OP_VF ,      },{K1,(0x0)+((0x02+0x01)<<2)},
  "movev",   {OP_VF ,OP_REG,      },{K1,(0x0)+((0x04+0x01)<<2)},


  //VFPU 2
  "itof0",  {OP_VF ,             },{K1,(0x01)+((0x00+0x00)<<2)},
  "itof4",  {OP_VF ,             },{K1,(0x01)+((0x00+0x01)<<2)},
  "itof8",  {OP_VF ,             },{K1,(0x01)+((0x00+0x02)<<2)},
  "itof15", {OP_VF ,             },{K1,(0x01)+((0x00+0x03)<<2)},

  "ftoi0",  {OP_VF ,             },{K1,(0x01)+((0x04+0x00)<<2)},
  "ftoi4",  {OP_VF ,             },{K1,(0x01)+((0x04+0x01)<<2)},
  "ftoi8",  {OP_VF ,             },{K1,(0x01)+((0x04+0x02)<<2)},
  "ftoi15", {OP_VF ,             },{K1,(0x01)+((0x04+0x03)<<2)},

  "movevi", {OP_VF ,OP_IMM,      },{K1,(0x01<2)+((0x00+0x01)<<2)},

  //FDIV
  "fdiv",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x03<2)+((0x00+0x00)<<2)},
  "ddiv",   {OP_VF ,OP_VF ,OP_VF },{K1,(0x03<2)+((0x00+0x00)<<2)},


  "fsqrt",  {OP_VF ,OP_VF ,OP_VF },{K1,(0x03<2)+((0x00+0x01)<<2)},
  "dsqrt",  {OP_VF ,OP_VF ,OP_VF },{K1,(0x03<2)+((0x00+0x01)<<2)},
*/
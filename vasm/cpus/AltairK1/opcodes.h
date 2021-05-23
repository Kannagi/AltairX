
  //DMA
  "lddmar",  {OP_REG,OP_REG,OP_IMM},{K1,(0b00000000)},
  "stdmar",  {OP_REG,OP_REG,OP_IMM},{K1,(0b00010000)},

  "dmair",   {OP_REG,OP_REG,OP_IMM},{K1,(0b00100000)},

  "lddmacl", {OP_REG,OP_IMM,      },{K1,(0b00110000)},
  "stdmacl", {OP_REG,OP_IMM,      },{K1,(0b01000000)},

  "wait",    {                    },{K1,(0b01010000)},

  "prefetch",{OP_IMR,             },{K1,(0b10000000)},
  "clearc",  {OP_IMR,             },{K1,(0b10010000)},
  "prefetch",{OP_RRG,             },{K1,(0b10100000)},
  "clearc",  {OP_RRG,             },{K1,(0b10110000)},

  //LSU
  "ldm",    {OP_REG,OP_RRG,      },{K1,(0b00000001)},
  "stm",    {OP_REG,OP_RRG,      },{K1,(0b00010001)},
  "ldc",    {OP_REG,OP_RRG,      },{K1,(0b00100001)},
  "stc",    {OP_REG,OP_RRG,      },{K1,(0b00110001)},

  "ldmi",   {OP_REG,OP_IMR,      },{K1,(0b10000001)},
  "stmi",   {OP_REG,OP_IMR,      },{K1,(0b10010001)},
  "ldci",   {OP_REG,OP_IMR,      },{K1,(0b10100001)},
  "stci",   {OP_REG,OP_IMR,      },{K1,(0b10110001)},
  
  "ldmv",   {OP_VP ,OP_RRG,      },{K1,(0b01000001)},
  "stmv",   {OP_VP ,OP_RRG,      },{K1,(0b01010001)},
  "ldcv",   {OP_VP ,OP_RRG,      },{K1,(0b01100001)},
  "stcv",   {OP_VP ,OP_RRG,      },{K1,(0b01110001)},

  "ldmvi",  {OP_VP ,OP_IMR,      },{K1,(0b11000001)},
  "stmvi",  {OP_VP ,OP_IMR,      },{K1,(0b11010001)},
  "ldcvi",  {OP_VP ,OP_IMR,      },{K1,(0b11100001)},
  "stcvi",  {OP_VP ,OP_IMR,      },{K1,(0b11110001)},

  "ldml",   {OP_REG,OP_IMM,      },{K1,(0b00000101)},
  "stml",   {OP_REG,OP_IMM,      },{K1,(0b00010101)},
  "ldcl",   {OP_REG,OP_IMM,      },{K1,(0b00100101)},
  "stcl",   {OP_REG,OP_IMM,      },{K1,(0b00110101)},

  "ldmvl",  {OP_VP ,OP_IMM,      },{K1,(0b01000101)},
  "stmvl",  {OP_VP ,OP_IMM,      },{K1,(0b01010101)},
  "ldcvl",  {OP_VP ,OP_IMM,      },{K1,(0b01100101)},
  "stcvl",  {OP_VP ,OP_IMM,      },{K1,(0b01110101)},

  //ALU
  "nop",    {                    },{K1,(0b00001010)},

  "move",   {OP_RLR,OP_REG,      },{K1,(0b00111010)},
  "move",   {OP_REG,OP_RLR,      },{K1,(0b01011010)},
  "move",   {OP_RBR,OP_REG,      },{K1,(0b01001010)},

  "move",   {OP_REG,OP_REG,      },{K1,(0b00110110)},
  "movei",  {OP_REG,OP_IMH,      },{K1,(0b00001110)},

  "add",    {OP_REG,OP_REG,OP_REG},{K1,(0b00000010)},
  "sub",    {OP_REG,OP_REG,OP_REG},{K1,(0b00010010)},
  "xor",    {OP_REG,OP_REG,OP_REG},{K1,(0b00100010)},
  "or",     {OP_REG,OP_REG,OP_REG},{K1,(0b00110010)},
  "and",    {OP_REG,OP_REG,OP_REG},{K1,(0b01000010)},
  "lsl",    {OP_REG,OP_REG,OP_REG},{K1,(0b01010010)},
  "asr",    {OP_REG,OP_REG,OP_REG},{K1,(0b01100010)},
  "lsr",    {OP_REG,OP_REG,OP_REG},{K1,(0b01110010)},
  "muls",   {OP_RGP,OP_REG,OP_REG},{K1,(0b10000010)},
  "mulu",   {OP_RGP,OP_REG,OP_REG},{K1,(0b10010010)},
  "divs",   {OP_RGQ,OP_REG,OP_REG},{K1,(0b10100010)},
  "divu",   {OP_RGQ,OP_REG,OP_REG},{K1,(0b10110010)},
  "rems",   {OP_RGQ,OP_REG,OP_REG},{K1,(0b11000010)},
  "remu",   {OP_RGQ,OP_REG,OP_REG},{K1,(0b11010010)},

  "addi",   {OP_REG,OP_REG,OP_IMM},{K1,(0b00000110)},
  "subi",   {OP_REG,OP_REG,OP_IMM},{K1,(0b00010110)},
  "xori",   {OP_REG,OP_REG,OP_IMM},{K1,(0b00100110)},
  "ori",    {OP_REG,OP_REG,OP_IMM},{K1,(0b00110110)},
  "andi",   {OP_REG,OP_REG,OP_IMM},{K1,(0b01000110)},
  "lsli",   {OP_REG,OP_REG,OP_IMM},{K1,(0b01010110)},
  "asri",   {OP_REG,OP_REG,OP_IMM},{K1,(0b01100110)},
  "lsri",   {OP_REG,OP_REG,OP_IMM},{K1,(0b01110110)},
  "mulsi",  {OP_RGP,OP_REG,OP_IMM},{K1,(0b10000110)},
  "mului",  {OP_RGP,OP_REG,OP_IMM},{K1,(0b10010110)},
  "divsi",  {OP_RGQ,OP_REG,OP_IMM},{K1,(0b10100110)},
  "divui",  {OP_RGQ,OP_REG,OP_IMM},{K1,(0b10110110)},
  "remsi",  {OP_RGQ,OP_REG,OP_IMM},{K1,(0b11000110)},
  "remui",  {OP_RGQ,OP_REG,OP_IMM},{K1,(0b11010110)},

  //BRU
  "bne",    {OP_IMB,             },{K1,(0b00000000)},
  "beq",    {OP_IMB,             },{K1,(0b00010000)},

  "bl",     {OP_IMB,             },{K1,(0b00100000)},
  "ble",    {OP_IMB,             },{K1,(0b00110000)},

  "bg",     {OP_IMB,             },{K1,(0b01000000)},
  "bge",    {OP_IMB,             },{K1,(0b01010000)},

  "bls",    {OP_IMB,             },{K1,(0b01100000)},
  "bles",   {OP_IMB,             },{K1,(0b01110000)},

  "bgs",    {OP_IMB,             },{K1,(0b10000000)},
  "bges",   {OP_IMB,             },{K1,(0b10010000)},

  "bra",    {OP_IMB,             },{K1,(0b10100000)},

  "jmp",    {OP_IMM,             },{K1,(0b10110000)},
  "call",   {OP_IMM,             },{K1,(0b11000000)},
  "jmpbr",  {OP_IMM,             },{K1,(0b11010000)},
  "callbr", {OP_IMM,             },{K1,(0b11100000)},


  //CMP
  "cmp",    {OP_REG,OP_REG,      },{K1,(0b00000100)},
  "fcmp",   {OP_VP ,OP_VP ,      },{K1,(0b00010100)},
  "dcmp",   {OP_VP ,OP_VP ,      },{K1,(0b00100100)},
  "fcmpi",  {OP_VP ,OP_IMF,      },{K1,(0b00110100)},
  "dcmpi",  {OP_VP ,OP_IMF,      },{K1,(0b01000100)},

  "endp",   {                    },{K1,(0b10000100)},
  "int",    {OP_REG,OP_IMM,      },{K1,(0b10010100)},
  "waiti",  {                    },{K1,(0b10100100)},
  "ret",    {                    },{K1,(0b10110100)},

  "cmpi",   {OP_REG,OP_IMH,      },{K1,(0b00000100)},

  //VFPU
  "vfadd",  {OP_VP ,OP_VP ,OP_VP },{K1,(0b00000011)},
  "vfsub",  {OP_VP ,OP_VP ,OP_VP },{K1,(0b00000011)},
  "vfmul",  {OP_VP ,OP_VP ,OP_VP },{K1,(0b00000011)},
  "vfmadd", {OP_VP ,OP_VP ,OP_VP },{K1,(0b00000011)},

  "fipr",   {OP_VP ,OP_VP ,OP_VP },{K1,(0b00000011)},

  "fdiv",   {OP_VP ,OP_VP ,      },{K1,(0b00000011)},
  "fsqrt",  {OP_VP ,             },{K1,(0b00000011)},
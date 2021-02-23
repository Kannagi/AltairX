
  //AGU
  "lddma",   {OP_IMR,OP_IMR,      },{K1,(0b0000)},
  "stdma",   {OP_IMR,OP_IMR,      },{K1,(0b1000)},


  "lddmar",  {OP_REG,OP_REG,OP_IMM},{K1,(0b00000100)},
  "stdmar",  {OP_REG,OP_REG,OP_IMM},{K1,(0b00010100)},

  "dmair",   {OP_REG,OP_REG,OP_IMM},{K1,(0b00100100)},

  "lddmal",  {OP_REG,OP_IMM,      },{K1,(0b00110100)},
  "stdmal",  {OP_REG,OP_IMM,      },{K1,(0b01000100)},

  "prefetch",{OP_IMR,             },{K1,(0b10000100)},
  "clearca", {OP_IMR,             },{K1,(0b10010100)},


  "clearc",  {                    },{K1,(0b10100100)},
  "wait",    {                    },{K1,(0b10110100)},


  //LSU
  "ldm",    {OP_REG,OP_IMR,      },{K1,(0b000001)},
  "stm",    {OP_REG,OP_IMR,      },{K1,(0b010001)},
  "ldmi",   {OP_REG,OP_IMM,      },{K1,(0b100001)},
  "stmi",   {OP_REG,OP_IMM,      },{K1,(0b110001)},

  "ldc",    {OP_REG,OP_IMR,      },{K1,(0b000101)},
  "stc",    {OP_REG,OP_IMR,      },{K1,(0b010101)},
  "ldci",   {OP_REG,OP_IMM,      },{K1,(0b100101)},
  "stci",   {OP_REG,OP_IMM,      },{K1,(0b110101)},


  "in",     {OP_IMM,OP_REG,      },{K1,(0b00001001)},
  "out",    {OP_IMM,OP_REG,      },{K1,(0b00011001)},

  "outi",   {OP_IMM,OP_IMM,      },{K1,(0b00101001)},
  "outib",  {OP_IMM,OP_IMM,      },{K1,(0b00101001)},
  "outiw",  {OP_IMM,OP_IMM,      },{K1,(0b00111001)},


  "ldmv",   {OP_VP ,OP_IMR,      },{K1,(0b00001101)},
  "stmv",   {OP_VP ,OP_IMR,      },{K1,(0b00011101)},
  "ldmvi",  {OP_VP ,OP_IMM,      },{K1,(0b01001101)},
  "stmvi",  {OP_VP ,OP_IMM,      },{K1,(0b01011101)},

  "ldcv",   {OP_VP ,OP_IMR,      },{K1,(0b00101101)},
  "stcv",   {OP_VP ,OP_IMR,      },{K1,(0b00111101)},
  "ldcvi",  {OP_VP ,OP_IMM,      },{K1,(0b01101101)},
  "stcvi",  {OP_VP ,OP_IMM,      },{K1,(0b01111101)},

  //ALU
  "nop",    {                    },{K1,(0b00010010)},

  "move",   {OP_RLR,OP_REG,      },{K1,(0b00100010)},
  "move",   {OP_REG,OP_RLR,      },{K1,(0b00110010)},
  "move",   {OP_RBR,OP_REG,      },{K1,(0b01000010)},

  "move",   {OP_REG,OP_REG,      },{K1,(0b00000110)},
  "movei",  {OP_REG,OP_IMM,      },{K1,(0b1110)},

  "add",    {OP_REG,OP_REG,OP_REG},{K1,(0b000000000010)},
  "addi",   {OP_REG,OP_REG,OP_IMM},{K1,(0b00000110)},
  "addq",   {OP_REG,OP_IMM,      },{K1,(0b00001010)},

  "sub",    {OP_REG,OP_REG,OP_REG},{K1,(0b000100000010)},
  "subi",   {OP_REG,OP_REG,OP_IMM},{K1,(0b00010110)},
  "subq",   {OP_REG,OP_IMM,      },{K1,(0b00011010)},

  "muls",   {OP_REG,OP_REG,OP_REG},{K1,(0b001000000010)},
  "mulsi",  {OP_REG,OP_REG,OP_IMM},{K1,(0b00100110)},
  "mulsq",  {OP_REG,OP_IMM,      },{K1,(0b00101010)},

  "mulu",   {OP_REG,OP_REG,OP_REG},{K1,(0b001100000010)},
  "mului",  {OP_REG,OP_REG,OP_IMM},{K1,(0b00110110)},
  "muluq",  {OP_REG,OP_IMM,      },{K1,(0b00111010)},

  "divs",   {OP_REG,OP_REG,OP_REG},{K1,(0b010000000010)},
  "divsi",  {OP_REG,OP_REG,OP_IMM},{K1,(0b01000110)},
  "divsq",  {OP_REG,OP_IMM,      },{K1,(0b01001010)},

  "divu",   {OP_REG,OP_REG,OP_REG},{K1,(0b010100000010)},
  "divui",  {OP_REG,OP_REG,OP_IMM},{K1,(0b01010110)},
  "divuq",  {OP_REG,OP_IMM,      },{K1,(0b01011010)},

  "and",    {OP_REG,OP_REG,OP_REG},{K1,(0b011000000010)},
  "andi",   {OP_REG,OP_REG,OP_IMM},{K1,(0b01100110)},
  "andq",   {OP_REG,OP_IMM,      },{K1,(0b01101010)},

  "or",     {OP_REG,OP_REG,OP_REG},{K1,(0b011100000010)},
  "ori",    {OP_REG,OP_REG,OP_IMM},{K1,(0b01110110)},
  "orq",    {OP_REG,OP_IMM,      },{K1,(0b01111010)},

  "xor",    {OP_REG,OP_REG,OP_REG},{K1,(0b100000000010)},
  "xori",   {OP_REG,OP_REG,OP_IMM},{K1,(0b10000110)},
  "xorq",   {OP_REG,OP_IMM,      },{K1,(0b10001010)},

  "asl",    {OP_REG,OP_REG,OP_REG},{K1,(0b100100000010)},
  "asli",   {OP_REG,OP_REG,OP_IMM},{K1,(0b10010110)},
  "aslq",   {OP_REG,OP_IMM,      },{K1,(0b10011010)},

  "lsl",    {OP_REG,OP_REG,OP_REG},{K1,(0b101000000010)},
  "lsli",   {OP_REG,OP_REG,OP_IMM},{K1,(0b10100110)},
  "lslq",   {OP_REG,OP_IMM,      },{K1,(0b10101010)},

  "asr",    {OP_REG,OP_REG,OP_REG},{K1,(0b101100000010)},
  "asri",   {OP_REG,OP_REG,OP_IMM},{K1,(0b10110110)},
  "asrq",   {OP_REG,OP_IMM,      },{K1,(0b10111010)},
  
  "lsr",    {OP_REG,OP_REG,OP_REG},{K1,(0b110000000010)},
  "lsri",   {OP_REG,OP_REG,OP_IMM},{K1,(0b11000110)},
  "lsrq",   {OP_REG,OP_IMM,      },{K1,(0b11001010)},

  //BRU
  "bne",    {OP_IMM,             },{K1,(0b000000110000)},
  "beq",    {OP_IMM,             },{K1,(0b000100110000)},

  "bl",     {OP_IMM,             },{K1,(0b001000110000)},
  "ble",    {OP_IMM,             },{K1,(0b001100110000)},

  "bg",     {OP_IMM,             },{K1,(0b010000110000)},
  "bge",    {OP_IMM,             },{K1,(0b010100110000)},

  "bls",    {OP_IMM,             },{K1,(0b011000110000)},
  "bles",   {OP_IMM,             },{K1,(0b011100110000)},

  "bgs",    {OP_IMM,             },{K1,(0b100000110000)},
  "bges",   {OP_IMM,             },{K1,(0b100100110000)},

  "bra",    {OP_IMM,             },{K1,(0b101000110000)},

  "jmp",    {OP_IMM,             },{K1,(0b000010110000)},
  "call",   {OP_IMM,             },{K1,(0b001010110000)},
  "jmpr",   {OP_IMM,             },{K1,(0b000110110000)},
  "callr",  {OP_IMM,             },{K1,(0b001110110000)},

  "endp",   {                    },{K1,(0b010010110000)},
  "swt",    {OP_IMM,             },{K1,(0b011010110000)},
  "ret",    {                    },{K1,(0b000011110000)},

  //CMP
  "cmp",    {OP_REG,OP_REG,      },{K1,(0b000000)},
  "pcmp",   {OP_VP ,OP_VP ,      },{K1,(0b000000)}, //0b100000 or 0b010000

  "cmpi",   {OP_REG,OP_IMM,      },{K1,(0b0100)},
  "pcmpi",  {OP_VP ,OP_IMM,      },{K1,(0b1000)}, //+0b100

  //VPU
  "padd",   {OP_VP ,OP_VP ,OP_VP },{K1,(0b0000000011)},
  "psub",   {OP_VP ,OP_VP ,OP_VP },{K1,(0b0010000011)},
  "pmul",   {OP_VP ,OP_VP ,OP_VP },{K1,(0b0100000011)},
  "pmadd",  {OP_VP ,OP_VP ,OP_VP },{K1,(0b0110000011)},

  "padds",  {OP_VP ,OP_VP ,OP_VP },{K1,(0b0000100011)},
  "psubs",  {OP_VP ,OP_VP ,OP_VP },{K1,(0b0010100011)},
  "pmuls",  {OP_VP ,OP_VP ,OP_VP },{K1,(0b0100100011)},
  "pmadds", {OP_VP ,OP_VP ,OP_VP },{K1,(0b0110100011)},

  "pipr",   {OP_VP ,OP_VP ,OP_VP },{K1,(0b0110110011)},

  "pmove",  {OP_VP ,OP_VP ,      },{K1,(0b0000000111)},
  "vpmove", {OP_VP ,OP_VP ,      },{K1,(0b0000010111)},

  "pmovei", {OP_VP ,OP_IMM,      },{K1,(0b01000111)},
  "vpmovei",{OP_VP ,OP_IMM,      },{K1,(0b01100111)},

  "pmovero",{OP_VP ,OP_REG,      },{K1,(0b0000100111)},
  "pmoveri",{OP_VP ,OP_REG,      },{K1,(0b0000110111)},

  "pdiv",   {OP_VP ,OP_VP ,OP_VP },{K1,(0b0000001011)},
  "psqrt",  {OP_VP ,OP_VP ,      },{K1,(0b0000101011)},
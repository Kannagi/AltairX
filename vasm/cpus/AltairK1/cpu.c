 /* cpu.c example cpu-description file */
/* (c) in 2002 by Volker Barthelmann */
/* (c) in 2020 by Samy Meguenoun */
#include <stdlib.h>
#include "vasm.h" 

char *cpu_copyright="vasm Altair cpu backend 1.0 (c) 2020 Samy Mahjid Meguenoun";
 

char *cpuname="AltairK1";
int bitsperbyte=8;
int bytespertaddr=4;
int k1ext = 0x0;


mnemonic mnemonics[]={
#include "opcodes.h"

};

int mnemonic_cnt=sizeof(mnemonics)/sizeof(mnemonics[0]);
static char current_ext;   

char *parse_instruction(char *s,int *inst_len,char **ext,int *ext_len,
                        int *ext_cnt)
/* parse instruction and save extension locations */
{
    char *inst = s;
    int cnt = *ext_cnt;

    if (*s == '.')  /* allow dot as first char */
        s++;
    while (*s && *s!='.' && !isspace((unsigned char)*s))
        s++;
    *inst_len = s - inst;

    while (*s=='.' && cnt<MAX_QUALIFIERS)
    {
        s++;
        ext[cnt] = s;
        while (*s && *s!='.' && !isspace((unsigned char)*s))
            s++;
        ext_len[cnt] = s - ext[cnt];
        if (ext_len[cnt] <= 0)
            cpu_error(34);  /* illegal opcode extension */
        else
            cnt++;
    }
    *ext_cnt = cnt;

    if (cnt > 0)
        current_ext = tolower((unsigned char)ext[0][0]);
    else
        current_ext = '\0';

    return s;
}


int set_default_qualifiers(char **q,int *q_len)
/* fill in pointers to default qualifiers, return number of qualifiers */
{
    q[0] = "w";
    q_len[0] = 1;
    return 1;
}

/* Does not do much useful parsing yet. */
int parse_operand(char *p,int len,operand *op,int requires)
{
    op->type = requires;
    op->value = NULL;
    char arg[32];
    int i;
    
    if(requires == OP_DATA )
    {
        op->value = parse_expr(&p);

        return 1;
    }


    //rxx[rxx]
    if(requires == OP_RRG )
    {
        if( (len < 6) || (len > 8) ) return 0;
        if( !(p[0] == 'r' || p[0] == 'R') )
            return 0;
        
        int a2 = 0;
        arg[0] = p[1];
        if(p[2] == '[')
        {
            arg[1] = 0;
            a2 = 3;
        }else
        {
            arg[1] = p[2];
            arg[2] = 0;
            a2 = 4;
        }
        op->reg = atoi(arg);
        if(op->reg > 63) return 0;

        arg[0] = p[a2+1];
        if(p[a2+2] == ']')
        {
            arg[1] = 0;
        }else
        {
            arg[1] = p[a2+2];
            arg[2] = 0;
        }
        op->reg2 = atoi(arg);

        if(op->reg2 > 63) return 0;

        return 1;
    }

    //imm[rxx]
    if(requires == OP_IMR )
    {
        i = 0;
        
        while(p[i] != '[')
        {
            arg[i] = p[i];
            i++;
        }

        len -= i;
        if(len < 4) return 0;
        if(len > 6) return 0;

        if( !(p[i+1] == 'r' || p[i+1] == 'R') )
            return 0;


        arg[0] = p[i+2];
        if(len == 4)
        {
            arg[1] = 0;


        }else
        {
            arg[1] = p[i+3];
            arg[2] = 0;
        }

        op->reg = atoi(arg);

        op->value = parse_expr(&p);

        return 1;
    }
    
    //Register VF0-VF63
    if(requires == OP_VRG )
    {
        
        if( (len < 2) || (len > 4) ) return 0;
        if( !(p[0] == 'v' || p[0] == 'V') )
            return 0;

        op->val = -1;

        
        arg[0] = p[1];
        if(len == 3)
        {
            arg[1] = 0;
        }
        {
        	if( (p[2] >= '0') && (p[2] <= '9') )
            	arg[1] = p[2];
            else
            {
            	op->val = 'x'-p[2]+1;
            	arg[1] = 0;
            }

            arg[2] = 0;

            if(!( (p[3] >= '0') && (p[3] <= '9') ))
            {
            	op->val = 'x'-p[3]+1;            	
            }
        }

        op->reg = atoi(arg);
        if(op->reg > 63) return 0;

        return 1;
    }


    //Register R0-R63
    if(requires == OP_REG )
    {
        if( (len < 2) || (len > 3) ) return 0;
        if( !(p[0] == 'r' || p[0] == 'R') )
            return 0;
        
        
        arg[0] = p[1];
        if(len == 2)
        {
            arg[1] = 0;
        }
        {
            arg[1] = p[2];
            arg[2] = 0;
        }
        op->reg = atoi(arg);
        if(op->reg > 63) return 0;
        return 1;
    }

    //Register LR
    if(requires == OP_RLR )
    {
        if(len != 2) return 0;
        if( !(p[0] == 'l' || p[0] == 'L') )
            return 0;

        if( !(p[1] == 'r' || p[1] == 'R') )
            return 0;
        
        return 1;
    }


    //Register FR
    if(requires == OP_RFR )
    {
        if(len != 2) return 0;
        if( !(p[0] == 'f' || p[0] == 'F') )
            return 0;

        if( !(p[1] == 'r' || p[1] == 'R') )
            return 0;
        
        return 1;
    }

    //Register BR
    if(requires == OP_RBR )
    {
        if(len != 2) return 0;
        if( !(p[0] == 'b' || p[0] == 'B') )
            return 0;

        if( !(p[1] == 'r' || p[1] == 'R') )
            return 0;
        
        return 1;
    }

    //Register IR
    if(requires == OP_RIR )
    {
        if(len != 2) return 0;
        if( !(p[0] == 'i' || p[0] == 'I') )
            return 0;

        if( !(p[1] == 'r' || p[1] == 'R') )
            return 0; 
        
        return 1;
    }
/*
    //Register PC
    if(requires == OP_RPC )
    {
        if(len != 2) return 0;
        if( !(p[0] == 'p' || p[0] == 'P') )
            return 0;

        if( !(p[1] == 'c' || p[1] == 'C') )
            return 0; 
        
        return 1;
    }
*/
    //Register Q
    if(requires == OP_RGQ )
    {
        if(len != 1) return 0;
        if( !(p[0] == 'q' || p[0] == 'Q') )
            return 0;
        
        return 1;
    }

    //Register P
    if(requires == OP_RGP )
    {
        if(len != 1) return 0;
        if( !(p[0] == 'p' || p[0] == 'P') )
            return 0;
        
        return 1;
    }

   
    //Immediate
    if( (requires >= OP_IMM) && (requires <= OP_IML) )
    {
        
        if(len > 2)
        {
            if(p[0] == '0' && p[1] == 'x') 
            {
                unsigned long hexa = 0,number;
                for(int i = 2;i < len;i++)
                {
                    if(p[i] >= '0' && p[i] <= '9')
                        number = p[i]- '0';
                    if(p[i] >= 'A' && p[i] <= 'F')
                        number = p[i]- 'A' + 10;

                    hexa += number<<( (len - i-1)*4);
                }
            }
            op->val = 5;

            
        }else
        {
            

        }

        //op->val = atoi(p);
        //op->fval = atof(p);
        op->value = parse_expr(&p);

        //printf("%d\n",op->val);
        
        return 1;
    }

    if(requires == OP_IMF)
    {
        //op->val = atoi(p);
        op->fval = atof(p);
        memcpy(&op->val, &op->fval, 4);
        //printf("%d\n",op->val);
        
        return 1;
    }


    return 0;
}

char *outbin(unsigned int n)
{
    static char buffer [1+sizeof (unsigned int)*8] = { 0 };
    unsigned int bit = 0 ;
    int i;
    for (i = 0 ; i < sizeof (unsigned int)*8; i++)
    {
        bit = n&1;
        buffer[i] = bit+'0';
        n = n>>1;
    }
    buffer[i] = '\0';

    return buffer;
}

void printbin(unsigned int n)
{
    unsigned int bit = 0 ;
    for (int i = 0 ; i < sizeof(unsigned int)*8 ; i++)
    {
        bit = n&1;
        printf("%d", bit) ;
        if(((i+1)&7) == 0) printf(" ") ;
        n = n>>1;
    }
    printf("\n");
}

/* Convert an instruction into a DATA atom including relocations,
   if necessary. */
dblock *eval_instruction(instruction *p,section *sec,taddr pc)
{
    
    dblock *db=new_dblock();

    unsigned char *d;
    int opcode = (mnemonics[p->code].ext.opcode);
    int val;
    operand operand1,operand2,operand3;

    int k1ext = 0x00;

    if( ((opcode>>5)&0x07) == 2 )
        k1ext = 0x02;

    if(p->qualifiers[0] != 0)
    {
        char *ext = p->qualifiers[0];
        int i = 0,n;

        while(p->qualifiers[0][i]) i++;
        n = i;

        if(ext[0] == 'b') k1ext = 0;
        if(ext[0] == 'w') k1ext = 1;
        if(ext[0] == 'd') k1ext = 2;
        if(ext[0] == 'q') k1ext = 3;

        
        if(ext[0] == 'x') k1ext = 0;
        if( (n == 2) && (ext[1] == 'y') ) k1ext = 1;
        if( (n == 3) && (ext[2] == 'z') ) k1ext = 2;
        if( (n == 4) && (ext[3] == 'w') ) k1ext = 3;

        if(ext[0] == 'p') opcode |= 1;
        if( (n == 2) && (ext[1] == 'p') ) opcode |= 1;
        if( (n == 3) && (ext[2] == 'p') ) opcode |= 1;
        if( (n == 4) && (ext[3] == 'p') ) opcode |= 1;
        if( (n == 5) && (ext[4] == 'p') ) opcode |= 1;
    } 
    
    if(p->op[0] != NULL)
        operand1 = *(p->op[0]);
    else
        operand1.type = OP_VOID;

    if(p->op[1] != NULL)
        operand2 = *(p->op[1]);
    else
        operand2.type = OP_VOID;
        
    if(p->op[2] != NULL)
        operand3 = *(p->op[2]);
    else
        operand3.type = OP_VOID;

    opcode |= (k1ext<<8);

    //------------- OPERAND 1 -------------
    if( (operand1.type == OP_REG) || (operand1.type == OP_VRG) )
    {
        opcode |= (operand1.reg<<26);
    }

    if(operand1.type == OP_IMB)
    { 
        eval_expr(operand1.value,&val,sec,pc);
        val = (val-pc-4-1)>>2;

        int toffset = 0;
        if(opcode&1)
            toffset = 1;
            
        val -= toffset;

        operand1.val = val&0x3FFFFF;

        opcode |= (operand1.val<<10);
    }

    if(operand1.type == OP_ILB)
    { 
        eval_expr(operand1.value,&val,sec,pc);

        if(val == 0)
        {
            operand1.val = 0;
        }else
        {
            val = (val+4-1)>>2;
            
            operand1.val = val&0xFFFFFF;
        }
        
        opcode |= (operand1.val<<8);
    }

    if(operand1.type == OP_IMR)
    {
        opcode |= (operand1.reg<<20);

        eval_expr(operand1.value,&val,sec,pc);
        opcode |= ( (val & 0xFFFF)<<10);
    }

    if(operand1.type == OP_RRG)
    {
        opcode |= (operand1.reg<<20);
        opcode |= (operand1.reg2<<14);
    }


    if( (operand1.type >= OP_RLR) && (operand1.type <= OP_RIR ) )
    {
        opcode |= (operand1.type&0x3)<<26;
    }

    if(operand1.type == OP_RGQ)
    {
        opcode |= (0x3F<<26);
    }

    if(operand1.type == OP_RGP)
    {
        opcode |= (0x3E<<26);
    }

    //------------- OPERAND 2 -------------
    if( (operand2.type == OP_REG) || (operand2.type == OP_VRG) )
    {
        opcode |= (operand2.reg<<20);
    }

    if(operand2.type == OP_IMM)
    {
        eval_expr(operand2.value,&val,sec,pc);
        operand2.val = val&0xFFFF;
        opcode |= (operand2.val<<10);
    }

    if(operand2.type == OP_IML)
    {
        eval_expr(operand2.value,&val,sec,pc);
        operand2.val = val&0x3FFFF;
        opcode &= 0xFFFFFCFF;
        opcode |= (operand2.val<<8);
       
    } 

    if(operand2.type == OP_RRG)
    {
        opcode |= (operand2.reg<<20);
        opcode |= (operand2.reg2<<14);
    }

    if(operand2.type == OP_IMR)
    {
        opcode |= (operand2.reg<<20);

        eval_expr(operand2.value,&val,sec,pc);
        opcode |= ( (val & 0x3FF)<<10);
    }

    if(operand2.type == OP_IMF)
    {
        unsigned int tmp,sign,exp,mant,texp;

        tmp = operand2.val;

        sign = (tmp>>16)&0x8000; //sign
        texp = (tmp>>23)&0x00FF; //exp
        mant = (tmp>>13)&0x03FF; //mantisse
        
        exp = (texp&0x0F)<<10;

        if(texp&0x80)
            exp |= 0x4000;

        val = sign+mant+exp;

        opcode |= ( (val & 0xFFFF)<<10);
    }

    if( (operand2.type >= OP_RLR) && (operand2.type <= OP_RIR ) )
    {
        opcode |= (operand2.type&0x3)<<20;
    }


    //------------- OPERAND 3 -------------
    if( (operand3.type == OP_REG) || (operand3.type == OP_VRG) )
    {
        opcode |= (operand3.reg<<14);
    }

    if(operand3.type == OP_IMM)
    {
        eval_expr(operand3.value,&val,sec,pc);
        operand3.val = val&0x3FF;
        opcode |= (operand3.val<<10);
    }

    if(operand3.type == OP_IMS)
    {
        eval_expr(operand3.value,&val,sec,pc);
        operand3.val = val&0x1FF;
        opcode |= (operand3.val<<11); 
    }

    //-------------
    //printf("%x\n",opcode);
   // printbin(opcode);

    val = opcode;
    db->size = 4;
    
	d = db->data = mymalloc(db->size);
	*d++ = (val)&0xFF;
	*d++ = (val>>8)&0xFF;
	*d++ = (val>>16)&0xFF;
    *d++ = (val>>24)&0xFF;

    return db;
}

/* Create a dblock (with relocs, if necessary) for size bits of data. */
dblock *eval_data(operand *op,taddr bitsize,section *sec,taddr pc)
{
    dblock *db=new_dblock();
    unsigned char *d;
    operand operand1;
    int val;

    operand1 = op[0];

    eval_expr(operand1.value,&val,sec,pc);

    db->size = bitsize>>3;
    d = db->data = mymalloc(db->size);
    
    int shift = 8,i;
    for(i = 0;i < db->size;i++)
    {
       *d++ = (val)&0xFF;
        val = val>>shift;
        shift+= 8;
    }

    return db;
}



/* Calculate the size of the current instruction; must be identical
   to the data created by eval_instruction. */
taddr instruction_size(instruction *p,section *sec,taddr pc)
{
    return 4;
}

operand *new_operand()
{
    operand *new = mymalloc(sizeof(*new));
    new->type = -1;
    return new;
}


/* return true, if initialization was successfull */
int init_cpu()
{
    return 1;
}

/* return true, if the passed argument is understood */
int cpu_args(char *p)
{

    /* no args */
    return 0;
}
/* parse cpu-specific directives; return pointer to end of
   cpu-specific text */
char *parse_cpu_special(char *start)
{
    //printf("ok\n",len);
    /* no specials */
    return start;
}

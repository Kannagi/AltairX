/* symbol.h - manage all kinds of symbols */
/* (c) in 2014 by Volker Barthelmann and Frank Wille */

#ifndef SYMBOL_H
#define SYMBOL_H

/* symbol types */
#define LABSYM 1
#define IMPORT 2
#define EXPRESSION 3

/* symbol flags */
#define TYPE(sym) ((sym)->flags&7)
#define TYPE_UNKNOWN  0
#define TYPE_OBJECT   1
#define TYPE_FUNCTION 2
#define TYPE_SECTION  3
#define TYPE_FILE     4

#define EXPORT (1<<3)
#define INEVAL (1<<4)
#define COMMON (1<<5)
#define WEAK (1<<6)
#define VASMINTERN (1<<7)
#define PROTECTED (1<<8)
#define RSRVD_S (1L<<24)    /* bits 24..27 are reserved for syntax modules */
#define RSRVD_O (1L<<28)    /* bits 28..31 are reserved for output modules */

struct symbol {
  struct symbol *next;
  int type;
  uint32_t flags;
  char *name;
  expr *expr;
  expr *size;
  section *sec;
  taddr pc;
  taddr align;
  uint32_t idx; /* usable by output module */
};


#ifdef HAVE_REGSYMS
/* register symbols */
struct regsym {
  char *reg_name;
  int reg_type;
  unsigned int reg_flags;
  unsigned int reg_num;
};
#endif /* HAVE_REGSYMS */


extern symbol *first_symbol;

void print_symbol(FILE *,symbol *);
void add_symbol(symbol *);
symbol *find_symbol(char *);
void refer_symbol(symbol *,char *);
void save_symbols(void);
void restore_symbols(void);

char *make_local_label(char *,int,char *,int);
symbol *new_abs(char *,expr *);
symbol *new_import(char *);
symbol *new_labsym(section *,char *);
symbol *new_tmplabel(section *);
symbol *internal_abs(char *);
expr *set_internal_abs(char *,taddr);

#ifdef HAVE_REGSYMS
void add_regsym(regsym *);
regsym *find_regsym(char *,int);
regsym *find_regsym_nc(char *,int);
regsym *new_regsym(int,char *,int,unsigned int,unsigned int);
#endif /* HAVE_REGSYMS */

int init_symbol(void);

#endif

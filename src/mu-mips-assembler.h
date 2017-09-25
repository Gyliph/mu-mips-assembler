#include <stdint.h>
#define NUM_INSTRUCTIONS 32

char REG_LOOKUP[32][10] = {
  "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
  "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
  "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
  "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

void parse_program(FILE*, FILE*);
uint32_t instruction_type_special(char*, uint32_t);
uint32_t instruction_type_normal(char*, uint32_t);
uint32_t instruction_type_branch(char*, uint32_t);
uint32_t findIn_regLookup(char*);

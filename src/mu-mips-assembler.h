#include <stdint.h>

char REG_LOOKUP[32][10] = {
  "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
  "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
  "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
  "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"
};

void parse_program(FILE*, FILE*);
void instruction_type_one(char*);
void instruction_type_two(char*);
void instruction_type_three(char*);
uint32_t findIn_regLookup(char*);

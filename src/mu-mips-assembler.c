#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "mu-mips-assembler.h"

void parse_program(FILE *infp, FILE *outfp){

  	/* Read in the program. */
  int len = 25;
  char str[len];
  char line[len];
  const char s[7] = ", ()\n\t";
  char *token;
  uint32_t instruction;

  while( fgets(str, len, infp) != NULL ) {
    instruction = 0x00000000;
    token = strtok(str, s);

    if(token != NULL && token[0] != '#'){
      if(strcmp(token, "add") == 0){
        instruction |= 0x00000020;
      	instruction = instruction_type_special(token, instruction);
      }else if(strcmp(token, "addi") == 0){
        instruction |= 0x20000000;
      	instruction = instruction_type_normal(token, instruction);
      }else if(strcmp(token, "addu") == 0){
        instruction |= 0x00000021;
      	instruction = instruction_type_special(token, instruction);
      }else if(strcmp(token, "addiu") == 0){
        instruction |= 0x24000000;
      	instruction = instruction_type_normal(token, instruction);
      }else if(strcmp(token, "beq") == 0){
        instruction |= 0x10000000;
        instruction = instruction_type_branch(token, instruction);
    	}else if(strcmp(token, "bne") == 0){
        instruction |= 0x14000000;
        instruction = instruction_type_branch(token, instruction);
      }else if(strcmp(token, "sll") == 0){
        instruction |= 0x00000000;
        instruction = instruction_type_shift(token, instruction);
      }else if(strcmp(token, "sw") == 0){
        instruction |= 0xAC000000;
        instruction = instruction_type_store_load(token, instruction);
      }else if(strcmp(token, "bgez") == 0){
        instruction |= 0x04010000;
        instruction = instruction_type_branch_z(token, instruction);
      }else if(strcmp(token, "blez") == 0){
        instruction |= 0x18000000;
        instruction = instruction_type_branch_z(token, instruction);
      }else if(strcmp(token, "sub") == 0){
        instruction |= 0x00000022;
        instruction = instruction_type_special(token, instruction);
      }else if(strcmp(token, "lw") == 0){
        instruction |= 0x8C000000;
        instruction = instruction_type_store_load(token, instruction);
      }else if(strcmp(token, "mult") == 0){
        instruction |= 0x00000018;
        instruction = instruction_type_mult(token, instruction);
      }else if(strcmp(token, "mflo") == 0){
        instruction |= 0x00000012;
        instruction = instruction_type_mf(token, instruction);
      }else if(strcmp(token, "syscall") == 0){
        instruction |= 0x0000000C;
      }

      if(instruction != 0x0){
        printf("instr: %08x\n", instruction);
        sprintf(line, "%08x\n", instruction);
        fputs(line, outfp);
      }
    }
  }
}

uint32_t instruction_type_special(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  int i;
  uint32_t rs = 0x0;
  uint32_t rt = 0x0;
  uint32_t rd = 0x0;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rd
        rd = (findIn_regLookup(token) << 11);
        break;
      case 1: //rs
        rs = (findIn_regLookup(token) << 21);
        break;
      case 2: //rt
        rt = (findIn_regLookup(token) << 16);
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }

  instruction |= rd;
  instruction |= rs;
  instruction |= rt;

  return instruction;
}

uint32_t instruction_type_mult(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  int i;
  uint32_t rs = 0x0;
  uint32_t rt = 0x0;

  for(i=0; i<2; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rd
        rs = (findIn_regLookup(token) << 21);
        break;
      case 1: //rs
        rt = (findIn_regLookup(token) << 16);
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }

  instruction |= rs;
  instruction |= rt;

  return instruction;
}

uint32_t instruction_type_mf(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  uint32_t rd = 0x0;

  token = strtok(NULL, s);
  rd = (findIn_regLookup(token) << 11);
  instruction |= rd;

  return instruction;
}

uint32_t instruction_type_shift(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  int i;
  uint32_t rt = 0x0;
  uint32_t rd = 0x0;
  uint32_t sa = 0x0;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rd
        rd = (findIn_regLookup(token) << 11);
        break;
      case 1: //rs
        rt = (findIn_regLookup(token) << 16);
        break;
      case 2: //rt
        sa = strtol(token, NULL, 16);
        sa = sa << 6;
        sa &= 0x000007C0;
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }

  instruction |= rd;
  instruction |= rt;
  instruction |= sa;

  return instruction;
}

uint32_t instruction_type_store_load(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  int i;
  uint32_t rt = 0x0;
  uint32_t offset = 0x0;
  uint32_t base = 0x0;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rd
        rt = (findIn_regLookup(token) << 16);
        break;
      case 1: //rs
        offset = strtol(token, NULL, 16);
        offset &= 0x0000FFFF;
        break;
      case 2: //rt
        base = (findIn_regLookup(token) << 21);
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }

  instruction |= rt;
  instruction |= offset;
  instruction |= base;

  return instruction;
}

uint32_t instruction_type_normal(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  int i;
  uint32_t rs = 0x0;
  uint32_t rt = 0x0;
  uint32_t immediate = 0x0;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rt
        rt = (findIn_regLookup(token) << 16);
        break;
      case 1: //rs
        rs = (findIn_regLookup(token) << 21);
        break;
      case 2: //immediate
        immediate = strtol(token, NULL, 16);
        immediate &= 0x0000FFFF;
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }

  instruction |= rt;
  instruction |= rs;
  instruction |= immediate;

  return instruction;
}

uint32_t instruction_type_branch(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  int i;
  uint32_t rs = 0x0;
  uint32_t rt = 0x0;
  uint32_t offset = 0x0;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rs
        rs = (findIn_regLookup(token) << 21);
        break;
      case 1: //rt
        rt = (findIn_regLookup(token) << 16);
        break;
      case 2: //offset
        offset = strtol(token, NULL, 16);
        offset &= 0x0000FFFF;
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }

  instruction |= rs;
  instruction |= rt;
  instruction |= offset;

  return instruction;
}

uint32_t instruction_type_branch_z(char *token, uint32_t instruction){
  const char s[7] = ", ()\n\t";
  int i;
  uint32_t rs = 0x0;
  uint32_t offset = 0x0;

  for(i=0; i<2; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rs
        rs = (findIn_regLookup(token) << 21);
        break;
      case 1: //rt
        offset = strtol(token, NULL, 16);
        offset &= 0x0000FFFF;
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }

  instruction |= rs;
  instruction |= offset;

  return instruction;
}

uint32_t findIn_regLookup(char *s){
  uint32_t i;
  for(i=0; i<32; i++){
    if(strcmp(s, REG_LOOKUP[i]) == 0){
      return i;
    }
  }
  return 0x0; //couldn't find
}

int main(int argc, char *argv[]){
  if(argc != 3){
    printf("Incorrect number of input arguments\n");
    exit(1);
  }

  FILE *infp;
  infp = fopen(argv[1], "r");
  if(!infp){
    printf("Can't open input file\n");
    exit(2);
  }

  FILE *outfp;
  outfp = fopen(argv[2], "w");
  if(!outfp){
    printf("Cant open output file\n");
    exit(3);
  }

  parse_program(infp, outfp);

  return 0;
}

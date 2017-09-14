#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mu-mips-assembler.h"

void parse_program(FILE *infp, FILE *outfp){

  	/* Read in the program. */
  int len = 25;
  char [len];
  const char s[2] = " ,";
  char *token;
  int rt,rs,immediate;

  char rt[5]

  while( fgets(str, len, infp) != NULL ) {
    token = strtok(str, s);

    if(strcmp(token, "ADD") == 0){
    	instruction_type_one(token);
    }else if(strcmp(token, "ADDI") == 0){
    	instruction_type_two(token);
    }else if(strcmp(token, "ADDU") == 0){
    	insturction_type_one(token);
    }else if(strcmp(token, "ADDIU") == 0){
    	instruction_type_two(token);
    }else if(strcmp(token, "BEQ") == 0){
      instruction_type_three(token);
  	}else if(strcmp(token, "BNE") == 0){
      instruction_type_three(token);
    while(token != NULL){
      token = strtok(NULL, s);
    }
  }
}

void instruction_type_one(char *tok){
  const char s[2] = " ,";
  int i;
  uint32_t rd, rs, rt;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rd
        rd = findIn_regLookup(token);
        break;
      case 1: //rs
        rs = findIn_regLookup(token);
        break;
      case 2: //rt
        rt = findIn_regLookup(token);
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }
}

void instruction_type_two(char *tok){
  const char s[2] = " ,";
  int i;
  uint32_t rt, rs, immediate;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rt
        rt = findIn_regLookup(token);
        break;
      case 1: //rs
        rs = findIn_regLookup(token);
        break;
      case 2: //immediate
        immediate = token;
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }
}

void instruction_type_three(char *tok){
  const char s[2] = " ,";
  int i;
  uint32_t rs, rt, offset;

  for(i=0; i<3; i++){
    token = strtok(NULL, s);
    switch(i){
      case 0: //rs
        rs = findIn_regLookup(token);
        break;
      case 1: //rt
        rt = findIn_regLookup(token);
        break;
      case 2: //offset
        offsest = token;
        break;
      default:
        printf("Error\n");
        exit(3);
        break;
    }
  }
}

uint32_t findIn_regLookup(char *s){
  uint32_t i;
  for(i=0; i<32; i++){
    if(strcmp(s, REG_LOOKUP[i]) == 0){
      return i;
    }
  }
  return -1; //couldn't find
}

int main(int argc, char *argv[]){
  if(argc != 2){
    printf("Incorrect number of input arguments\n");
    exit(1);
  }

  FILE *infp;
  infp = fopen(argv[2], "r");
  if(!infp){
    printf("Can't open program file\n");
    exit(2);
  }

  FILE *outfp;


  parse_program(infp, outfp);

  return 0;
}

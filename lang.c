#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int stack;
} VirtualMachine;

const int MAX_LINE_SIZE = 1024;

typedef enum {
  SET = 1,
  ADD,
  GOTO,
  INTEGER,
  DOTS,
  THREE_DOTS,
  HASHTAG,
  CLIPBOARD_REFERENCE,
  CELL_REFERENCE,
  COMMENT,
} TokenType;

typedef struct {
  TokenType Kind;
  char *Literal;
} Token;

void print_token(Token *token) {
  printf("Token kind: %d\n", token->Kind);
  printf("Token literal: %s\n", token->Literal);
}

typedef struct {
  Token *tokens[5];
} Instruction;

void print_instruction(Instruction *instruction) {
  for (int i = 0; i < 5; i++) {
    if (instruction->tokens[i] == NULL) {
      break;
    }
    print_token(instruction->tokens[i]);
  }
}

Token *new_token(char *literal, TokenType kind) {
  Token *token = malloc(sizeof(Token));
  if (token != NULL) {
    perror("Error while allocating memory for token: ");
    return NULL;
  }
  token->Literal = literal;
  token->Kind = kind;
  return token;
}

void free_token(Token *token) {
  if (token == NULL) {
    return;
  }
  if (token->Literal != NULL) {
    free(token->Literal);
  }
  free(token);
  printf("Token freed\n");
}

Instruction *interpret(char line[MAX_LINE_SIZE]) {
  char *line_dup = strdup(line);
  Instruction *instruction = malloc(sizeof(Instruction));
  if (instruction == NULL) {
    perror("Error while allocating memory for instruction: ");
    return NULL;
  }
  line = strtok(line, " ");
  if (strcmp("#", line) == 0) {
    Token *token = malloc(sizeof(Token));
    if (token == NULL) {
      perror("Error while allocating memory for token: ");
      return NULL;
    }
    token->Kind = COMMENT;
    token->Literal = line_dup;
    instruction->tokens[0] = token;
    return instruction;
  }
  while ((line = strtok(NULL, " ")) != NULL) {
    printf("%s", line);
  }
  printf("\n");
  return instruction;
}

void free_instruction(Instruction *instruction) {
  if (instruction == NULL) {
    return;
  }
  for (int i = 0; i < 5; i++) {
    if (instruction->tokens[i] != NULL) {
      free_token(instruction->tokens[i]);
    }
  }
  free(instruction);
  printf("Instruction freed\n");
}

void evaluate(Instruction *instruction) {
  if (instruction == NULL) {
    return;
  }
  print_instruction(instruction);
  free_instruction(instruction);
}

void repl() {
  int choice = 0;
  char buffer[MAX_LINE_SIZE];
  do {
    printf(">> ");
    fgets(buffer, MAX_LINE_SIZE, stdin);
    Instruction *instruction = interpret(buffer);
    evaluate(instruction);
  } while (1);
}

int main(int argc, char **argv) {
  if (argc != 1 && argc != 2) {
    printf("Usage: lang <file>.lang\nor lang to launch the repl mode\n");
    return 0;
  }
  if (argc == 1) {
    repl();
    return 0;
  }
  if (argc == 2) {
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
      perror("Error while opening file: ");
      return 1;
    }
    char buffer[MAX_LINE_SIZE];
    while (fgets(buffer, MAX_LINE_SIZE, f) != NULL) {
      Instruction *instruction = interpret(buffer);
      evaluate(instruction);
    }
  }
  return 0;
}

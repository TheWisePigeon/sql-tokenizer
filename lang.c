#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *HELP_MESSAGE = "Usage: lang file.lang\n";
const int MAX_TOKENS_IN_INSTRUCTION = 4;
const int MAX_TOKEN_LENGTH = 10;
const int MAX_CELL_REFERENCE_LENGTH = 5;
const int MIN_CELL_REFERENCE_LENGTH = 2;

typedef enum {
  OUT,
  OUT_CHAR,
  GOTO,
  COPY,
  SET,
  ADD,
  SUB,
  MUL,
  DIV,
  CELL_REFERENCE,
  CLIPBOARD_REFERENCE,
  DOT,
  THREE_DOTS,
  INTEGER,
  INVALID,
} TokenKind;

typedef struct {
  TokenKind token_kind;
  void *token_literal;
  /* char *token_parsing_error; // only INVALID tokens will have this */
} Token;

typedef struct {
  Token *tokens;
} Instruction;

void free_instruction(Instruction *instruction) {
  if (instruction != NULL) {
  }
}

Token *token_from_literal(char *literal) {
  Token *token = malloc(sizeof(Token));
  token->token_literal = literal;
  switch (literal[0]) {
  case 'O':
    if (strcmp(literal, "OUT")) {
      token->token_kind = OUT;
      return token;
    }
    if (strcmp(literal, "OUT_CHAR")) {
      token->token_kind = OUT_CHAR;
      return token;
    }
  case 'G':
    if (strcmp(literal, "GOTO")) {
      token->token_kind = GOTO;
      return token;
    }
  case 'C':
    if (strcmp(literal, "COPY")) {
      token->token_kind = GOTO;
      return token;
    }
  case 'S':
    if (strcmp(literal, "SET")) {
      token->token_kind = SET;
      return token;
    }
    if (strcmp(literal, "SUB")) {
      token->token_kind = SUB;
      return token;
    }
  case 'A':
    if (strcmp(literal, "ADD")) {
      token->token_kind = ADD;
      return token;
    }
  case 'M':
    if (strcmp(literal, "MUL")) {
      token->token_kind = MUL;
      return token;
    }
  case 'D':
    if (strcmp(literal, "DIV")) {
      token->token_kind = DIV;
      return token;
    }
  case '$':
    if (strlen(literal) < MIN_CELL_REFERENCE_LENGTH) {
      token->token_kind = INVALID;
      return token;
    }
    if (isdigit(literal[1])) {
      int len = strlen(literal);
      if (len <= MAX_CELL_REFERENCE_LENGTH) {
        int *value = malloc(sizeof(int));
        int result = sscanf(literal + 1, "%d", value);
        if (result == 1) {
          token->token_literal = value;
          token->token_kind = CELL_REFERENCE;
        } else {
          token->token_kind = INVALID;
          free(value);
          return token;
        }
      }
    }
    if (literal[1] == 'C') {
      if (strcmp(literal, "$CLIPBOARD")) {
        token->token_kind = CLIPBOARD_REFERENCE;
        return token;
      }
    }
  case '.':
    if (strcmp(literal, ".")) {
      token->token_kind = DOT;
      return token;
    }
    if (strcmp(literal, "...")) {
      token->token_kind = THREE_DOTS;
      return token;
    }
  case '-':
    if (strlen(literal) < MIN_CELL_REFERENCE_LENGTH) {
      token->token_kind = INVALID;
      return token;
    }
    int *value = malloc(sizeof(int));
    int result = sscanf(literal, "%d", value);
    if (result == 1) {
      token->token_kind = INTEGER;
      token->token_literal = value;
    } else {
      token->token_kind = INVALID;
      token->token_literal = value;
    }
    return token;
  default:
    if (isdigit(literal[0])) {
      // parse positive integer literal
    }
  }
  token->token_kind = INVALID;
  return token;
}

void free_token(Token *token) {
  if (token != NULL) {
    free(token->token_literal);
    free(token);
  }
}

const char *display_token_literal(TokenKind token_kind) {
  switch (token_kind) {
  case OUT:
    return "out";
  case OUT_CHAR:
    return "out_char";
  case GOTO:
    return "goto";
  case COPY:
    return "copy";
  case SET:
    return "set";
  case ADD:
    return "add";
  case SUB:
    return "sub";
  case MUL:
    return "mul";
  case DIV:
    return "div";
  case CELL_REFERENCE:
    return "cell_reference";
  case CLIPBOARD_REFERENCE:
    return "clipboard_reference";
  case DOT:
    return "dot";
  case THREE_DOTS:
    return "three_dots";
  case INTEGER:
    return "integer";
  case INVALID:
    return "invalid";
  default:
    return "";
  }
}

typedef struct {
  char *buffer;
  uint8_t buffer_size;
  uint8_t position;
  uint8_t read_position;
  char current_character;
} Lexer;

char peek_char(Lexer *lexer) {
  if (lexer->read_position >= lexer->buffer_size) {
    return EOF;
  }
  return lexer->buffer[lexer->read_position];
}

char read_char(Lexer *lexer) {
  lexer->current_character = peek_char(lexer);
  lexer->position = lexer->read_position;
  lexer->read_position++;
  return lexer->current_character;
}

void skip_whitespace(Lexer *lexer) {
  while (isspace(lexer->current_character)) {
    read_char(lexer);
  }
}

void skip_line(Lexer *lexer) {
  while (lexer->current_character != '\n' && lexer->current_character != EOF) {
    read_char(lexer);
  }
}

Lexer *new_lexer(char *buffer, uint8_t buffer_size) {
  Lexer *lexer = malloc(sizeof(Lexer));
  lexer->buffer = buffer;
  lexer->buffer_size = buffer_size;
  lexer->position = 0;
  lexer->read_position = 0;
  lexer->current_character = 0;
  read_char(lexer);
  return lexer;
}

Instruction *new_instruction() {
  Instruction *instruction = malloc(sizeof(Instruction));
  instruction->tokens = malloc(sizeof(Token) * 3);
  return instruction;
}

void evaluate_instruction(Instruction *instruction) {}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("%s", HELP_MESSAGE);
    return EXIT_SUCCESS;
  }
  char *file_name = argv[1];
  char *line = NULL;
  size_t line_size = 0;
  ssize_t read = 0;
  FILE *file = NULL;
  file = fopen(file_name, "rb");
  if (file == NULL) {
    perror("Error while opening file");
    return EXIT_FAILURE;
  }
  while ((read = (getline(&line, &line_size, file)) != -1)) {
    char *token_literal = strtok(line, " ");
    if (token_literal != NULL) {
      printf("%s", token_literal);
      while ((token_literal = strtok(NULL, " ")) != NULL) {
        printf("%s", token_literal);
      }
    }
  }
  return EXIT_SUCCESS;
}

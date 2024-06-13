#include <complex.h>
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
  COMMENT,
  INVALID,
} TokenKind;

typedef struct {
  TokenKind token_kind;
  char *token_literal;
  char *token_parsing_error;
} Token;

typedef struct {
  Token *tokens;
} Instruction;

void lowercase(char *str) {
  for (int i = 0; i < strlen(str); i++) {
    if (isalpha(str[i])) {
      str[i] = tolower(str[i]);
    }
  }
}

void free_instruction(Instruction *instruction) {
  if (instruction != NULL) {
  }
}

Token *token_from_literal(char *literal) {
  Token *token = malloc(sizeof(Token));
  if (!token) {
    perror("Error while allocating memory for token");
    exit(EXIT_FAILURE);
  }
  lowercase(literal);
  token->token_literal = literal;
  char first_char = literal[0];
  if (first_char == '#') {
    token->token_kind = COMMENT;
    return token;
  }
  if (first_char == 'o') {
    if (strcmp(literal, "out") == 0) {
      token->token_kind = OUT;
      return token;
    }
    if (strcmp(literal, "out_char") == 0) {
      token->token_kind = OUT_CHAR;
      return token;
    }
  }
  if (first_char == 'g') {
    if (strcmp(literal, "goto") == 0) {
      token->token_kind = GOTO;
      return token;
    }
  }
  if (first_char == 'c') {
    if (strcmp(literal, "copy") == 0) {
      token->token_kind = GOTO;
      return token;
    }
  }
  if (first_char == 's') {
    if (strcmp(literal, "set") == 0) {
      token->token_kind = SET;
      return token;
    }
    if (strcmp(literal, "sub") == 0) {
      token->token_kind = SUB;
      return token;
    }
  }
  if (first_char == 'a') {
    if (strcmp(literal, "add") == 0) {
      token->token_kind = ADD;
      return token;
    }
  }
  if (first_char == 'm') {
    if (strcmp(literal, "mul") == 0) {
      token->token_kind = MUL;
      return token;
    }
  }
  if (first_char == 'd') {
    if (strcmp(literal, "div") == 0) {
      token->token_kind = DIV;
      return token;
    }
  }
  if (first_char == '$') {
    if (strlen(literal) < MIN_CELL_REFERENCE_LENGTH) {
      token->token_kind = INVALID;
      return token;
    }
    if (isdigit(literal[1])) {
    }
    if (literal[1] == 'c') {
      if (strcmp(literal, "$clipboard") == 0) {
        token->token_kind = CLIPBOARD_REFERENCE;
        return token;
      }
    }
  }
  if (first_char == '.') {
    if (strcmp(literal, ".") == 0) {
      token->token_kind = DOT;
      return token;
    }
    if (strcmp(literal, "...") == 0) {
      token->token_kind = THREE_DOTS;
      return token;
    }
  }
  if (first_char == '-') {
    token->token_kind = INVALID;
    return token;
  }
  if (isdigit(first_char)) {
    // parse positive integer literal
  }
  token->token_kind = INVALID;
  return token;
}

const char *display_token_kind_literal(TokenKind token_kind) {
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
  case COMMENT:
    return "comment";
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

void print_token(Token *token) {
  if (token != NULL) {
    printf("%s(%s)\n", display_token_kind_literal(token->token_kind),
           token->token_literal);
  }
}

void free_token(Token *token) {
  if (token != NULL) {
    free(token->token_literal);
    free(token);
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
  size_t line_size;
  ssize_t read;
  FILE *file = NULL;
  file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Error while opening file");
    return EXIT_FAILURE;
  }
  while (((read = getline(&line, &line_size, file)) != -1)) {
    if (read > 0 && line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    char *token_literal = strtok(line, " ");
    while (token_literal != NULL) {
      Token *token = token_from_literal(token_literal);
      print_token(token);
      if (token->token_kind == COMMENT) {
        token_literal = strtok(NULL, " ");
        break;
      }
      token_literal = strtok(NULL, " ");
    }
  }
  free(line);
  return EXIT_SUCCESS;
}

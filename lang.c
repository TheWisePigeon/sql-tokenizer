#include <ctype.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const char *HELP_MESSAGE = "Usage: lang file.lang\n";
const int MAX_TOKENS_IN_INSTRUCTION = 4;
const int MAX_TOKEN_LENGTH = 10; //length of CLIPBOARD_REFERENCE '$CLIPBOARD'

typedef enum {
  OUT,
  OUT_CHAR,
  GOTO,
  COPY,
  SET,
  ADD,
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
  char *token_literal;
} Token;

typedef struct {
  Token *tokens;
} Instruction;

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

Token *read_token(Lexer *lexer) {
  char *identifier = malloc(MAX_TOKEN_LENGTH);
  if (isdigit(lexer->current_character)) {
    // read integer
  }
  if (isalpha(lexer->current_character)) {
    // read keyword
  }
  if (lexer->current_character == '$') {
    // read cell reference
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("%s", HELP_MESSAGE);
    return EXIT_SUCCESS;
  }
  char *file_name = argv[1];
  FILE *file = NULL;
  char *buffer = NULL;
  int file_size = 0;
  file = fopen(file_name, "rb");
  if (file == NULL) {
    perror("Error while opening file");
    return EXIT_FAILURE;
  }
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  rewind(file);
  buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    fclose(file);
    perror("Error while allocating memory for buffer:");
    return EXIT_FAILURE;
  }
  size_t read = fread(buffer, 1, file_size, file);
  if (read != file_size) {
    perror("Error while reading file");
    free(buffer);
    fclose(file);
    return EXIT_FAILURE;
  }
  fclose(file);
  buffer[file_size] = '\0';
  Lexer *lexer = new_lexer(buffer, file_size);
  while (peek_char(lexer) != EOF) {
    skip_whitespace(lexer);
    if (lexer->current_character == '#') {
      skip_line(lexer);
    }
    if (lexer->current_character == '\n') {
    }
    Token *token = read_token(lexer);
    display_token_literal(token->token_kind);
  }
  free(buffer);
  free(lexer);
  return EXIT_SUCCESS;
}

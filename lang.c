#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *HELP_MESSAGE = "Usage: lang <file.lang>";

enum TokenType {
  COMMAND,
  CELL_REFERENCE,
  INTEGER,
  DOT,
  THREE_DOTS,
  INVALID,
};

enum Command {
  OUT,
  OUT_CHAR,
  GOTO,
  COPY,
  SET,
  ADD,
  SUB,
  MUL,
};

const char *commands[8] = {"out", "out_char", "goto", "copy",
                           "set", "add",      "sub",  "mul"};

typedef struct {
  enum TokenType Type;
  char *TokenLiteral;
} Token;

typedef struct {
  char *buffer;
  int buffer_size;
  char current_char;
  int current_position;
  int read_position;
} Lexer;

Lexer *new_lexer(char *buffer, int buffer_size) {
  Lexer *l = (Lexer *)malloc(sizeof(Lexer));
  if (l == NULL) {
    perror("Error allocating memory for Lexer");
    return NULL;
  }
  l->buffer = buffer;
  l->buffer_size = buffer_size;
  l->current_position = 0;
  l->read_position = l->current_position + 1;
  l->current_char = l->buffer[l->current_position];
  return l;
}

int is_alpha(char c) {
  return (('a' <= c && c <= 'z') || (('A' <= c && c <= 'Z') || (c == '_')));
}

char peek_char(Lexer *l) { return l->buffer[l->read_position]; }

void read_char(Lexer *l) {
  if (l->current_position >= l->buffer_size) {
    l->current_char = '\0';
  }
  l->current_position = l->read_position;
  l->read_position++;
  l->current_char = l->buffer[l->current_position];
}

void skip_white_space(Lexer *l) {
  for (; l->current_char == ' ' || l->current_char == '\t' ||
         l->current_char == '\n' || l->current_char == '\r';) {
    read_char(l);
  }
}

Token *tokenize(Lexer *l){
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("%s", HELP_MESSAGE);
    return EXIT_SUCCESS;
  }
  char *file_name = argv[1];
  FILE *file;
  file = fopen(file_name, "r");
  if (file == NULL) {
    perror("Error reading file");
    fclose(file);
    return EXIT_FAILURE;
  }
  char *line = NULL;
  size_t line_length = 0;
  while (getline(&line, &line_length, file) != -1) {
    Lexer *lexer = new_lexer(line, line_length);
  }
  free(line);
  fclose(file);
  return EXIT_SUCCESS;
}

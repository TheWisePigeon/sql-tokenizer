#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *HELP_MESSAGE = "Usage: lang <file.lang>";

enum TokenType {
  OUTPUT,
  OUTPUT_UTF8,
  GOTO,
  COPY,
  SET,
  ADD,
  SUB,
  MUL,
  DOLLAR,
  INTEGER,
  HASH,
};

typedef struct {
  char *Value;
  enum TokenType Kind;
} Token;

typedef struct {
  uint8_t *buffer;
  uint8_t buffer_size;
  uint8_t current_position;
  uint8_t current_char;
  int8_t read_position;
} Lexer;

Lexer *new_lexer(uint8_t *buffer, uint8_t buffer_size) {
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

void display_lexer_info(Lexer *l) {
  printf("\n\nLexer info\n\n");
  printf("Buffer %s\n", l->buffer);
  printf("Buffer size %d\n", l->buffer_size);
  printf("Current position %d\n", l->current_position);
  printf("Current char %c\n", l->current_char);
  printf("Read position %d\n", l->read_position);
  printf("\n\nLexer info\n\n");
}

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

void skip_comment_line(Lexer *l) {
  for (; l->current_char != '\n';) {
    read_char(l);
  }
}

uint8_t *read_symbol(Lexer *l) { return NULL; }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("%s", HELP_MESSAGE);
    return EXIT_SUCCESS;
  }
  char *file_name = argv[1];
  FILE *fp;
  fp = fopen(file_name, "r");
  if (fp == NULL) {
    perror("Error reading file");
    fclose(fp);
    return EXIT_FAILURE;
  }
  size_t file_size;
  fseek(fp, 1, SEEK_END);
  file_size = ftell(fp);
  rewind(fp);
  uint8_t *buffer = malloc(file_size + 1);
  if (buffer == NULL) {
    perror("Error allocating memory");
    return EXIT_FAILURE;
  }
  fread(buffer, 1, file_size, fp);
  fclose(fp);
  Lexer *l = new_lexer(buffer, file_size);
  while (l->current_char != '\0') {
    skip_white_space(l);
    switch (l->current_char) {
    case '#':
      skip_comment_line(l);
      continue;
    case ';':
    default:
      if (isalpha(l->current_char)) {
      }
    }
    read_char(l);
  }
  free(buffer);
  free(l);
  return EXIT_SUCCESS;
}

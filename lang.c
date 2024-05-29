#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *HELP_MESSAGE = "Usage: lang <file.lang>";

const int MAX_COMMAND_LENGTH = 10;

enum Command {
  OUT,
  OUT_CHAR,
  GOTO,
  COPY,
  SET,
  ADD,
  SUB,
  MUL,
  INVALID,
};

const char *commands[8] = {"out", "out_char", "goto", "copy",
                           "set", "add",      "sub",  "mul"};

enum ArgumentLiteral {
  INTEGER,
  CELL_REFERENCE,
  DOT,
  THREE_DOTS,
  CLIPBOARD,
};

typedef struct {
  enum ArgumentLiteral Literal;
  void *value;
} Argument;

typedef struct {
  enum Command cmd;
  Argument args[3];
  uint8_t arg_count;
} Instruction;

/* typedef struct { */
/*   char *Value; */
/*   enum TokenType Kind; */
/* } Token; */

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

int8_t is_alpha(uint8_t c) {
  return (('a' <= c && c <= 'z') || (('A' <= c && c <= 'Z') || (c == '_')));
}

uint8_t peek_char(Lexer *l) { return l->buffer[l->read_position]; }

uint8_t *read_command_literal(Lexer *l) {
  uint8_t *cmd = (uint8_t *)malloc(MAX_COMMAND_LENGTH);
  int8_t index = 0;
  for (; is_alpha(l->current_char);) {
    cmd[index] = l->current_char;
    index++;
    read_char(l);
  }
  cmd[index] = '\0';
  return cmd;
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

enum Command parse_command(uint8_t *cmd_literal) {
  enum Command cmd = INVALID;
  for (int i = 0; i < 8; i++) {
    if (strcmp((char *)cmd_literal, commands[i]) == 0) {
      cmd = i;
    }
  }
  return cmd;
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
    if (l->current_char == '#') {
      skip_comment_line(l);
      continue;
    }
    if (is_alpha(l->current_char)) {
      uint8_t *cmd_literal = read_command_literal(l);
      enum Command cmd = parse_command(cmd_literal);
      printf("%d here\n", cmd);
      switch (cmd) {
      case OUT:
        printf("out case");
        break;
      case OUT_CHAR:
        printf("out case");
        break;
      case GOTO:
        printf("out case");
        break;
      case COPY:
        printf("out case");
        break;
      case SET:
        printf("out case");
        break;
      case ADD:
        printf("out case");
        break;
      case SUB:
        printf("out case");
        break;
      case MUL:
        printf("out case");
        break;
      case INVALID:
        printf("%s is not a valid command!", cmd_literal);
        break;
      default:
        printf("Something went really really wrong");
        break;
      }
      printf("got here??");
      read_char(l);
      continue;
    }
  }
  free(buffer);
  free(l);
  return EXIT_SUCCESS;
}

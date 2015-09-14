/*
 * Adapted from
 * http://stackoverflow.com/questions/9907160/how-to-convert-enum-names-to-string-in-c
 */

#ifndef _TOKEN_H
#define _TOKEN_H

#define FOREACH_TOKEN(MACRO) \
  MACRO(SEMICOLON), \
  MACRO(WHITESPACE), \
  MACRO(COMMENT), \
  MACRO(IDENTIFIER), \
  MACRO(INTEGER), \
  MACRO(STRING), \
  MACRO(CHAR), \
  MACRO(BOOLEAN), \
  MACRO(INTEGER_LITERAL), \
  MACRO(STRING_LITERAL), \
  MACRO(CHAR_LITERAL), \
  MACRO(ARRAY), \
  MACRO(VOID), \
  MACRO(TRUE), \
  MACRO(FALSE), \
  MACRO(IF), \
  MACRO(ELSE), \
  MACRO(WHILE), \
  MACRO(FOR), \
  MACRO(FUNCTION), \
  MACRO(RETURN), \
  MACRO(PRINT), \
  MACRO(DECLARATION), \
  MACRO(ASSIGNMENT), \
  MACRO(LEFT_BRACE), \
  MACRO(RIGHT_BRACE), \
  MACRO(LEFT_PAREN), \
  MACRO(RIGHT_PAREN), \
  MACRO(LEFT_BRACKET), \
  MACRO(RIGHT_BRACKET), \
  MACRO(PLUSPLUS), \
  MACRO(MINUSMINUS), \
  MACRO(PLUS), \
  MACRO(MINUS), \
  MACRO(MULTIPLICATION), \
  MACRO(DIVISION), \
  MACRO(MODULUS), \
  MACRO(LT), \
  MACRO(LE), \
  MACRO(GT), \
  MACRO(GE), \
  MACRO(EQ), \
  MACRO(NE), \
  MACRO(AND), \
  MACRO(OR), \
  MACRO(PARSE_ERROR)

#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING

typedef enum {
  FOREACH_TOKEN(GENERATE_ENUM)
} TOKEN;

static const char *TOKEN_STRING[] = {
  FOREACH_TOKEN(GENERATE_STRING)
};

#endif

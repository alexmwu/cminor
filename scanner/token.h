/*
 * Adapted from
 * http://stackoverflow.com/questions/9907160/how-to-convert-enum-names-to-string-in-c
 */

#ifndef _TOKEN_H
#define _TOKEN_H

#define FOREACH_TOKEN(MACRO) \
  MACRO(SEMICOLON), \
  MACRO(COMMA), \
  MACRO(WHITESPACE), \
  MACRO(CCOMMENT), \
  MACRO(CPPCOMMENT), \
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
  MACRO(COLON), \
  MACRO(EQUALS), \
  MACRO(LEFT_BRACE), \
  MACRO(RIGHT_BRACE), \
  MACRO(LEFT_PAREN), \
  MACRO(RIGHT_PAREN), \
  MACRO(LEFT_BRACKET), \
  MACRO(RIGHT_BRACKET), \
  MACRO(PLUSPLUS), \
  MACRO(MINUSMINUS), \
  MACRO(EXPONENTIATION), \
  MACRO(PLUS), \
  MACRO(MINUS), \
  MACRO(NOT), \
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
  MACRO(SCAN_ERROR)

#define GENERATE_ENUM(ENUM) ENUM
#define GENERATE_STRING(STRING) #STRING

typedef enum {
  BAD_TOKEN,  // otherwise SEMICOLON would start off as 0, which makes the token variable 0, breaking the loop
  FOREACH_TOKEN(GENERATE_ENUM)
} TOKEN;

static const char *TOKEN_STRING[] = {
  "BAD_TOKEN",  // need to match the index of the TOKEN enum
  FOREACH_TOKEN(GENERATE_STRING)
};

#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// scanner tools

char handleChar(char *c) {
  // length should only be 3 or 4
  int len = strlen(c);
  if(len == 3) {
    // if character is just a single backslash
    if(c[1] == '\\') {
      return (char) -1;
    }
    return c[1];  // c should look like ' + char + '
  }
  else if(len == 4) {   // c should look like ' + \ + char + '
    if(c[2] == '0') {
      return (char) 0;  // ASCII code for NULL
    }
    else if(c[2] == 'n') {
      return (char) 10;  // ASCII code for NULL
    }
    else {
      return c[2];
    }
  }
  else {
    fprintf(stderr, "Unknown/malformed CHARLIT scanned in: %s\n", c);
    exit(1);
  }
}

// need to free string
// type 0 is string literal; 1 is identifier
char *handleString(char *s, int type) {
  int len = strlen(s);
  char *retStr = malloc(len * sizeof(char));
  char *ptr = s;
  int i = 0;
  while(*ptr) {
    if(*ptr == '"') {
      ptr++;
      continue;
    }
    else if(*ptr == '\\') {
      ptr++;  // move one ahead for actual value
      if(!ptr) {
        fprintf(stderr, "SCAN_ERROR: STRING_LITERAL terminated on '\\'\n");
      }
      else if(*ptr == 'n') {
        retStr[i++] = '\n';
      }
      else if(*ptr == '0') {
        retStr[i++] = '\0';
      }
      else {
        retStr[i++] = *ptr;
      }
      ptr++;
    }
    else {
      retStr[i++] = *ptr;
      ptr++;
    }
  }
  retStr[i] = '\0';
  if(strlen(retStr) > 255) {
    free(retStr);
    if(type == 0)
      fprintf(stderr, "SCAN_ERROR: STRING_LITERAL has exceeded the max size of 255\n");
    else if(type == 1)
      fprintf(stderr, "SCAN_ERROR: IDENTIFIER has exceeded the max size of 255\n");
    else
      fprintf(stderr, "Bad token type (should be string or ident)\n");
    exit(1);
  }
  return retStr;
}

long handleInt(char *i) {
  char *end;
  // 0 means allow C integer constant formats
  long ret = strtol(i, &end, 0);
  if(!ret && end == i) {
    fprintf(stderr, "Integer scan error");
    exit(1);
  }

  return ret;
}


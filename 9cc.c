#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// トークンの型定義
enum {
  TK_NUM = 256, // 整数トークン
  TK_EOF,       // EOF
};

// トークンの型
typedef struct {
  int ty;       // トークンの型
  int val;      // tyがTK_NUMの場合、その数値
  char *input;  // トークン文字列 (エラーメッセージ用)
} Token;

// トークナイズした結果のトークン列
Token tokens[100];

void tokenize(char *p) {
  int i=0;
  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      tokens[i].ty = *p;
      tokens[i].input = p;
      i++;
      p++;
      continue;
    }

    if (isdigit(*p)) {
      tokens[i].ty = TK_NUM;
      tokens[i].input = p;
      tokens[i].val = strtol(p, &p, 10);
      i++;
      continue;
    }

    fprintf(stderr, "トークナイズできません: %s\n",p);
    exit(1);
  }

  tokens[i].ty = TK_EOF;
  tokens[i].input = p;
}

void error(int i){
  fprintf(stderr, "予期しないトークンです: %s\n",tokens[i].input);
  exit(1);
}


int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "引数の個数が正しくありません\n");
    return 1;
  }

  tokenize(argv[1]);

  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  if (tokens[0].ty != TK_NUM) error(0);

  printf("  mov rax, %d\n", tokens[0].val);

  int i=1;

  while (tokens[i].ty != TK_EOF) {
    if (tokens[i].ty == '+') {
      i++;
      if (tokens[i].ty != TK_NUM) error(i);
      printf("  add rax, %d\n", tokens[i].val);
      i++;
      continue;
    }

    if (tokens[i].ty == '-') {
      i++;
      if (tokens[i].ty != TK_NUM) error(i);
      printf("  sub rax, %d\n", tokens[i].val);
      i++;
      continue;
    }
    error(i);
  }

  printf("  ret\n");
  return 0;
}

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXQ 100

int yylex(void);
void yyerror(const char *s);

struct Quadruple {
    char op[20];
    char arg1[20];
    char arg2[20];
    char result[20];
} quad[MAXQ];

int qcount = 0;
int tcount = 0;
int lcount = 0;

void addquad(const char *op, const char *a1,
             const char *a2, const char *res)
{
    if (qcount >= MAXQ) {
        printf("Quadruple table overflow!\n");
        exit(1);
    }

    snprintf(quad[qcount].op, 20, "%s", op);
    snprintf(quad[qcount].arg1, 20, "%s", a1);
    snprintf(quad[qcount].arg2, 20, "%s", a2);
    snprintf(quad[qcount].result, 20, "%s", res);

    qcount++;
}

char *newtemp(void)
{
    char *s = malloc(20);
    if (s == NULL) exit(1);

    sprintf(s, "t%d", ++tcount);
    return s;
}

char *newlabel(void)
{
    char *s = malloc(20);
    if (s == NULL) exit(1);

    sprintf(s, "L%d", ++lcount);
    return s;
}

void printquads(void)
{
    int i;

    printf("\n%-24s %-12s %-8s %-8s %-8s\n",
           "TAC", "Op", "Op1", "Op2", "Result");

    printf("------------------------------------------------------------------------\n");

    for (i = 0; i < qcount; i++) {
        char tac[100];

        if (strcmp(quad[i].op, "label") == 0) {
            snprintf(tac, sizeof(tac), "%s:",
                     quad[i].result);
        }
        else if (strcmp(quad[i].op, "if_false") == 0) {
            snprintf(tac, sizeof(tac), "if %s goto %s",
                     quad[i].arg1, quad[i].result);
        }
        else if (strcmp(quad[i].op, "if_true") == 0) {
            snprintf(tac, sizeof(tac), "if %s goto %s",
                     quad[i].arg1, quad[i].result);
        }
        else if (strcmp(quad[i].op, "=") == 0) {
            snprintf(tac, sizeof(tac), "%s = %s",
                     quad[i].result, quad[i].arg1);
        }
        else if (strcmp(quad[i].op, "[]") == 0) {
            snprintf(tac, sizeof(tac), "%s = %s[%s]",
                     quad[i].result,
                     quad[i].arg1,
                     quad[i].arg2);
        }
        else if (strcmp(quad[i].op, "[]=") == 0) {
            snprintf(tac, sizeof(tac), "%s[%s] = %s",
                     quad[i].arg1,
                     quad[i].arg2,
                     quad[i].result);
        }
        else {
            snprintf(tac, sizeof(tac), "%s = %s %s %s",
                     quad[i].result,
                     quad[i].arg1,
                     quad[i].op,
                     quad[i].arg2);
        }

        printf("%-24s %-12s %-8s %-8s %-8s\n",
               tac,
               quad[i].op,
               quad[i].arg1,
               quad[i].arg2,
               quad[i].result);
    }

    printf("\n");
}
%}

%union {
    char *str;
}

%token <str> ID NUM RELOP
%token IF

%type <str> expr cond lvalue

%left '+' '-'
%left '*' '/'
%left RELOP

%%

program
    : stmts
      {
          printquads();
      }
    ;

stmts
    : stmts stmt
    | /* empty */
    ;

stmt
    : assign
    | if_stmt
    ;

assign
    : lvalue '=' expr ';'
      {
          if ($1[0] == '@') {
              char *p = strchr($1, ':');
              *p = '\0';
              addquad("[]=", $1 + 1, p + 1, $3);
          }
          else {
              addquad("=", $3, "-", $1);
          }
      }
    ;

lvalue
    : ID
      {
          $$ = $1;
      }
    | ID '[' expr ']'
      {
          char *index = newtemp();
          char *s = malloc(strlen($1) + strlen(index) + 3);

          if (s == NULL) exit(1);

          addquad("=", $3, "-", index);
          sprintf(s, "@%s:%s", $1, index);
          $$ = s;
      }
    ;

if_stmt
    : IF '(' cond ')'
      {
          $<str>$ = newlabel();
          addquad("if_false", $3, "-", $<str>$);
      }
      '{' stmts '}'
      {
          addquad("label", "-", "-", $<str>5);
      }
    ;

cond
    : expr RELOP expr
      {
          $$ = newtemp();
          addquad($2, $1, $3, $$);
      }
    ;

expr
    : expr '+' expr
      {
          $$ = newtemp();
          addquad("+", $1, $3, $$);
      }
    | expr '-' expr
      {
          $$ = newtemp();
          addquad("-", $1, $3, $$);
      }
    | expr '*' expr
      {
          $$ = newtemp();
          addquad("*", $1, $3, $$);
      }
    | expr '/' expr
      {
          $$ = newtemp();
          addquad("/", $1, $3, $$);
      }
    | '(' expr ')'
      {
          $$ = $2;
      }
    | ID '[' expr ']'
      {
          char *index = newtemp();

          addquad("=", $3, "-", index);

          $$ = newtemp();
          addquad("[]", $1, index, $$);
      }
    | ID
      {
          $$ = $1;
      }
    | NUM
      {
          $$ = $1;
      }
    ;

%%

void yyerror(const char *s)
{
    fprintf(stderr, "Parse error: %s\n", s);
}

int main(void)
{
    printf("Enter the statement:\n");
    yyparse();
    return 0;
}
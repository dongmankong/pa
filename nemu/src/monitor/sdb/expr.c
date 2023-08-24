/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,
  TK_DECIMAL=10,
  EQ=11,
  NOTEQ=12,
  AND=13,
  RESGISTER=14,
  HEX=15,
  OR=16
  /* TODO: Add more token types */
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},           // minus
  {"\\*", '*'},         // multiply
  {"/", '/'},           // divide
  {"\\(", '('},          // left parenthesis
  {"\\)", ')'},          // right parenthesis

  {"\\=\\=)", EQ},          // equal
  {"\\!\\=", NOTEQ},          // !=
  {"\\&\\&", AND},          // &&
  {"\\|\\|", OR},          // ||
  // {"\\!", '!'},          // ||


  {"\\$[a-zA-Z]*[0-9]*", RESGISTER},
  {"0[xX][0-9a-fA-F]+", HEX}, 

  {"[0-9]*", TK_DECIMAL}, //nums
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      // printf("%d",i);
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */


        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case '+':
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case '-':
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case '*':
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case '/':
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case '(':
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case ')':
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case TK_DECIMAL:
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case EQ:
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case NOTEQ:
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case AND:
          case OR:
          // case '!':
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;
          case RESGISTER: 
          case HEX:
            tokens[nr_token].type=rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;  
          default: 
            printf("i = %d and No rules is com.\n", i);
            break;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

word_t eval(int p,int q,bool *success);
bool check_parentheses(int p,int q,bool *success);

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();
	for(int i=0;i<nr_token;++i){
		printf("%s",tokens[i].str);
	}
	printf("\n");
  return eval(0,nr_token-1,success);
  // return 0;
}

word_t eval(int p,int q,bool *success){
	// printf("%d %d\n",p,q);
  if (p > q) {
    *success = false;
    return 0;
  }
  else if (p == q) {
    if(tokens[p].type!=TK_DECIMAL){
      *success = false;
      return 0;
    }
    return strtoul(tokens[p].str, NULL, 10);
  }
  else if (check_parentheses(p, q,success) == true) {
    return eval(p + 1, q - 1,success);
  }
  else {
    int op=TK_NOTYPE ;
    int leftParentheses=0;
    int level=0;
    for(int i=p;i<=q;++i){
      if(tokens[i].type=='('){
        leftParentheses++;
		continue;
      }else if(tokens[i].type==')'){
        leftParentheses--;
		continue;
      }
      if(leftParentheses>0){
        continue;
      }
      if(level<=1 && (tokens[i].type == '*' || tokens[i].type == '/')){
        level=1;
        op=i;
      }
      if(level<=2 && (tokens[i].type == '+' || tokens[i].type=='-')){
        level=2;
        op=i;
      }
      if(level<=3 && (tokens[i].type==EQ || tokens[i].type==NOTEQ)){
        level=3;
        op=i;
      }
      if(level<=4 && tokens[i].type==AND){
        level=4;
        op=i;
      }
      if(level<=5 && tokens[i].type==OR){
        level=5;
        op=i;
      }
      // if(tokens[i].type=='+' || tokens[i].type=='-'){
      //   op=i;
      // }else if(tokens[i].type=='*' || tokens[i].type=='/'){
      //   if(op==TK_NOTYPE || tokens[op].type=='*' || tokens[op].type=='/'){
      //     op=i;
      //   }
      // }
    }
	if(op==TK_NOTYPE){
		*success=false;
		return 0;
	}
    word_t val1 = eval(p, op - 1,success);
    word_t val2 = eval(op + 1, q,success);
    switch (tokens[op].type) {
      case '+': return val1 + val2;
      case '-': return val1-val2;
      case '*': return val1*val2;
      case '/': 
        if(val2==0){
          *success=false;
          printf("error divide by zero ");
          return 0;
        }
        return val1/val2;
      case EQ: return val1==val2;
      case NOTEQ: return val1!=val2;
      case AND: return val1 && val2;
      case OR: return val1 || val2;
      default:
        printf("No Op type.");
        assert(0);
    }
  }
}
bool check_parentheses(int p,int q,bool *success){
	int l=p;
	int r=q;
	// if(tokens[l].type!='(' || tokens[r].type==')'){
	// 	return false;
	// }
	while(l<=r){
		if(tokens[l].type=='('){
			if(tokens[r].type==')'){
				l++;
				r--;
				continue;
			}else{
				r--;
				if(l>=r){
					*success=false;
					return false;
				}
			}
		}else if(tokens[l].type==')'){
			return false;
		}else{
			l++;
		}
	}
	if(tokens[p].type!='(' || tokens[q].type!=')'){
		return false;
	}
	return true;
} 
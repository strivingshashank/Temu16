/* Under-development */

#include "t_io.h"
#include "t_utils.h"
#include "t_types.h"
#include "t_memory.h"
#include "t_structs.h"

#define MAX_EXPRESSION_SIZE (32 + 1)

typedef enum {
  TOKEN_SDEC,
  TOKEN_OPERATOR,
  TOKEN_PAREN_OPEN, /* '(' */
  TOKEN_PAREN_CLOSE, /* ')' */
  TOKEN_NULL
} token_type_t;

typedef struct {
  token_type_t token;
  sbit16_t value;
} token_t;

void greeting(void);
void program_main(void);
void tcalc(void); 
token_t * tokenize_infix(bit8_t *infix_str);
bool_t is_operator(bit8_t value);
bool_t is_operand(bit8_t value);
token_t *infix_to_postfix(token_t *infix_tok);
sbit16_t evaluate_postfix(token_t *postfix_tok);
size_t get_operator_precedence(bit8_t operator);

/* For debugging */
// void dump_stack_s16(stack_t *stack, bit8_t *message) {
//   stack_t *tmp_stack = NULL_PTR;

//   tmp_stack = create_stack(sizeof(sbit16_t));
  
//   write_str(message);
//   write_str(": ");

//   if (stack_get_size(stack) < 0) {
//     write_str("stack is empty.\n");
//     return;
//   }

//   while (0 < stack_get_size(stack)) {
//     write_sdec(*(sbit16_t *)(stack_peek(stack)));
//     write_char(' ');
//     stack_push(tmp_stack, stack_peek(stack));
//     stack_pop(stack);
//   }

//   while (0 < stack_get_size(tmp_stack)) {
//     stack_push(stack,stack_peek(tmp_stack));
//     stack_pop(tmp_stack);
//   }

//   write_char('\n');
// }

void dump_tokens(token_t *tok, bit8_t *message) {
  size_t idx = 0;

  write_str(message);
  write_str(": ");

  while (tok[idx].token != TOKEN_NULL) {
    if (tok[idx].token == TOKEN_SDEC) {
      write_sdec(tok[idx].value);
      write_char(' ');
      idx++;
      continue;
    }
    
    write_char(tok[idx].value);
    write_char(' ');
    
    idx++;
  }

  write_char('\n');
}

void greeting(void) {
  write_str("----- TClock -----\n");
  write_str("A super-simple clock\n");
}

void tcalc(void) {
  bit8_t *infix_expression = NULL_PTR;
  token_t *infix_tok = NULL_PTR;
  token_t *postfix_tok = NULL_PTR;
  size_t tok_size = 0;
  sbit16_t expression_result = 0;

  infix_expression = heap_alloc(MAX_EXPRESSION_SIZE);
  
  if (infix_expression == NULL_PTR) {
    return;
  }
  
  write_str("Expression: ");
  read_str(infix_expression, MAX_EXPRESSION_SIZE);
  write_char('\n');

  infix_tok = tokenize_infix(infix_expression);

  if (infix_tok == NULL_PTR) {
    write_str("Tokenization failed.\n");
    return;
  }

  postfix_tok = infix_to_postfix(infix_tok);

  if (postfix_tok == NULL_PTR) {
    write_str("Shunting yard failed.\n");
    return;
  }

  expression_result = evaluate_postfix(postfix_tok);

  write_str("Result = ");
  write_sdec(expression_result);
  write_char('\n');

  heap_free(infix_expression, MAX_EXPRESSION_SIZE);

  {
    size_t tok_count = 0;
    heap_free(infix_tok, sizeof(token_t) * MAX_EXPRESSION_SIZE); 

    tok_count = 0; 
    
    while (postfix_tok[tok_count].token != TOKEN_NULL){
      tok_count++;
    } 
    
    heap_free(postfix_tok, sizeof(token_t) * (tok_count + 1));
  }
}

token_t *tokenize_infix(bit8_t *infix_str) {
  token_t *infix_tok = NULL_PTR;
  size_t str_idx = 0;
  size_t tok_idx = 0;
  bit8_t infix_str_size = 0;

  if (infix_str == NULL_PTR) {
    return NULL_PTR;
  }

  infix_tok = (token_t *)(heap_alloc(sizeof(token_t) * MAX_EXPRESSION_SIZE));

  if (infix_tok == NULL_PTR) {
    return NULL_PTR;
  }

  infix_str_size = str_get_size(infix_str);
  
  while (str_idx < infix_str_size && tok_idx < MAX_EXPRESSION_SIZE) {
    bit8_t current_char = infix_str[str_idx];

    if (current_char == ' ') {
      str_idx++;
      continue;
    } 
    
    if (is_operand(current_char) == TRUE) {
      sbit16_t sdec_value = 0;
      
      while (str_idx < infix_str_size) {
        bit8_t sdec_char = infix_str[str_idx];

        if (! is_operand(sdec_char)) {
          break;
        }

        sdec_value = (sdec_value * 10) + (ascii_to_dec(sdec_char));
        str_idx++;
      }

      infix_tok[tok_idx].token = TOKEN_SDEC;
      infix_tok[tok_idx].value = sdec_value;
      tok_idx++;
      continue;
    } 
    
    if (is_operator(current_char) == TRUE) {
      infix_tok[tok_idx].token = TOKEN_OPERATOR;
      infix_tok[tok_idx].value = current_char;
      str_idx++;
      tok_idx++;
      continue;
    } 
    
    if (current_char == '(') {
      infix_tok[tok_idx].token = TOKEN_PAREN_OPEN;
      infix_tok[tok_idx].value = current_char;
      str_idx++;
      tok_idx++;
      continue;
    } 
    
    if (current_char == ')') {
      infix_tok[tok_idx].token = TOKEN_PAREN_CLOSE;
      infix_tok[tok_idx].value = current_char;
      str_idx++;
      tok_idx++;
      continue;
    }

    write_str("Invalid char.\n");
    heap_free(infix_tok, sizeof(token_t) * MAX_EXPRESSION_SIZE);
    return NULL_PTR;
  }
  
  infix_tok[tok_idx].token = TOKEN_NULL;
  infix_tok[tok_idx].value = 0;

  return infix_tok;
}

bool_t is_operator(bit8_t value) {
  switch (value) {
    case '*':
    case '/':
    case '+':
    case '-':
      return TRUE;
  }

  return FALSE;
} 

bool_t is_operand(bit8_t value) {
  return ('0' <= value && value <= '9');
}

token_t *infix_to_postfix(token_t *infix_tok) {
  stack_t *operator_stack = NULL_PTR;
  token_t *postfix_tok = NULL_PTR;
  size_t infix_idx = 0;
  size_t postfix_idx = 0;
  size_t tok_count = 0;

  if (infix_tok == NULL_PTR) {
    return NULL_PTR;
  }

  while (infix_tok[tok_count].token != TOKEN_NULL) {
    tok_count++;
  }

  operator_stack = create_stack(sizeof(token_t));
  postfix_tok = heap_alloc(sizeof(token_t) * (tok_count + 1));

  if (operator_stack == NULL_PTR || postfix_tok == NULL_PTR) {
    return NULL_PTR;
  }

  while (infix_tok[infix_idx].token != TOKEN_NULL) {
    token_type_t current_tok_type = infix_tok[infix_idx].token;

    if (current_tok_type == TOKEN_SDEC) {
      memory_copy(&infix_tok[infix_idx], &postfix_tok[postfix_idx], sizeof(token_t));
      postfix_idx++;
      infix_idx++;
      continue;
    }

    if (current_tok_type == TOKEN_PAREN_OPEN) {
      stack_push(operator_stack, &infix_tok[infix_idx]);
      infix_idx++;
      continue;
    }

    if (current_tok_type == TOKEN_PAREN_CLOSE) {
      while (0 < stack_get_size(operator_stack)) {
        while (((token_t *)(stack_peek(operator_stack)))->token != TOKEN_PAREN_OPEN) {
          memory_copy((token_t *)(stack_peek(operator_stack)), &postfix_tok[postfix_idx], sizeof(token_t));
          stack_pop(operator_stack);
          postfix_idx++;
        }
      }
      
      if (stack_get_size(operator_stack) == 0) {
        heap_free(postfix_tok, sizeof(token_t) * (tok_count + 1));
        delete_stack(operator_stack);
        return NULL_PTR;
      }
      
      stack_pop(operator_stack); /* Pop remaining '('. */
      infix_idx++;
      continue;
    }

    if (current_tok_type == TOKEN_OPERATOR) {
      bit16_t current_operator = infix_tok[infix_idx].value;

      while (0 < stack_get_size(operator_stack)) {
        token_t *top_operator = (token_t *)(stack_peek(operator_stack));

        if (top_operator->token == TOKEN_PAREN_OPEN) {
          break;
        }

        if (get_operator_precedence(top_operator->value) < get_operator_precedence(current_operator)) {
          break;
        }

        memory_copy(top_operator, &postfix_tok[postfix_idx], sizeof(token_t));
        stack_pop(operator_stack);
        postfix_idx++;
      }

      stack_push(operator_stack, &infix_tok[infix_idx]);
      infix_idx++;
      continue;
    }

    infix_idx++;
  }

  while (0 < stack_get_size(operator_stack)) {
    memory_copy((token_t *)(stack_peek(operator_stack)), &postfix_tok[postfix_idx], sizeof(token_t));
    stack_pop(operator_stack);
    postfix_idx++;
  }

  postfix_tok[postfix_idx].token = TOKEN_NULL;
  postfix_tok[postfix_idx].value = 0;

  delete_stack(operator_stack);
  return postfix_tok;
}

sbit16_t evaluate_postfix(token_t *postfix_tok) {
  stack_t *evaluation_stack = NULL_PTR;
  static sbit16_t result = 0;
  size_t idx = 0;

  if (postfix_tok == NULL_PTR) {
    return 0;
  }

  evaluation_stack = create_stack(sizeof(sbit16_t));

  if (evaluation_stack == NULL_PTR) {
    return 0;
  }
  
  while (postfix_tok[idx].token != TOKEN_NULL) {
    token_type_t tok_type = postfix_tok[idx].token;

    if (tok_type == TOKEN_SDEC) {
      stack_push(evaluation_stack, &postfix_tok[idx].value);
      idx++;
      continue;
    }

    if (tok_type == TOKEN_OPERATOR) {
      sbit16_t left_operand = 0;
      sbit16_t right_operand = 0;
      bit8_t operator = (bit8_t)(postfix_tok[idx].value);

      right_operand = *((sbit16_t *)(stack_peek(evaluation_stack)));
      stack_pop(evaluation_stack);

      left_operand = *((sbit16_t *)(stack_peek(evaluation_stack)));
      stack_pop(evaluation_stack);
      
      write_sdec(left_operand);
      write_char(' ');
      write_char(operator);
      write_char(' ');
      write_sdec(right_operand);
      write_char('\n');

      switch (operator) {
        case '+': result = left_operand + right_operand; break;
        case '-': result = left_operand - right_operand; break;
        case '*': result = left_operand * right_operand; break;
        case '/': 
          if (right_operand == 0) {
            write_str("Invalid operation: Division by ZERO (0)\n");
            result = 0;
          } 
          else {
            result = left_operand / right_operand;
          }
          break;
        default:
          write_str("Unknown operator: ");
          write_char(operator);
          write_char('\n');
          result = 0;
          break;
      }

      stack_push(evaluation_stack, &result);
      idx++;
      continue;
    }

    write_str("WTF\n");
    idx++;
  }

  {
    sbit16_t final_result = 0;
    
    if (stack_get_size(evaluation_stack) == 0) {
      /* no result */
      delete_stack(evaluation_stack);
      return 0;
    }
    
    final_result = *((sbit16_t *)(stack_peek(evaluation_stack)));
    delete_stack(evaluation_stack);
    return final_result;
  }
}

size_t get_operator_precedence(bit8_t operator) {
  switch (operator) {
    case '*':
    case '/':
      return 2;
    case '+':
    case '-':
      return 1;
    default:
      return 0;
  }
}

void _program_init(void) {
  heap_init();
  program_main();
}

void program_main(void) {
  tcalc(); 

  wait_for_char(CHARACTER_ESCAPE);  
  write_char('\n');
  return;
}


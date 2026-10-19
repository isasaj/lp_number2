#ifndef SOLVER_H
#define SOLVER_H


typedef struct {
    int** operands;
    int* op_lengths;
    char* operators;
    int num_operands;
    int capacity_operands;
    int* result;
    int result_length;
    int is_leading[26];
    int letter_values[26];
    int used_digits[10];
    int unique_letters[10];
    int num_letters;
} str_pars;

int parse(char* string, str_pars* str_parsing, int len);
void solve_base(str_pars* p);
void free_struct(str_pars* p);

#endif
#define _CRT_SECURE_NO_WARNINGS
#include "solver.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


void sort_unique_letters(str_pars* p) {
    for (int i = 0; i < p->num_letters - 1; i++) {
        for (int j = 0; j < p->num_letters - i - 1; j++) {
            int letter_a = p->unique_letters[j];
            int letter_b = p->unique_letters[j + 1];
            int power_a = 100;
            int power_b = 100;


            for (int k = 0; k < p->num_operands; k++) {
                for (int m = 0; m < p->op_lengths[k]; m++) {
                    if (p->operands[k][m] == letter_a) {
                        int pwr = p->op_lengths[k] - 1 - m; 
                        if (pwr < power_a) power_a = pwr;
                    }
                }
            }
            for (int m = 0; m < p->result_length; m++) {
                if (p->result[m] == letter_a) {
                    int pwr = p->result_length - 1 - m;
                    if (pwr < power_a) power_a = pwr;
                }
            }


            for (int k = 0; k < p->num_operands; k++) {
                for (int m = 0; m < p->op_lengths[k]; m++) {
                    if (p->operands[k][m] == letter_b) {
                        int pwr = p->op_lengths[k] - 1 - m;

                        if (pwr < power_b) power_b = pwr;
                    }
                }
            }
            for (int m = 0; m < p->result_length; m++) {
                if (p->result[m] == letter_b) {
                    int pwr = p->result_length - 1 - m;
                    if (pwr < power_b) power_b = pwr;

                }
            }
            if (power_a > power_b) {
                int temp = p->unique_letters[j];
                p->unique_letters[j] = p->unique_letters[j + 1];
                p->unique_letters[j + 1] = temp;
            }
        }
    }
}


void add_word(str_pars* struct_el, char* temp, int len) {
    if (struct_el->num_operands >= struct_el->capacity_operands) {
        struct_el->capacity_operands *= 2;
        struct_el->operands = (int**)realloc(struct_el->operands, struct_el->capacity_operands * sizeof(int*));
        struct_el->op_lengths = (int*)realloc(struct_el->op_lengths, struct_el->capacity_operands * sizeof(int));
    }
    struct_el->operands[struct_el->num_operands] = (int*)malloc(len * sizeof(int));
    for (size_t i = 0; i < len; i++) {
        struct_el->operands[struct_el->num_operands][i] = temp[i] - 'A';
    }
    struct_el->op_lengths[struct_el->num_operands] = len;

    
    struct_el->is_leading[temp[0] - 'A'] = 1;

    
    for (size_t i = 0; i < len; i++) {
        int current_letter = temp[i] - 'A';
        int is_found = 0;
        for (int j = 0; j < struct_el->num_letters; j++) {
            if (struct_el->unique_letters[j] == current_letter) {
                is_found = 1;
                break;
            }
        }
        if (!is_found) {
            struct_el->unique_letters[struct_el->num_letters] = current_letter;
            struct_el->num_letters++;
        }
    }
    struct_el->num_operands++;
}

int parse(char* string, str_pars* str_parsing, int len) {
    memset(str_parsing, 0, sizeof(str_pars));
    str_parsing->capacity_operands = 4;
    str_parsing->operands = (int**)malloc(str_parsing->capacity_operands * sizeof(int*));
    str_parsing->op_lengths = (int*)malloc(str_parsing->capacity_operands * sizeof(int));
    str_parsing->result = NULL;
    str_parsing->result_length = 0;
    for (int i = 0; i < 26; i++) {
        str_parsing->letter_values[i] = -1;
    }
    str_parsing->num_letters = 0;

    char temp[50];
    int temp_len = 0;
    int is_right_side = 0;

    for (size_t i = 0; i < len; i++) {
        char c = string[i];
        if (isalpha(c)) {
            temp[temp_len++] = c;
        }
        
        else if (c == '+') {
            if (temp_len > 0) {
                add_word(str_parsing, temp, temp_len);
                temp_len = 0;
            }
        }
        else if (c == '=') {
            if (temp_len > 0) {
                add_word(str_parsing, temp, temp_len);
                temp_len = 0;
            }
            is_right_side = 1;
        }
    }

    if (is_right_side && temp_len > 0) {
        str_parsing->result = (int*)malloc(temp_len * sizeof(int));
        for (int i = 0; i < temp_len; i++) {
            str_parsing->result[i] = temp[i] - 'A';
        }
        str_parsing->result_length = temp_len;
        str_parsing->is_leading[temp[0] - 'A'] = 1;

        for (int i = 0; i < temp_len; i++) {
            int current_letter = temp[i] - 'A';
            int is_found = 0;
            for (int j = 0; j < str_parsing->num_letters; j++) {
                if (str_parsing->unique_letters[j] == current_letter) {
                    is_found = 1; break;
                }
            }
            if (!is_found) {
                str_parsing->unique_letters[str_parsing->num_letters] = current_letter;
                str_parsing->num_letters++;
            }
        }
    }
    sort_unique_letters(str_parsing);
    return 1;
}





int check(str_pars* p) {
    long long left_sum = 0;
    long long current_num = 0;
    for (int i = 0; i < p->num_operands; i++) {
        current_num = 0;
        for (int j = 0; j < p->op_lengths[i]; j++) {
            current_num = current_num * 10 + p->letter_values[p->operands[i][j]];
        }
        left_sum += current_num;
    }

    long long right_sum = 0;
    for (int i = 0; i < p->result_length; i++) {
        right_sum = right_sum * 10 + p->letter_values[p->result[i]];
    }

    return left_sum == right_sum;
}


int check_columns(str_pars* p) {
    int carry = 0;
    int max_len = p->result_length;

    for (int col = 0; col < max_len; col++) {
        int sum = carry;
        int all_known = 1;

        for (int i = 0; i < p->num_operands; i++) {
 
            if (p->op_lengths[i] > col) {
                int letter = p->operands[i][p->op_lengths[i] - 1 - col];
                int val = p->letter_values[letter];
                if (val == -1) {
                    all_known = 0;
                    break;
                }
                sum += val;
            }
        }

        
        if (p->result_length > col) {
            int letter = p->result[p->result_length - 1 - col];
            int val = p->letter_values[letter];
            if (val == -1) {
                all_known = 0;
            }

            
            if (all_known) {
                if (sum % 10 != val) {
                    return 0;
                }
            }
        }
        else {
            if (all_known && sum % 10 != 0) {
                return 0;
            }
        }

    
        if (!all_known) {
            break;
        }

        carry = sum / 10;
    }
    return 1;
}



int recursive(str_pars* p, int letter_index) {
    if (letter_index == p->num_letters) {
        return 1;
    }

    int current_letter = p->unique_letters[letter_index];

    for (int digit = 0; digit <= 9; digit++) {
        if (p->used_digits[digit] == 1) {
            continue;
        }
        if (digit == 0 && p->is_leading[current_letter] == 1) {
            continue;
        }
        p->letter_values[current_letter] = digit;
        p->used_digits[digit] = 1;

        
        if (check_columns(p)) {
            if (recursive(p, letter_index + 1)) {
                return 1;
            }
        }

        p->letter_values[current_letter] = -1;
        p->used_digits[digit] = 0;
    }
    return 0;
}

void solve_base(str_pars* p) {
    if (recursive(p, 0)) {
        for (int i = 0; i < p->num_operands; i++) {
            for (int j = 0; j < p->op_lengths[i]; j++) {
                printf("%d", p->letter_values[p->operands[i][j]]);
            }
            if (i < p->num_operands - 1) {
                printf(" + ");
            }
        }
        printf(" = ");
        for (int i = 0; i < p->result_length; i++) {
            printf("%d", p->letter_values[p->result[i]]);
        }
        printf("\n");
    }
    else {
        printf("Решения нет\n");
    }
}

void free_struct(str_pars* p) {
    for (int i = 0; i < p->num_operands; i++) {
        free(p->operands[i]);
    }
    free(p->operands);
    free(p->op_lengths);
    if (p->result != NULL) {
        free(p->result);
    }
}
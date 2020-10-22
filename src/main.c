#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUMBER_COUNT 100

extern void sort(long long *numbers, int numbers_amount);

int parse_params(int argc, char **argv, long long *to, long long *from, bool *get_to, bool *get_from) {
    if (argc <= 1) return -1;
    if (argc > 3) return -2;
    char pref_from[] = "--from=";
    char pref_to[] = "--to=";
    size_t pref_from_length = strlen(pref_from);
    size_t pref_to_length = strlen(pref_to);
    for (int i = 1; i < argc; i++) {
        char *pointer_begin = argv[i];
        char *pointer_end = argv[i] + strlen(argv[i]);
        if (strncmp(argv[i], pref_from, pref_from_length) == 0) {
            if (*get_from) return -3;
            *get_from = true;
            *from = strtol(pointer_begin + pref_from_length, &pointer_end, 10);
        }
        if (strncmp(argv[i], pref_to, pref_to_length) == 0) {
            if (*get_to) return -3;
            *get_to = true;
            *to = strtol(pointer_begin + pref_to_length, &pointer_end, 10);
        }
    }
    if (get_to || get_from) return 1;
    return -4;
}

int get_numbers(long long *numbers, long long to, long long from, bool get_to, bool get_from) {
    int numbers_amount = 0;
    long long read_element;
    int read_check = scanf("%lld", &read_element);
    while (read_check != EOF) {
        if (read_element <= from && get_from) {
            fprintf(stdout, "%lld ", read_element);
        }
        if (read_element >= to && get_to) {
            fprintf(stderr, "%lld ", read_element);
        }
        if ((read_element > from || !get_from) && (read_element < to || !get_to)) {
            numbers[numbers_amount] = read_element;
            numbers_amount++;
        }
        read_check = scanf("%lld", &read_element);
    }
    return numbers_amount;
}


int main(int argc, char **argv) {
    long long to = 0;
    long long from = 0;
    bool get_to = false;
    bool get_from = false;
    int parse_check = parse_params(argc, argv, &to, &from, &get_to, &get_from);
    if (parse_check < 0) return parse_check;
    long long numbers[MAX_NUMBER_COUNT];
    size_t amount_of_numbers = get_numbers(numbers, to, from, get_to, get_from);
    long long unsorted_numbers[MAX_NUMBER_COUNT];
    for (size_t i = 0; i < amount_of_numbers; i++)
        unsorted_numbers[i] = numbers[i];
    sort(numbers, amount_of_numbers);
    int changed_position = 0;
    for (size_t i = 0; i < amount_of_numbers; i++) {
        if (numbers[i] != unsorted_numbers[i])
            changed_position++;
    }
    return changed_position;
}

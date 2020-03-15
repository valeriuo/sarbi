#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char*);


int* largestPermutation(int k, int arr_count, int* arr, int* result_count) {
    int s[arr_count];
    memset(s, 0, sizeof(s));
    int i, j;
    for (i=0, j=0; i<arr_count; i++) {
        int v = arr[i];
        while (s[v-1]) v = s[v-1]; 
        arr[i] = v;
        if (arr[i] != arr_count-i && j<k) {
            s[arr_count-i-1] = arr[i];
            arr[i] = arr_count-i;
            j++;
        }
    }

    *result_count = arr_count;
    return arr;
}

int main()
{
    //FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** nk = split_string(readline());

    char* n_endptr;
    char* n_str = nk[0];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }
    fprintf(stdout, "n=%d\n", n);
    char* k_endptr;
    char* k_str = nk[1];
    int k = strtol(k_str, &k_endptr, 10);
    fprintf(stdout, "'%s', k=%d\n", k_str, k);
    if (k_endptr == k_str || *k_endptr != '\0') { exit(EXIT_FAILURE); }

    char** arr_temp = split_string(readline());

    int* arr = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        char* arr_item_endptr;
        char* arr_item_str = *(arr_temp + i);
        int arr_item = strtol(arr_item_str, &arr_item_endptr, 10);

        if (arr_item_endptr == arr_item_str || *arr_item_endptr != '\0') { exit(EXIT_FAILURE); }

        *(arr + i) = arr_item;
    }

    int arr_count = n;

    int result_count=0;

    int* result = largestPermutation(k, arr_count, arr, &result_count);
    fprintf(stdout, "rescnt=%d\n", result_count);
    for (int i = 0; i < result_count; i++) {
        fprintf(stdout, "%d", *(result + i));

        if (i != result_count - 1) {
            fprintf(stdout, " ");
        }
    }

    fprintf(stdout, "\n");

    //fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) { break; }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

        size_t new_length = alloc_length << 1;
        data = realloc(data, new_length);

        if (!data) { break; }

        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
    }

    data = realloc(data, data_length);

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

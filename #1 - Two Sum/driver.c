#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "two_sum.h"

void print_help_message();
int run_test_case(char *input_filename);
void run_two_sum(int *test_case, short test_case_Size, int target, short first_addend_index, short second_addend_index);
int is_index_array_match(int *index_array, short first_addend_index, short second_addend_index);
void print_command_line_argument_error_message();

int main(int argc, char **argv)
{
    if (argc == 2)
    {
        if (strcmp(argv[1], "-h") == 0)
        {
            print_help_message();
        }
        else
        {
            print_command_line_argument_error_message();
        }
    }
    else if ((argc == 3) && (strcmp(argv[1], "-f") == 0))
    {
        return (run_test_case(argv[2]));
    }
    else
    {
        print_command_line_argument_error_message();
    }

    return (EXIT_SUCCESS);
}

void print_help_message()
{
    printf("Usage: two_sum [OPTION] [INPUT FILE]\n");
    printf("Run a test case for LeetCode question #1 - 'Two Sum'.\n\n");
    printf("-f\tprogram will run the test case specified in the command line and print the execution time or '-1' if the test fails.\n");
}

int run_test_case(char *input_filename)
{
    int target;
    short first_addend_index;
    short second_addend_index;
    short test_case_size;

    FILE *input_file_ptr = fopen(input_filename, "rb");
    if (input_file_ptr == NULL)
    {
        fprintf(stderr, "ERROR: could not open input file.\n");
        return (EXIT_FAILURE);
    }

    if (fread(&target, sizeof(int), 1, input_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case target.\n");
        return (EXIT_FAILURE);
    }

    if (fread(&first_addend_index, sizeof(short), 1, input_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case addend index 1.\n");
        return (EXIT_FAILURE);
    }

    if (fread(&second_addend_index, sizeof(short), 1, input_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case addend index 2.\n");
        return (EXIT_FAILURE);
    }

    if (fread(&test_case_size, sizeof(short), 1, input_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case size.\n");
        return (EXIT_FAILURE);
    }

    int *test_case = malloc(sizeof(int) * test_case_size);
    if (test_case == NULL)
    {
        fprintf(stderr, "ERROR: malloc could not allocate memory for the test case.\n");
        return (EXIT_FAILURE);
    }

    if (fread(test_case, sizeof(int), test_case_size, input_file_ptr) != test_case_size)
    {
        fprintf(stderr, "ERROR: could not read test case array elements.\n");
        return (EXIT_FAILURE);
    }

    run_two_sum(test_case, test_case_size, target, first_addend_index, second_addend_index);

    if (fclose(input_file_ptr) == EOF)
    {
        return (EXIT_FAILURE);
    }

    free(test_case);

    return (EXIT_SUCCESS);
}

void run_two_sum(int *test_case, short test_case_size, int target, short first_addend_index, short second_addend_index)
{
    int return_size;
    clock_t tic = clock();
    int *index_array = twoSum(test_case, test_case_size, target, &return_size);
    clock_t toc = clock();

    if ((return_size == 2) && (is_index_array_match(index_array, first_addend_index, second_addend_index)))
    {
        printf("%f\n", (double)(toc - tic) / CLOCKS_PER_SEC);
    }
    else
    {
        printf("-1\n");
    }

    free(index_array);
}

int is_index_array_match(int *index_array, short first_addend_index, short second_addend_index)
{
    if ((index_array[0] == first_addend_index) && (index_array[1] == second_addend_index))
    {
        return (1);
    }
    else if ((index_array[1] == first_addend_index) && (index_array[0] == second_addend_index))
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

void print_command_line_argument_error_message()
{
    printf("two_sum: invalid command line arguments.\nTry 'two_sum -h' for more information.\n");
}
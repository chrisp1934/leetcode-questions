#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_ARRAY_SIZE 2
#define MAX_ARRAY_SIZE 10000
#define MIN_ELEMENT_SIZE -1000000000
#define MAX_ELEMENT_SIZE 1000000000

void print_help_message();
short get_test_case_size_from_user();
int create_test_case_t(short test_case_size, char *output_filename);
int get_target_from_user();
int create_test_case_a(short test_case_size, char *output_filename);
int get_addend_from_user();
int create_test_case_r(char *output_filename);
int create_and_write_test_case_with_addends(char *output_filename, int test_case_size, int first_addend, int second_addend);
int get_random_int();
int generate_valid_random_int(int *test_case, short current_test_case_size, int first_addend, int second_addend);
void generate_random_test_case_with_parameters(int *test_case, int test_case_size, int first_addend, int second_addend);
void generate_random_addend_indices(short *first_addend_index, short *second_addend_index, short test_case_size);
int write_output_file(char *output_filename, short first_addend_index, short second_addend_index, short test_case_size, int *test_case);
int print_test_case(char *filename);
void print_command_line_argument_error_message();

int main(int argc, char **argv)
{
    if ((argc == 2) && (strcmp(argv[1], "-h") == 0))
    {
        print_help_message();
    }
    else if (argc == 3)
    {
        srand(time(NULL));

        if ((strcmp(argv[1], "-t") == 0) || (strcmp(argv[1], "-a") == 0))
        {
            short test_case_size = get_test_case_size_from_user();
            if (test_case_size == -1)
            {
                return (EXIT_FAILURE);
            }

            if (strcmp(argv[1], "-t") == 0)
            {
                return (create_test_case_t(test_case_size, argv[2]));
            }
            else if (strcmp(argv[1], "-a") == 0)
            {
                return (create_test_case_a(test_case_size, argv[2]));
            }
        }
        else if (strcmp(argv[1], "-r") == 0)
        {
            return (create_test_case_r(argv[2]));
        }
        else if (strcmp(argv[1], "-p") == 0)
        {
            return (print_test_case(argv[2]));
        }
        else
        {
            print_command_line_argument_error_message();
        }
    }
    else
    {
        print_command_line_argument_error_message();
    }

    return (EXIT_SUCCESS);
}

void print_help_message()
{
    printf("Usage: test_gen [OPTION] [OUTPUT FILE]\n");
    printf("Generate test cases for LeetCode question #1 - 'Two Sum'.\n\n");
    printf("-t\tprogram will ask for a test case size and a target, choose two random numbers that sum to the target, and place them at random locations.\n");
    printf("-a\tprogram will ask for for a test case size and two addends that will be placed at random locations.\n");
    printf("-r\tprogram will generate random values for the test case.\n");
    printf("-p\tprogram will print test case to stdout.\n");
}

short get_test_case_size_from_user()
{
    short test_case_size;
    printf("Enter the size of the test case: ");
    if (scanf("%hd", &test_case_size) != 1)
    {
        fprintf(stderr, "ERROR: scanf ran into an issue.\n");
        return (-1);
    }

    if ((test_case_size < MIN_ARRAY_SIZE) || (test_case_size > MAX_ARRAY_SIZE))
    {
        fprintf(stderr, "ERROR: test case size provided is out of the expected range.\n");
        return (-1);
    }

    return (test_case_size);
}

int create_test_case_t(short test_case_size, char *output_filename)
{
    int target = get_target_from_user();
    if (target == -1)
    {
        return (EXIT_FAILURE);
    }

    int first_addend = get_random_int() % target;
    int second_addend = target - first_addend;

    return (create_and_write_test_case_with_addends(output_filename, test_case_size, first_addend, second_addend));
}

int get_target_from_user()
{
    int target;
    printf("Enter the target: ");
    if (scanf("%d", &target) != 1)
    {
        fprintf(stderr, "ERROR: scanf ran into an issue.\n");
        return (-1);
    }

    if ((target < 2 * MIN_ELEMENT_SIZE) || (target > 2 * MAX_ELEMENT_SIZE))
    {
        fprintf(stderr, "ERROR: target provided is out of the expected range.\n");
        return (-1);
    }

    return (target);
}

int create_test_case_a(short test_case_size, char *output_filename)
{
    int first_addend = get_addend_from_user();
    int second_addend = get_addend_from_user();
    if ((first_addend == -1) || (second_addend == -1))
    {
        return (EXIT_FAILURE);
    }

    return (create_and_write_test_case_with_addends(output_filename, test_case_size, first_addend, second_addend));
}

int create_test_case_r(char *output_filename)
{
    short test_case_size = rand() % MAX_ARRAY_SIZE;
    if (test_case_size < MIN_ARRAY_SIZE)
    {
        test_case_size = 2;
    }

    int target = get_random_int();
    int first_addend = get_random_int() % target;
    int second_addend = target - first_addend;

    return (create_and_write_test_case_with_addends(output_filename, test_case_size, first_addend, second_addend));
}

int create_and_write_test_case_with_addends(char *output_filename, int test_case_size, int first_addend, int second_addend)
{
    int *test_case = malloc(test_case_size * sizeof(int));
    if (test_case == NULL)
    {
        fprintf(stderr, "ERROR: malloc could not allocate memory for the test case.\n");
        return (EXIT_FAILURE);
    }

    generate_random_test_case_with_parameters(test_case, test_case_size, first_addend, second_addend);

    short first_addend_index;
    short second_addend_index;
    generate_random_addend_indices(&first_addend_index, &second_addend_index, test_case_size);

    test_case[first_addend_index] = first_addend;
    test_case[second_addend_index] = second_addend;

    return (write_output_file(output_filename, first_addend_index, second_addend_index, test_case_size, test_case));
}

int get_addend_from_user()
{
    int addend;
    printf("Enter addend: ");
    if (scanf("%d", &addend) != 1)
    {
        fprintf(stderr, "ERROR: scanf ran into an issue.\n");
        return (-1);
    }

    if ((addend < MIN_ELEMENT_SIZE) || (addend > MAX_ELEMENT_SIZE))
    {
        fprintf(stderr, "ERROR: addend provided is out of the expected range.\n");
        return (-1);
    }

    return (addend);
}

int get_random_int()
{
    short upper_half = rand();
    short lower_half = rand();
    return (((int)upper_half << 16) | (int)lower_half);
}

int generate_valid_random_int(int *test_case, short current_test_case_size, int first_addend, int second_addend)
{
    int target = first_addend + second_addend;
    int random_int = get_random_int();

    int i;
    for (i = 0; i < current_test_case_size; i++)
    {
        while ((random_int == first_addend) || (random_int == second_addend) || ((test_case[i] + random_int) == target))
        {
            random_int = get_random_int();
            i = 0;
        }
    }

    return (random_int);
}

void generate_random_test_case_with_parameters(int *test_case, int test_case_size, int first_addend, int second_addend)
{
    int current_test_case_size;
    for (current_test_case_size = 0; current_test_case_size < test_case_size; current_test_case_size++)
    {
        int random_int = generate_valid_random_int(test_case, current_test_case_size, first_addend, second_addend);
        test_case[current_test_case_size] = random_int;
    }
}

void generate_random_addend_indices(short *first_addend_index, short *second_addend_index, short test_case_size)
{
    *first_addend_index = rand() % test_case_size;
    *second_addend_index = rand() % test_case_size;

    while (*first_addend_index == *second_addend_index)
    {
        *second_addend_index = rand() % test_case_size;
    }
}

int write_output_file(char *output_filename, short first_addend_index, short second_addend_index, short test_case_size, int *test_case)
{
    FILE *output_file_ptr = fopen(output_filename, "wb");
    if (output_file_ptr == NULL)
    {
        fprintf(stderr, "ERROR: could not open output file.\n");
        return (EXIT_FAILURE);
    }

    int target = test_case[first_addend_index] + test_case[second_addend_index];
    if (fwrite(&target, sizeof(int), 1, output_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not write test case target.\n");
        return (EXIT_FAILURE);
    }

    if (fwrite(&first_addend_index, sizeof(short), 1, output_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not write test case addend index 1.\n");
        return (EXIT_FAILURE);
    }

    if (fwrite(&second_addend_index, sizeof(short), 1, output_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not write test case addend index 2.\n");
        return (EXIT_FAILURE);
    }

    if (fwrite(&test_case_size, sizeof(short), 1, output_file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not write test case size.\n");
        return (EXIT_FAILURE);
    }

    if (fwrite(test_case, sizeof(int), test_case_size, output_file_ptr) != test_case_size)
    {
        fprintf(stderr, "ERROR: could not write test case array elements.\n");
        return (EXIT_FAILURE);
    }

    if (fclose(output_file_ptr) == EOF)
    {
        fprintf(stderr, "ERROR: could not close output file pointer.");
        return (EXIT_FAILURE);
    }

    free(test_case);

    return (EXIT_SUCCESS);
}

int print_test_case(char *filename)
{
    int target;
    short first_addend_index;
    short second_addend_index;
    short test_case_size;

    FILE *file_ptr = fopen(filename, "rb");

    if (fread(&target, sizeof(int), 1, file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case target.\n");
        return (EXIT_FAILURE);
    }

    if (fread(&first_addend_index, sizeof(short), 1, file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case addend index 1.\n");
        return (EXIT_FAILURE);
    }

    if (fread(&second_addend_index, sizeof(short), 1, file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case addend index 2.\n");
        return (EXIT_FAILURE);
    }

    if (fread(&test_case_size, sizeof(short), 1, file_ptr) != 1)
    {
        fprintf(stderr, "ERROR: could not read test case size.\n");
        return (EXIT_FAILURE);
    }

    printf("Target: %d\n", target);
    printf("Index 1: %hd Index 2: %hd\n", first_addend_index, second_addend_index);
    printf("Array Size: %hd\n", test_case_size);
    printf("Array:\n");

    int i, temp;
    for (i = 0; i < test_case_size; i++)
    {
        if (fread(&temp, sizeof(int), 1, file_ptr) != 1)
        {
            fprintf(stderr, "ERROR: could not read test case array element.\n");
            return (EXIT_FAILURE);
        }
        
        if (i != (test_case_size - 1))
        {
            printf("%d ", temp);
        }
    }

    printf("%d\n", temp);

    if (fclose(file_ptr) == EOF)
    {
        fprintf(stderr, "ERROR: could not close input file pointer.");
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

void print_command_line_argument_error_message()
{
    printf("test_gen: invalid command line arguments.\nTry 'test_gen -h' for more information.\n");
}
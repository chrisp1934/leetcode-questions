#include <stdlib.h>

#include "two_sum.h"

static short search_array_for_second_addend(short index_of_first_addend, int *nums, int numsSize, int *returnSize, int target);
static int is_sum_equal_to_target(int first_addend, int second_addend, int target);
static int *create_return_array(int index_of_first_addend, int index_of_second_addend, int *returnSize);
static int is_addend_pair_valid(short index_of_second_addend);

int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    short index_of_first_addend;
    for (index_of_first_addend = 0; index_of_first_addend < numsSize; index_of_first_addend++)
    {
        short index_of_second_addend = search_array_for_second_addend(index_of_first_addend, nums, numsSize, returnSize, target);
        if (is_addend_pair_valid(index_of_second_addend))
        {
            return (create_return_array(index_of_first_addend, index_of_second_addend, returnSize));
        }
    }

    return (NULL);
}

static short search_array_for_second_addend(short index_of_first_addend, int *nums, int numsSize, int *returnSize, int target)
{
    short index_of_second_addend;
    for (index_of_second_addend = index_of_first_addend + 1; index_of_second_addend < numsSize; index_of_second_addend++)
    {
        if (is_sum_equal_to_target(nums[index_of_first_addend], nums[index_of_second_addend], target))
        {
            return (index_of_second_addend);
        }
    }

    return (-1);
}

static int is_sum_equal_to_target(int first_addend, int second_addend, int target)
{
    return ((first_addend + second_addend) == target);
}

static int *create_return_array(int index_of_first_addend, int index_of_second_addend, int *returnSize)
{
    int *array_of_two_indices = malloc(sizeof(int) * 2);
    if (array_of_two_indices == NULL)
    {
        return (NULL);
    }

    *returnSize = 2;
    array_of_two_indices[0] = index_of_first_addend;
    array_of_two_indices[1] = index_of_second_addend;

    return (array_of_two_indices);
}

static int is_addend_pair_valid(short index_of_second_addend)
{
    return (index_of_second_addend != -1);
}
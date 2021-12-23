#include <stdlib.h>

#include "two_sum.h"

#define INCORRECT_SECOND_ADDEND_CODE -1
#define MALLOC_ERROR_CODE -2
#define LOAD_FACTOR 0.75

typedef struct _item
{
    int key;
    short index;
} item;

typedef struct _node
{
    item data;
    struct _node *next;
} node;

static node **create_and_fill_hash_table(int hash_table_size, int numsSize, int *nums);
static int fill_hash_table(node **hash_table, int hash_table_size, int numsSize, int *nums);
static short search_array_for_second_addend(node **hash_table, int hash_table_size, int *nums, short index_of_first_addend, int target);
static int insert_key_in_hash_table(node **hash_table, int hash_table_size, int key, short index);
static node *find_key_in_hash_table(node **hash_table, int hash_table_size, int key);
static void remove_key_from_hash_table(node **hash_table, int hash_table_size, int key);
static void free_hash_table(node **hash_table, int hash_table_size);
static unsigned int get_hash_value(int key, int hash_table_size);
static node *append_node(node *head, item key);
static node *create_node(item key);
static node *remove_node(node *head, int key);
static node *remove_this_node(node *head, node *prev, node *current);
static node *get_last_node(node *head);
static node *get_node_in_list(node *head, int key);
static void free_list(node *head);
static int is_addend_pair_valid(short index_of_second_addend);
static int is_hash_table_valid(short index_of_second_addend);
static int is_item_found_in_hash_table(node *hash_item_node);
static int *create_return_array(int index_of_first_addend, int index_of_second_addend, int *returnSize);

int *twoSum(int *nums, int numsSize, int target, int *returnSize)
{
    int hash_table_size = numsSize / LOAD_FACTOR;
    node **hash_table = create_and_fill_hash_table(hash_table_size, numsSize, nums);

    short index_of_first_addend;
    for (index_of_first_addend = 0; index_of_first_addend < numsSize; index_of_first_addend++)
    {
        short index_of_second_addend = search_array_for_second_addend(hash_table, hash_table_size, nums, index_of_first_addend, target);

        if (is_addend_pair_valid(index_of_second_addend))
        {
            return (create_return_array(index_of_first_addend, index_of_second_addend, returnSize));
        }
        else if (!is_hash_table_valid(index_of_second_addend))
        {
            return (NULL);
        }
    }

    free_hash_table(hash_table, hash_table_size);

    return (NULL);
}

static node **create_and_fill_hash_table(int hash_table_size, int numsSize, int *nums)
{
    node **hash_table = calloc(hash_table_size, sizeof(node *));
    if (hash_table == NULL)
    {
        return (NULL);
    }

    if (fill_hash_table(hash_table, hash_table_size, numsSize, nums) == EXIT_FAILURE)
    {
        return (NULL);
    }

    return (hash_table);
}

static int fill_hash_table(node **hash_table, int hash_table_size, int numsSize, int *nums)
{
    short i;
    for (i = 0; i < numsSize; i++)
    {
        if (insert_key_in_hash_table(hash_table, hash_table_size, nums[i], i) == EXIT_FAILURE)
        {
            free_hash_table(hash_table, hash_table_size);
            return (EXIT_FAILURE);
        }
    }

    return (EXIT_SUCCESS);
}

static short search_array_for_second_addend(node **hash_table, int hash_table_size, int *nums, short index_of_first_addend, int target)
{
    short index_of_second_addend = INCORRECT_SECOND_ADDEND_CODE;

    remove_key_from_hash_table(hash_table, hash_table_size, nums[index_of_first_addend]);

    int second_addend = target - nums[index_of_first_addend];
    node *hash_item_node = find_key_in_hash_table(hash_table, hash_table_size, second_addend);

    if (is_item_found_in_hash_table(hash_item_node))
    {
        free_hash_table(hash_table, hash_table_size);
        index_of_second_addend = (hash_item_node->data).index;
    }
    else if (insert_key_in_hash_table(hash_table, hash_table_size, nums[index_of_first_addend], index_of_first_addend) == EXIT_FAILURE)
    {
        free_hash_table(hash_table, hash_table_size);
        index_of_second_addend = MALLOC_ERROR_CODE;
    }

    return (index_of_second_addend);
}

static int insert_key_in_hash_table(node **hash_table, int hash_table_size, int key, short index)
{
    item hash_item;
    hash_item.key = key;
    hash_item.index = index;

    int hash_value = get_hash_value(key, hash_table_size);

    hash_table[hash_value] = append_node(hash_table[hash_value], hash_item);
    if (hash_table[hash_value] == NULL)
    {
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}

static void remove_key_from_hash_table(node **hash_table, int hash_table_size, int key)
{
    int hash_value = get_hash_value(key, hash_table_size);
    hash_table[hash_value] = remove_node(hash_table[hash_value], key);
}

static node *find_key_in_hash_table(node **hash_table, int hash_table_size, int key)
{
    int hash_value = get_hash_value(key, hash_table_size);
    return (get_node_in_list(hash_table[hash_value], key));
}

static void free_hash_table(node **hash_table, int hash_table_size)
{
    int i;
    for (i = 0; i < hash_table_size; i++)
    {
        free_list(hash_table[i]);
    }

    free(hash_table);
}

static unsigned int get_hash_value(int key, int hash_table_size)
{
    unsigned int positive_key = (unsigned int)key;
    return (positive_key % hash_table_size);
}

static node *append_node(node *head, item key)
{
    node *new_node = create_node(key);
    if (new_node == NULL)
    {
        return (NULL);
    }

    if (head == NULL)
    {
        head = new_node;
    }
    else
    {
        node *last_node = get_last_node(head);
        last_node->next = new_node;
    }

    return (head);
}

static node *create_node(item key)
{
    node *new_node = malloc(sizeof(node));
    new_node->data = key;
    new_node->next = NULL;

    return (new_node);
}

static node *remove_node(node *head, int key)
{
    node *cursor = head;
    node *prev = NULL;

    while (cursor != NULL)
    {
        if ((cursor->data).key == key)
        {
            return (remove_this_node(head, prev, cursor));
        }

        prev = cursor;
        cursor = cursor->next;
    }

    return (head);
}

static node *remove_this_node(node *head, node *prev, node *current)
{
    if (prev == NULL)
    {
        node *next = current->next;
        free(current);
        return (next);
    }
    else
    {
        prev->next = current->next;
        free(current);
        return (head);
    }
}

static node *get_last_node(node *head)
{
    node *cursor = head;
    if (cursor == NULL)
    {
        return (NULL);
    }

    while (cursor->next != NULL)
    {
        cursor = cursor->next;
    }

    return (cursor);
}

static node *get_node_in_list(node *head, int key)
{
    node *cursor = head;

    while (cursor != NULL)
    {
        if ((cursor->data).key == key)
        {
            return (cursor);
        }

        cursor = cursor->next;
    }

    return (NULL);
}

static void free_list(node *head)
{
    if (head == NULL)
    {
        return;
    }

    node *cursor = head;
    node *next_node = head->next;

    while (next_node != NULL)
    {
        free(cursor);
        cursor = next_node;
        next_node = cursor->next;
    }

    free(cursor);
}

static int is_addend_pair_valid(short index_of_second_addend)
{
    return (index_of_second_addend != INCORRECT_SECOND_ADDEND_CODE);
}

static int is_hash_table_valid(short index_of_second_addend)
{
    return (index_of_second_addend != MALLOC_ERROR_CODE);
}

static int is_item_found_in_hash_table(node *hash_item_node)
{
    return (hash_item_node != NULL);
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
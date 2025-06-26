#include "../cruder.h"

typedef cruder(int) int_list_t;

int main()
{
    int_list_t *list = cruder_new(int);
    int i = 42;
    cruder_push(list, i);
    cruder_push(list, 84);
    cruder_insert(list, 1, 63);
    cruder_set(list, 0, 21);
    cruder_unshift(list, 7);
    cruder_swap(list, 0, 2);
    cruder_reverse(list);
    for (int j = 0; j < list->size; j++)
    {
        printf("list[%d] = %d\n", j, cruder_get(list, j));
    }
    int index = cruder_find(list, 63);
    if (index != -1)
    {
        printf("Found 63 at index %d\n", index);
    }
    else
    {
        printf("63 not found in the list\n");
    }
    int value = cruder_pop(list);
    printf("Popped value: %d\n", value);
    value = cruder_shift(list);
    cruder_free(list);
    return 0;
}
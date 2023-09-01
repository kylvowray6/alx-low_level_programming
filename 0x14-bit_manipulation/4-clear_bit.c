#include "main.h"

/**
 * clear_bit - Sets the value of a bit to 0 at a given index.
 * @n: Pointer to the number where the bit should be cleared.
 * @index: The index of the bit to clear.
 *
 * Return: 1 if it worked, or -1 if an error occurred.
 */
int clear_bit(unsigned long int *n, unsigned int index)
{
	if (index >= sizeof(unsigned long int) * 8) /* Check if index is within bounds */
		return (-1);

	*n = *n & ~(1 << index); /* Use bitwise AND with complement to clear the bit at index */
	return (1);
}

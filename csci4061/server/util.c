/* CSci4061 S2016 Assignment 2
* section: 1
* section: 5
* date: 03/11/16
* name: Yuanhao Lu, Phoebe Zhang, Peter Nguyen (for partner(s))
* id: luxx0267, zhan2937, nguy1435 */

#include <stdio.h>
#include <string.h>
#include "util.h"

/*
 * Print shell prompt. Pass the username as argument.
 */
void print_prompt(char *name)
{
	printf("%s >> ", name);
}

/*
 * Checks if the first string starts with the second. Return 1 if true.
 */
int starts_with(const char *a, const char *b)
{
	if (strncmp(a, b, strlen(b)) == 0)
		return 1;
	else
		return 0;
}

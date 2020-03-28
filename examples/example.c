/*
 * d11 - Test program
 *
 * Example: Show how to program
 *
 * Copyright Michael Holzheu 2020
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include "lib/d11.h"
#include "lib/util_base.h"

#include "opts.h"

int main(int argc, char *argv[])
{
	opts_parse(argc, argv);
	printf("bool: %d\n", opts.bool_test);
	printf("int.: %d\n", opts.int_test);
	libd11_test();
	return 0;
}

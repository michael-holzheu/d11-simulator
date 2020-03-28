/*
 * libd11 - D11 simulation library
 *
 * Copyright Michael Holzheu 2020
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef EXAMPLE_H
#define EXAMPLE_H

struct counter {
	int d[12];
};

#define PRINTER_COUNT 8
#define COUNTER_COUNT 8

/*
 * D11 state
 */
struct d11 {
	struct counter counters[PRINTER_COUNT + 1];
	struct counter printers[COUNTER_COUNT + 1];
	int low[81];
	int high[81];
} d11;

struct connector {
	int *source;
	int *target;
};


void d11_init(int argc, char *argv[]);
void d11_run(int cards[][80], int card_count, struct connector *connectors, int con_count);

#endif /* EXAMPLE_H */

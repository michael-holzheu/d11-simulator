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
	int low[80 + 1];
	int high[80 + 1];
	int nach_obere_buersten[20 + 1];
	int nach_untere_buersten[20 + 1];
	int abriegelung[20 + 1];
	struct {
		int u;
		int h;
		int ue;
	} gruppe;
	struct {
		int zwischengang[8 + 1];
		int summenschreibung[7 + 1];
		int summenloeschung[7 + 1];
	} kb;
} d11;

struct connector {
	int *source;
	int *target;
};


void d11_init(int argc, char *argv[]);
void d11_run(int cards[][80], int card_count, struct connector *connectors, int con_count,
	     struct connector *connectors_kommando, int con_count_kommando);

#endif /* EXAMPLE_H */

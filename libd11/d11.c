/*
 * libd11 - D11 simulation library
 *
 * Basic functions
 *
 * Copyright Michael Holzheu 2020
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#include <string.h>
#include <stdio.h>

#include "lib/d11.h"
#include "opts.h"

static void process_carry(int *digit) {
	*digit += 1;
	if (*digit == 10) {
		*digit = 0;
		process_carry(digit - 1);
	}
}

static void machine_gang(struct connector *connectors, int con_count)
{
	int i, j;

	memset(d11.printers, 0, sizeof(d11.printers));
	memset(d11.nach_obere_buersten, 0, sizeof(d11.nach_obere_buersten));
	memset(d11.nach_untere_buersten, 0, sizeof(d11.nach_untere_buersten));

	/* Process connectors */
	for (i = 0; i < con_count; i++) {
		int *source = connectors[i].source;
		int *target = connectors[i].target;
		int result = *source + *target;
		*target = result % 10;
		if (result >= 10)
			process_carry(target - 1);
	}
	/* Print */
	printf("P: ");
	for (i = PRINTER_COUNT; i > 0; i--) {
		for (j = 1; j <= 11; j++) {
			printf("%d", d11.printers[i].d[j]);
		}
		printf(" ");
	}
	printf("\n");
	if (opts.debug) {
		printf("C: ");
		for (i = COUNTER_COUNT; i > 0; i--) {
			for (j = 1; j <= 11; j++) {
				printf("%d", d11.counters[i].d[j]);
			}
			printf(" ");
		}
		printf("\n");
	}
	if (opts.debug) {
		printf("U: ");
		for (i = 1; i < 21; i++) {
			printf("%d", d11.nach_untere_buersten[i]);
		}
		printf("\n");
		printf("O: ");
		for (i = 1; i < 21; i++) {
			printf("%d", d11.nach_obere_buersten[i]);
		}
		printf("\n");
	}
	if (memcmp(d11.nach_untere_buersten, d11.nach_obere_buersten,
		   sizeof(d11.nach_untere_buersten)) != 0) {
		printf("Gruppenwechsel\n");
	}
}

void d11_run(int cards[][80], int card_count, struct connector *connectors, int con_count)
{
	int i, j;

	for (i = 0; i < card_count; i++) {
		int *card = cards[i];
		for (j = 0; j < 80; j++) {
			printf("%x", card[j]);
		}
		printf("\n");
	}
	for (i = 0; i < card_count; i++) {
		int *card = cards[i];
		memcpy(&d11.high[1], &d11.low[1], 80);
		memcpy(&d11.low[1], card, 80);
		machine_gang(connectors, con_count);
	}
}

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
#include <stdlib.h>

#include "lib/d11.h"
#include "lib/util_base.h"

#include "opts.h"

#define PTR_DIFF(x, y) ((unsigned long)(((char *) (x)) - ((unsigned long) (y))))

struct {
	struct connector *connectors_start;
	int count_start;
	struct connector *connectors_kartengang;
	int count_kartengang;
	struct connector *connectors_zwischengang;
	int count_zwischengang;
	struct {
		int ue;
		int h;
		int u;
	} gruppe;
} l;

#define connector_iterate(connectors, ptr, i) \
	for (i = 0; (ptr = &connectors[i], i < l.count); i++)

static struct connector *connector_find_source_next(void *start, void *end)
{
	struct connector *min = NULL;
	int i;

	for (i = 0; i < l.count_kartengang; i++) {
		void *source = l.connectors_kartengang[i].source;
		if (source < start)
		       continue;
		if (source > end)
		       continue;
		if (min == NULL || source < (void *) min->source)
			min = &l.connectors_kartengang[i];
	}
	return min;
}

static void process_carry(int *digit) {
	*digit += 1;
	if (*digit == 10) {
		*digit = 0;
		process_carry(digit - 1);
	}
}

static void do_connectors_kartengang(void)
{
	int i;

	for (i = 0; i < l.count_kartengang; i++) {
		int *source = l.connectors_kartengang[i].source;
		int *target = l.connectors_kartengang[i].target;
		int result = *source + *target;
		*target = result % 10;
		if (result >= 10)
			process_carry(target - 1);
	}
}

static void do_connectors_start(void)
{
	int i;

	for (i = 0; i < l.count_start; i++) {
		int *source = l.connectors_start[i].source;
		int *target = l.connectors_start[i].target;
		*target = *source + *target;
	}
}

static void do_connectors_zwischengang(void)
{
	int i;

	for (i = 0; i < l.count_zwischengang; i++) {
		int *source = l.connectors_zwischengang[i].source;
		int *target = l.connectors_zwischengang[i].target;
		*target = *source + *target;
	}
}

static void zwischen_gang(void)
{
	char line[256], *ptr;
	bool printed;
	int i, j, k;

	for (i = 9; i > 0; i--) {
		if (opts.debug)
			printf("Zwischengang: %d\n", i);
		memset(&d11.kb.zwischen, 0, sizeof(d11.kb.zwischen));
		d11.kb.zwischen.gang[i] = 1;
		/* Process connectors */
		do_connectors_zwischengang();
		/* Summenschreibung */
		memset(line, 0, sizeof(line));
		printed = false;
		ptr = line;
		for (j = PRINTER_COUNT; j > 0; j--) {
			for (k = 1; k <= 11; k++) {
				if (d11.kb.zwischen.summenschreibung[j]) {
					printed = true;
					sprintf(ptr++, "%x", d11.counters[j].d[k]);
				} else {
					sprintf(ptr++, " ");
				}
			}
			sprintf(ptr++, " ");
		}
		/* Summenloeschung */
		for (j = 8; j > 0; j--) {
			if (!d11.kb.zwischen.summenloeschung[j])
				continue;
			if (opts.debug)
				printf("Loesche counter: %d\n", j);
			memset(&d11.counters[j], 0, sizeof(d11.counters[j]));
		}
		if (printed)
			printf("Z%02d: %s\n", i, line);
	}
}

static void maschinen_gang(int mc)
{
	int u = l.gruppe.u;
	int h = l.gruppe.h;
	int ue = l.gruppe.ue;
	int i, j;

	memset(&d11.gruppe, 0, sizeof(d11.gruppe));
	memset(d11.printers, 0, sizeof(d11.printers));
	memset(d11.nach_obere_buersten, 0, sizeof(d11.nach_obere_buersten));
	memset(d11.nach_untere_buersten, 0, sizeof(d11.nach_untere_buersten));
	memset(d11.abriegelung, 0, sizeof(d11.abriegelung));

	/* Process connectors */
	do_connectors_kartengang();
	/* Print */
	printf("K%02d: ", mc);
	for (i = PRINTER_COUNT; i > 0; i--) {
		for (j = 1; j <= 11; j++) {
			if (d11.kb.postenschreibung[i])
				printf("%x", d11.printers[i].d[j]);
			else
				printf(" ");
		}
		printf(" ");
	}
	printf("\n");
	if (opts.debug) {
		printf("C: ");
		for (i = COUNTER_COUNT; i > 0; i--) {
			for (j = 1; j <= 11; j++) {
				printf("%x", d11.counters[i].d[j]);
			}
			printf(" ");
		}
		printf("\n");
	}
	/* Check for Untergruppenwechsel */
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
	if (u && memcmp(&d11.nach_untere_buersten[u], &d11.nach_obere_buersten[u], (21 - u) * sizeof(int)) != 0) {
		if (opts.debug)
			printf("Untergruppenwechsel\n");
		d11.gruppe.u = 1;
	}
	if (h && memcmp(&d11.nach_untere_buersten[h], &d11.nach_obere_buersten[h], (h - u) * sizeof(int)) != 0) {
		if (opts.debug)
			printf("Hauptgruppenwechsel\n");
		d11.gruppe.h = 1;
	}
	if (ue && memcmp(&d11.nach_untere_buersten[ue], &d11.nach_obere_buersten[ue],
		   (h - u) * sizeof(int)) != 0) {
		if (opts.debug)
			printf("Uebergruppenwechsel\n");
		d11.gruppe.ue = 1;
	}
	if (d11.gruppe.u || d11.gruppe.h || d11.gruppe.ue)
		zwischen_gang();
}

static void start_init(void)
{
	d11.schalter.kb.ep = 1;
}

static void gruppen_init(void)
{
	void *ptr = &d11.abriegelung[1];
	struct connector *connector;
	unsigned long off;

	/* Find connectors in Abriegelung */
	do {
		connector = connector_find_source_next(ptr, &d11.abriegelung[20]);
		if (connector == NULL)
			return;
		off = PTR_DIFF(connector->source, &d11.abriegelung[0]) / sizeof(d11.abriegelung[1]);
		if (connector->target == &d11.gruppe.u) {
			if (opts.debug)
				printf("Untergruppe: %lu\n", off);
			l.gruppe.u = off;
		}
		if (connector->target == &d11.gruppe.h) {
			if (opts.debug)
				printf("Hauptgruppe: %lu\n", off);
			l.gruppe.h = off;
		}
		if (connector->target == &d11.gruppe.ue) {
			if (opts.debug)
				printf("Uebergruppe: %lu\n", off);
			l.gruppe.ue = off;
		}
		ptr = connector->source + 1;
	} while (ptr <= (void *) &d11.abriegelung[20]);
}

void d11_run(int cards[][80], int card_count,
	     struct connector *connectors_start, int count_start,
	     struct connector *connectors_kartengang, int count_kartengang,
	     struct connector *connectors_zwischengang, int count_zwischengang)
{
	int i, j, *card;

	l.connectors_start = connectors_start;
	l.count_start = count_start;
	l.connectors_kartengang = connectors_kartengang;
	l.count_kartengang = count_kartengang;
	l.connectors_zwischengang = connectors_zwischengang;
	l.count_zwischengang = count_zwischengang;

	gruppen_init();

	for (i = 0; i < card_count; i++) {
		int *card = cards[i];
		for (j = 0; j < 80; j++) {
			printf("%x", card[j]);
		}
		printf("\n");
	}
	memcpy(&d11.high[1], cards[0], 80);
	start_init();
	do_connectors_start();
	for (i = 1; i <= card_count; i++) {
		card = cards[i];
		memcpy(&d11.low[1], &d11.high[1], 80);
		if (i != card_count)
			memcpy(&d11.high[1], card, 80);
		else
			memset(&d11.high[1], 0, 80);
		maschinen_gang(i);
	}
}

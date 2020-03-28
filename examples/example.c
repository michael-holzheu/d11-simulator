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

struct connector connectors[] = {
	/* Artikel 1-3 */
	{&d11.low[1],&d11.printers[6].d[9]},
	{&d11.low[2],&d11.printers[6].d[10]},
	{&d11.low[3],&d11.printers[6].d[11]},
	/* Menge 4-9 */
	{&d11.low[4],&d11.printers[5].d[6]},
	{&d11.low[5],&d11.printers[5].d[7]},
	{&d11.low[6],&d11.printers[5].d[8]},
	{&d11.low[7],&d11.printers[5].d[9]},
	{&d11.low[8],&d11.printers[5].d[10]},
	{&d11.low[9],&d11.printers[5].d[11]},
	/* Menge 4-9 */
	{&d11.low[4],&d11.counters[5].d[6]},
	{&d11.low[5],&d11.counters[5].d[7]},
	{&d11.low[6],&d11.counters[5].d[8]},
	{&d11.low[7],&d11.counters[5].d[9]},
	{&d11.low[8],&d11.counters[5].d[10]},
	{&d11.low[9],&d11.counters[5].d[11]},
	/* Wert 10-16 */
	{&d11.low[10],&d11.printers[4].d[5]},
	{&d11.low[11],&d11.printers[4].d[6]},
	{&d11.low[12],&d11.printers[4].d[7]},
	{&d11.low[13],&d11.printers[4].d[8]},
	{&d11.low[14],&d11.printers[4].d[9]},
	{&d11.low[15],&d11.printers[4].d[10]},
	{&d11.low[16],&d11.printers[4].d[11]},
	/* Wert 10-16 */
	{&d11.low[10],&d11.counters[4].d[5]},
	{&d11.low[11],&d11.counters[4].d[6]},
	{&d11.low[12],&d11.counters[4].d[7]},
	{&d11.low[13],&d11.counters[4].d[8]},
	{&d11.low[14],&d11.counters[4].d[9]},
	{&d11.low[15],&d11.counters[4].d[10]},
	{&d11.low[16],&d11.counters[4].d[11]},
	/* Gruppen Kontrolle: Untere buersten */
	{&d11.low[1],&d11.nach_untere_buersten[18]},
	{&d11.low[2],&d11.nach_untere_buersten[19]},
	{&d11.low[3],&d11.nach_untere_buersten[20]},
	/* Gruppen Kontrolle: Obere buersten */
	{&d11.high[1],&d11.nach_obere_buersten[18]},
	{&d11.high[2],&d11.nach_obere_buersten[19]},
	{&d11.high[3],&d11.nach_obere_buersten[20]},
};

int cards[][80] = {
	/* Artikel Menge        Wert   DM  Pfennig */
	{  1,0,1,  0,0,0,0,2,5, 0,0,0,5,0, 0,0,},
	{  1,0,1,  0,0,0,0,5,0, 0,0,1,0,0, 0,0,},
	{  1,0,1,  0,0,0,0,3,6, 0,0,0,7,2, 0,0,},
	{  1,0,1,  0,0,0,0,4,5, 0,0,0,9,0, 0,0,},
	{  1,0,2,  0,0,0,0,3,0, 0,0,0,9,0, 0,0,},
	{  1,0,2,  0,0,0,0,2,6, 0,0,0,7,8, 0,0,},
	{  1,0,2,  0,0,0,0,4,0, 0,0,1,2,0, 0,0,},
	{  1,0,3,  0,0,0,0,2,5, 0,0,1,0,0, 0,0,},
};

int main(int argc, char *argv[])
{
	d11_init(argc, argv);
	d11_run(cards, UTIL_ARRAY_SIZE(cards), connectors, UTIL_ARRAY_SIZE(connectors));
	return 0;
}

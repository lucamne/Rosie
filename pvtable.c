#include "defs.h"
#include <stdio.h>

// we want our pv table to be max 2 mb
static const int PVSIZE = 0x100000 * 2;

void clear_pv_table(struct PvTable* table)
{
	struct PvEntry* pvEntry;
	for (pvEntry = table->pTable; pvEntry < table->pTable + table->numEntries; pvEntry++) {
		pvEntry->posKey = 0ULL;
		pvEntry->move = (U32) 0;
	}
}

void init_pv_table(struct PvTable* table)
{
	table->numEntries = PVSIZE / sizeof(struct PvEntry);
	// table->numEntries -= 2;
	
	if (table->pTable) 
		free(table->pTable);

	table->pTable = malloc(table->numEntries * sizeof(struct PvEntry));
	assert(table->pTable);
	
	clear_pv_table(table);

	printf("PvTable init complete with %d entries\n", table->numEntries);
}

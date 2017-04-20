#include "../lib/uthash.h"

typedef struct _entry {
	const char * id; /* key */
	type_t type; /* type */
	void * value; /* Concrete value */
	UT_hash_handle hh; /* makes this structure hashable */
} entry;

entry * symbol_table = NULL; // declaration of the variable

void installID(char* id, type_t type);
void* lookupID(char* id, type_t type);
void setValue(char * id, type_t type, void * value);
void freeTable();

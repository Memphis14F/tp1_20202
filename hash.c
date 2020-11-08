#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include "hash.h"

void
string_hash_init(string_hash *h)
{
	h->flag = STRING_HASH_INIT;
	h->hash = 0;
	h->size = 0;
}

extern void string_hash_more(string_hash *sh, char *str, size_t len);

void
string_hash_done(string_hash *sh)
{
	assert(sh->flag == STRING_HASH_INIT || sh->flag == STRING_HASH_MORE);

	if ((sh->hash ^= sh->size) == -1)
		sh->hash = -2;

	sh->flag = STRING_HASH_DONE;
}

int32_t
string_hash_value(string_hash *sh)
{
	return sh->hash;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gc.h"
#include "nuc.h"

nuc_val rt_make_string(size_t length, char *bytes)
{
	String *str = gc_alloc(sizeof *str + length);
	str->length = length;
	strncpy(str->bytes, bytes, length);

	return ((nuc_val)str) | STRING_LOWTAG;
}

nuc_val rt_string_length(nuc_val string)
{
	CHECK(string, STRING_LOWTAG);
	return INT_TO_NUC_VAL(((String *)REMOVE_LOWTAG(string))->length);
}

nuc_val rt_char_at(nuc_val string_val, nuc_val index_val)
{
	CHECK(string_val, STRING_LOWTAG);
	CHECK(index_val, FIXNUM_LOWTAG);
	String *string = (String *)REMOVE_LOWTAG(string_val);
	size_t index = NUC_VAL_TO_INT(index_val);

	if (index >= string->length) {
		fprintf(stderr, "Index %zd out of bounds for string\n", index);
		exit(1);
	}

	return INT_TO_NUC_VAL(string->bytes[index]);
}

nuc_val rt_char_list_to_string(nuc_val char_list)
{
	if (char_list == NIL) {
		char nothing = '\0';
		return rt_make_string(0, &nothing);
	}

	CHECK(char_list, CONS_LOWTAG);
	Cons *cons = (Cons *)REMOVE_LOWTAG(char_list);
	size_t len = rt_list_length(cons);

	char *str_bytes = malloc(len);
	str_bytes[0] = (char)NUC_VAL_TO_INT(cons->car);
	size_t i = 1;
	Cons *tmp = cons;
	for (;;) {
		nuc_val next = tmp->cdr;
		if (next == NIL)
			break;

		CHECK(next, CONS_LOWTAG);
		tmp = (Cons *)REMOVE_LOWTAG(next);
		str_bytes[i++] = (char)NUC_VAL_TO_INT(tmp->car);
	}

	nuc_val string = rt_make_string(len, str_bytes);
	free(str_bytes);
	return string;
}

nuc_val rt_string_to_char_list(nuc_val string)
{
	CHECK(string, STRING_LOWTAG);

	String *str = (String *)REMOVE_LOWTAG(string);
	if (str->length == 0)
		return NIL;

	Cons *cons = gc_alloc(sizeof *cons);
	cons->car = INT_TO_NUC_VAL(str->bytes[0]);
	for (size_t i = 1; i < str->length; i++) {
		Cons *next = gc_alloc(sizeof *cons);
		next->car = INT_TO_NUC_VAL(str->bytes[i]);
		cons->cdr = ((nuc_val)next) | CONS_LOWTAG;
		cons = next;
	}

	cons->cdr = NIL;
	return ((nuc_val)cons) | CONS_LOWTAG;
}

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include "nuc.h"

nuc_val rt_type(nuc_val val)
{
	switch (LOWTAG(val)) {
	case FIXNUM_LOWTAG: return FIXNUM_TYPE;
	case CONS_LOWTAG: return CONS_TYPE;
	case SYMBOL_LOWTAG:  return SYMBOL_TYPE;
	case STRING_LOWTAG: return STRING_TYPE;
	case LAMBDA_LOWTAG: return LAMBDA_TYPE;
	case FOREIGN_LOWTAG: return FOREIGN_TYPE;
	case EXTTAG_LOWTAG:
		switch (EXTTAG(val)) {
		case DISCRETE_EXTTAG:
			switch (val) {
			case NIL: return NIL_TYPE;
			case TRUE: case FALSE: return BOOL_TYPE;
			}

			UNREACHABLE;
		case FLOAT_EXTTAG: return FLOAT_TYPE;
		}
	}

	printf("Got an invalid lowtag '%d'\n", (int)LOWTAG(val));
	UNREACHABLE;
}

static const char *type_name(nuc_val type)
{
#define CASE(x) case x##_TYPE: return #x;

	switch (type) {
		CASE(FIXNUM) CASE(CONS) CASE(SYMBOL) CASE(STRING) CASE(LAMBDA)
		CASE(FOREIGN) CASE(NIL) CASE(BOOL) CASE(FLOAT)
		default: printf("Got type %d (ft = %d)\n", (int)type, (int)FIXNUM_TYPE); UNREACHABLE;
	}

#undef CASE
}

// TODO: doesn't handle exttags
// TODO: use libbacktrace for better error messages
void rt_check_type(nuc_val val, nuc_val expected_type,
		const char *file, const char *func, int line)
{
	nuc_val given_type = rt_type(val);
	if (given_type != expected_type) {
		fprintf(stderr, "Wrong type given! Expected %s, got %s.\n",
				type_name(expected_type), type_name(given_type));
		if (file != NULL)
			fprintf(stderr, "Error occured at %s:%d in %s\n", file, line, func);

		exit(1);
	}
}

// TODO: This should be varargs
void rt_check_union_type(nuc_val val, nuc_val expected_type1, nuc_val expected_type2)
{
	nuc_val given_type = rt_type(val);
	if (given_type != expected_type1 && given_type != expected_type2) {
		fprintf(stderr, "Wrong type given! Expected {%s, %s}, got %s.\n",
				type_name(expected_type1), type_name(expected_type2),
				type_name(given_type));

		exit(1);
	}
}

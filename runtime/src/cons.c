// Everything to do with conses

#include <stdlib.h>
#include "nuc.h"
#include "gc.h"

nuc_val rt_cons(nuc_val car, nuc_val cdr)
{
	Cons *c = gc_alloc(sizeof *c);
	c->car = car;
	c->cdr = cdr;

	return ((nuc_val)c) | CONS_LOWTAG;
}

nuc_val rt_set_car(nuc_val cons, nuc_val new_car)
{
	// TODO: type checking
	Cons *c = (Cons *)REMOVE_LOWTAG(cons);
	c->car = new_car;

	return new_car;
}

nuc_val rt_set_cdr(nuc_val cons, nuc_val new_cdr)
{
	// TODO: type checking
	Cons *c = (Cons *)REMOVE_LOWTAG(cons);
	c->cdr = new_cdr;

	return new_cdr;
}

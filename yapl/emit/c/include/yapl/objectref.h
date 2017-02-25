/* THIS FILE IS CREATED USING YAPLC */

#ifndef YAPL_OBJECTREF
#define YAPL_OBJECTREF
#include "Object.h"

struct yapl$objectref {
	struct Object *target;
	unsigned long count;
};

void yapl$objectref$init(struct yapl$objectref *ref, struct Object *object);
void yapl$objectref$push(struct yapl$objectref *ref);
void yapl$objectref$pop(struct yapl$objectref *ref);

#endif
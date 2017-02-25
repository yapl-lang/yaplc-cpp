/* THIS FILE IS CREATED USING YAPLC */

#include "yapl/objectref.h"

void yapl$objectref$init(struct yapl$objectref *ref, struct Object *object) {
	ref->count = 1;
	ref->target = object;
}

void yapl$objectref$push(struct yapl$objectref *ref) {
	++ref->count;
}

void yapl$objectref$pop(struct yapl$objectref *ref) {
	if (ref->count == 1) {
		free(ref->target);
		ref->target = NULL;
		free(ref);
	} else {
		--ref->count;
	}
}

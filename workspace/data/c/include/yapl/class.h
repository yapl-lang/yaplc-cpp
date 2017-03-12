#ifndef YAPL_CLASS
#define YAPL_CLASS

struct yapl$class {
	struct yapl$class *parent;
	struct yapl$class **interfaces;
	char *name;
	void **vtable;
};

#endif
#ifndef YAPL_CLASS
#define YAPL_CLASS

struct yapl$class {
	unsigned long size;
	char *name;

	struct yapl$class *parent;
	struct yapl$class **interfaces;
};

#endif
#ifndef SQSTACK_H
#define SQSTACK_H

#define MAXSIZE 32

typedef int datatype;

typedef struct node_st
{
	datatype data[MAXSIZE];
	int top;
}sqstack;

sqstack *st_create(void);

void st_destroy(sqstack *);

int st_isempty(sqstack *);

int st_push(sqstack *, datatype *);

int st_pop(sqstack *, datatype *);

int st_top(sqstack *, datatype *);

void st_travel(sqstack *);

#endif



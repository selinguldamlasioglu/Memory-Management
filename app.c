// This is an application using the memory allocation module
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mem.h"

int main()
{

	void *p1, *p2, *p3, *p4;
	int ret;
	ret = meminit (16); // 16 KB memory area to manage
	if (ret == -1) {
		printf ("could not create a memory area to manage");
		exit (1);
	}

	//...
	//...
	//dump_memory();
	p1 = memalloc (4); // allocate 4 KB space
	if (p1 == NULL) {
		printf ("no more memory");
		exit (1);
	}
	//dump_memory();
	// you can write something to the space pointed by p1
	p2 = memalloc (7); // allocate 7 KB space
	if (p1 == NULL) {
		printf ("no more memory");
		exit (1);
	}
	//dump_memory();



	p3 = memalloc (2); // allocate 2 KB space
	if (p3 == NULL) {
		printf ("no more memory");
		exit (1);
	}
	//dump_memory();

	p4 = memalloc (2); // allocate 2 KB space
	if (p3 == NULL) {
		printf ("no more memory");
		exit (1);
	}
	//dump_memory();

	//printf("free\n");


	memfree(p1);
	//dump_memory();

	memfree(p2);
	//dump_memory();

	memfree(p3);
	//dump_memory();

	memfree(p4);
	//dump_memory();
}

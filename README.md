Memory-Management
=================

In this project, you will write a memory allocation module (a C source file) that can be used by a program to allocate memory dynamically. 

The module will have a functionality like the malloc library, but the amount of space allocated will be in kilobytes (KB). Additionally, the module will use the Buddy System algorithm to manage memory (a logical memory area).Read the Buddy System algorithm from the text book (it is in the virtual memory chapter).You can also find information in Internet.

The module you will write will be compiled together with a program that will use it. Hence it will be part of the address space of the program. This is a user level module (library), not a kernel module. It will be just a C file without a main() function and will be first compiled into an object file (the Makefile to do this is given to you below). The source file implementing your module will be called mem.c. The prototypes of the functions implemented there will be included in a header file called mem.h which will be included in an application program (for example app.c) that would like to use the module to allocate memory in multiples of KB.

The module will implement the following functions.

int meminit(int totalmemsize); // totalmemsize is in KB

void *memalloc(int size);

void memfree (void *ptr);

The meminit function is already implemented in the mem.c file below. You will implement memalloc and memfree functions. The meminit function is using the sbrk function (please read the related man page) to create a usable logical memory area in the address space of the program (namely it extends the data section of the program; you will manage this extended
section). The global variable startptr points to the start of this memory area that will be managed by your code using the Buddy System algorithm. Note that you can not use the malloc function anymore since it is also trying to manage this area. Never use malloc in mem.c or app.c.

The maximum size of the are to be managed can be 1024 KB. The minimum size can be 16 KB. That means the min value of totalmemsize is 16 and max value is 1024. It has to be a power 2.

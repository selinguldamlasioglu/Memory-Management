// This is the implementation of the memory allocation module
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mem.h"

#define MINSIZE 16
#define MAXSIZE 1024

#define UNUSED 	0
#define SPLIT 	1
#define FULL	2
#define USED	3

#define MAX_TREE_SIZE 10000

void *startptr = NULL; 		// points to the start of memory to manage
int  tree_indices[MAX_TREE_SIZE]; 	// an integer array to maintain a tree structure
int  level;					// number of levels in the tree
int memsize = 0;


int isPowerOfTwo(int number)
{
	return !(number & (number-1));
}

int nextPowerOfTwo( int number)
{
	if( isPowerOfTwo(number))
	{
		return number;
	}

	number |= number >> 1;
	number |= number >> 2;
	number |= number >> 4;
	number |= number >> 8;
	number |= number >> 16;
	return number + 1;
}

void buddyTreeInit()
{
	int numberOfNodes = pow(2,level) - 1;
	memset(tree_indices, UNUSED, MAX_TREE_SIZE);
}

unsigned int findLevel(int tree_index)
{
	//printf("index=%d level=%d\n", tree_index, (int) floor(log2(tree_index+1)));
	return floor(log2(tree_index+1));
}

unsigned int findNodeSize(int tree_index)
{
	//printf("level=%d size=%d\n", findLevel(tree_index), (int) pow(2, level - findLevel(tree_index)));
	return pow(2, level - findLevel(tree_index));
}

unsigned int findNodeOffset(int tree_index)
{
	int node_level = findLevel(tree_index);
	//printf("%d %d\n", tree_index, index_offset(tree_index, node_level, level -1 ) );
	int offset = index_offset(tree_index, node_level, level -1 );
	return offset * 1024; // in KB

}

int index_offset(int index, int level, int max_level) {
	return ((index + 1) - (1 << level)) << (max_level - level);
}


void dumpTreeIndices()
{
	int i;
	int numberOfNodes = pow(2,level) - 1;
	printf("Tree Begin\n");
	for( i = 0; i < numberOfNodes; i++)
		printf("%d=%d\n", i, tree_indices[i]);
	printf("Tree End\n");
}

// extend the current data segment by totalmemsize
// so that we have memory to manage
// we will manage this memory
// - returns a pointer to the beginning of the memory to manage
// - parameter is in KB
int meminit(int totalmemsize)
{
	if( totalmemsize < MINSIZE)
		return -1;
	if( totalmemsize > MAXSIZE)
		return -1;

	if( !isPowerOfTwo(totalmemsize))
		return -1; // fail

	void * p;
	// read the man page of sbrk
	p = sbrk ( (intptr_t) totalmemsize * 1024);
	if (p == (void *) -1)
		return -1; // fail
	else {
		startptr = p;
		level = log2(totalmemsize)+1;
		buddyTreeInit();
		memsize = totalmemsize;
		return 0; // success
	}
}

int findBuddyIndex(int tree_index)
{
	if( (tree_index%2) == 0 )
		return tree_index-1;
	else
		return tree_index+1;
}

void updateParent( int tree_index)
{
	while(1)
	{
		int buddyIndex = findBuddyIndex(tree_index);
		if( buddyIndex > 0 && (tree_indices[buddyIndex] == USED
								|| tree_indices[buddyIndex] == FULL))
		{
			int parent_index = (tree_index+1)/2 -1;
			tree_indices[parent_index] = FULL;
			tree_index = parent_index;
		}
		else
			return;
	}
}

void dump(int index , int node_level) {
	switch (tree_indices[index]) {
	case UNUSED:
		printf("(%d:%d)", index_offset(index, node_level, level-1) , 1 << (level-1 - node_level));
		break;
	case USED:
		printf("[%d:%d]", index_offset(index, node_level, level-1) , 1 << (level-1 - node_level));
		break;
	case FULL:
		printf("{");
		dump(index * 2 + 1 , node_level+1);
		dump(index * 2 + 2 , node_level+1);
		printf("}");
		break;
	default:
		printf("(");
		dump( index * 2 + 1 , node_level+1);
		dump( index * 2 + 2 , node_level+1);
		printf(")");
		break;
	}
}

void dump_memory() {
	dump( 0 , 0);
	printf("\n");
}

// returns NULL if allocation is not successful, otherwise
// returns a pointer to the allocated space
void *memalloc(int size)
{
	// use buddy algorithm to manage memory
	int new_size;
	if( size == 0 )
	{
		new_size = 1;
	}
	else
	{
		new_size = nextPowerOfTwo(size);
	}

	int node_size = memsize;
	if( new_size > node_size)
		return NULL;

	int tree_index = 0;
	int tree_level = 0;

	while( tree_index >= 0)
	{
		if( new_size == node_size)
		{
			if( tree_indices[tree_index] == UNUSED )
			{
				tree_indices[tree_index] = USED;
				updateParent(tree_index);
				return startptr + findNodeOffset(tree_index);
			}
		}
		else
		{
			if(tree_indices[tree_index] == UNUSED)
			{
				// split
				tree_indices[tree_index] = SPLIT;
				tree_indices[tree_index*2+1] = UNUSED;
				tree_indices[tree_index*2+2] = UNUSED;
				tree_index = tree_index*2+1;
				node_size /= 2;
				tree_level++;
				continue;
			}
			else if(tree_indices[tree_index] == FULL || tree_indices[tree_index] == USED )
			{
				// do nothing
			}
			else
			{
				tree_index = tree_index*2+1;
				node_size /= 2;
				tree_level++;
				continue;
			}
		}

		if( (tree_index%2) == 1 )
		{
			tree_index++;
			continue;
		}

		while(1)
		{
			tree_level--;
			node_size *= 2;
			tree_index = (tree_index+1)/2 -1;
			if(tree_index < 0)
				return NULL;
			if( (tree_index%2) == 1)
			{
				tree_index++;
				break;
			}
		}
	}

	return NULL;

}

int findIndexFromAddr(int addr)
{

	return 0;

}

void mergeBuddies( int tree_index)
{
	while(1)
	{
		int buddyIndex = findBuddyIndex(tree_index);
		if( buddyIndex < 0 || tree_indices[buddyIndex] != UNUSED )
		{
			tree_indices[tree_index] = UNUSED;
			while( ((tree_index = (tree_index+1) / 2 - 1) >= 0)
					&& tree_indices[tree_index] == FULL )
			{
				tree_indices[tree_index] = SPLIT;
			}
			return;
		}
		tree_index = (tree_index+1) / 2 - 1;
	}
}

// ptr points to the space allocated earlier and to be
// freed now
void  memfree (void *ptr)
{
	int offset = (ptr - startptr)/1024;
	int left = 0;
	int nodesize = memsize;
	int tree_index = 0;
	while(1)
	{
		if( tree_indices[tree_index] == USED)
		{
			mergeBuddies(tree_index);
			return;
		}
		else if( tree_indices[tree_index] == UNUSED)
		{
			printf("Error, trying to free unused memory block.\n");
		}
		else
		{
			nodesize /= 2;
			if( offset < left + nodesize)
			{
				tree_index = tree_index*2+1;
			}
			else
			{
				left += nodesize;
				tree_index = tree_index*2+2;
			}
		}
	}
}

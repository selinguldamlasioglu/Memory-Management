// this is the interface of the memory allocation module (library)
int  meminit(int totalmemsize);
void *memalloc(int size);
void  memfree (void *ptr);
int isPowerOfTwo(int number);
int nextPowerOfTwo(int number);
void buddyTreeInit();
void dumpTreeIndices();
unsigned int addressOffset(int tree_index);
unsigned int findLevel(int tree_index);
unsigned int findNodeSize(int tree_index);
unsigned int findNodeOffset(int tree_index);
int index_offset(int index, int level, int max_level);
void updateParent( int tree_index);
int findBuddyIndex(int tree_index);
void dump_memory();
void dump(int index, int level);
int findIndexFromAddr(int addr);

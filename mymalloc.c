#include "mymalloc.h"

#include <stdio.h>

#define MEMLENGTH 4096

static union {
    char bytes [MEMLENGTH];
    double not_used;
} heap = { .bytes = {0} };

static int initialized = 0;

//header place at the start of every chunk in the heap
typedef struct {
    size_t size;           //Size of the chunk in bytes
    int is_allocated;   //1 = allocated, 0 = free
} ChunkHeader;

static void leakDetector(void) {
    // Scan heap for un-freed allocations at program exit
    int objectsCount = 0;
    size_t bytesLeaked = 0;

    ChunkHeader *curr = (ChunkHeader *)heap.bytes;

    while ((char *)curr < heap.bytes + MEMLENGTH) {
        if (curr->is_allocated) {
            objectsCount++;
            bytesLeaked += curr->size - sizeof(ChunkHeader);
        }
        curr = (ChunkHeader *)((char *)curr + curr->size);
    }

    if (objectsCount > 0) {
        fprintf(stderr, "mymalloc: %zu bytes leaked in %d objects.\n", bytesLeaked, objectsCount);
    }
}

static void init_heap(void) {
    //First ChunkHeader at the start of heap.bytes
    ChunkHeader *firstChunk = (ChunkHeader *)heap.bytes;

    firstChunk->size = MEMLENGTH;
    firstChunk->is_allocated = 0; //start free

    atexit(leakDetector);
}

static void coalesce(void) {
    ChunkHeader *curr = (ChunkHeader *)heap.bytes;

    while ((char *)curr < heap.bytes + MEMLENGTH) {
        ChunkHeader *next = (ChunkHeader *)((char *)curr + curr-> size);

        if ((char *)next < heap.bytes + MEMLENGTH) {
            if ((char *)next < heap.bytes + MEMLENGTH && !curr->is_allocated && !next->is_allocated) {
                curr->size += next->size;
                continue;
            }
        }

        curr = next;

    }
}

void * mymalloc(size_t size, char *file, int line) {
    if (!initialized) {
        init_heap();
    }

    if (size == 0) {
        return NULL;
    }

    size_t roundedPayload = (size + 7) & ~((size_t)7);
    size_t totalNeeded = sizeof(ChunkHeader) + roundedPayload;

    ChunkHeader *curr = (ChunkHeader *)heap.bytes;

    while ((char *)curr < heap.bytes + MEMLENGTH) {
        if (!curr->is_allocated && curr->size >= totalNeeded) {
            //space found

            if (curr->size >= totalNeeded + sizeof(ChunkHeader) + 8) {
                ChunkHeader *next = (ChunkHeader *)(char *)curr + totalNeeded;
                next->size = curr->size - totalNeeded;
                next->is_allocated = 0;

                curr->size = totalNeeded;
            }

            curr->is_allocated = 1;

            return (void *)((char *)curr + sizeof(ChunkHeader));
        }

        curr = (ChunkHeader *)((char *)curr + curr-> size);

    }

    fprintf(stderr, "malloc: Unable to allocate %zu bytes (%s:%d)\n", size, file, line);
    return NULL;
}

void myfree(void *ptr, char *file, int line) {
    if (!initialized) {
        init_heap();
    }

    if (ptr == NULL) {
        return;
    }

    if ((char *) ptr < heap.bytes + sizeof(ChunkHeader) || (char *) ptr >= heap.bytes + MEMLENGTH) {
        fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2);
    }

    ChunkHeader *curr = (ChunkHeader *) heap.bytes;

    while ((char *) curr < heap.bytes + MEMLENGTH) {
        void *payload = (void *) ((char *) curr + sizeof(ChunkHeader));

        if (payload == ptr) {
            if (!curr->is_allocated) {
                fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
                exit(2);
            }

            curr->is_allocated = 0;
            coalesce();
            return;
        }

        curr = (ChunkHeader *) ((char *) curr + curr->size);
    }

    fprintf(stderr, "free: Inappropriate pointer (%s:%d)\n", file, line);
    exit(2);
}

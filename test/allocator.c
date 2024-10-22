#include "data/allocator.h"

#include <assert.h>

static allocator_t g_allocator;

int main(void) {
    allocator_alloc(&g_allocator, 1024);
    {
        size_t size = g_allocator.head->size;

        void *ptr0 = allocator_request(&g_allocator, 64);
        assert(ptr0);

        void *ptr1 = allocator_request(&g_allocator, 64);
        assert(ptr1);

        void *ptr2 = allocator_request(&g_allocator, 64);
        assert(ptr2);

        allocator_release(&g_allocator, ptr1);
        allocator_release(&g_allocator, ptr0);

        ptr0 = allocator_request(&g_allocator, 64);
        assert(ptr0);

        ptr1 = allocator_request(&g_allocator, 64);
        assert(ptr1);

        allocator_release(&g_allocator, ptr2);
        allocator_release(&g_allocator, ptr1);
        allocator_release(&g_allocator, ptr0);

        assert(g_allocator.head == g_allocator.data);
        assert(g_allocator.head->size == size);
    }
    allocator_free(&g_allocator);
    return 0;
}

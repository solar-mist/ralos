#include <mem/kmalloc.h>
#include <mem/pmm.h>

typedef struct
{
    size_t size;
    size_t npages;
} mem_control_block_t;


void* k_malloc(size_t size)
{
    mem_control_block_t* mcb = (mem_control_block_t*)PHYS_TO_VIRT(pmm_get_pages(NPAGES(size) + 1));
    mcb->size = size;
    mcb->npages = NPAGES(size);
    return (void*)mcb + PAGE_SIZE;
}

void k_free(void* addr)
{
    mem_control_block_t* mcb = (mem_control_block_t*)(addr - PAGE_SIZE);
    pmm_free_pages(VIRT_TO_PHYS(mcb), mcb->npages + 1);
}
#include <mem/kmalloc.h>
#include <mem/pmm.h>

typedef struct
{
    size_t size;
    size_t npages;
} mem_control_block_t;


void* k_malloc(size_t size)
{
    size += sizeof(mem_control_block_t);
    mem_control_block_t* mcb = (mem_control_block_t*)PHYS_TO_VIRT(pmm_get_pages(NPAGES(size)));
    mcb->size = size;
    mcb->npages = NPAGES(size);
    return (void*)mcb + sizeof(mem_control_block_t);
}

void k_free(void* addr)
{
    mem_control_block_t* mcb = (mem_control_block_t*)(addr - sizeof(mem_control_block_t));
    pmm_free_pages(VIRT_TO_PHYS(mcb), mcb->npages);
}
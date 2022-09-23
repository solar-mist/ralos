#ifndef RALOS_MEM_PMM_H
#define RALOS_MEM_PMM_H
#include <boot/limine.h>
#include <stdbool.h>

typedef uint64_t physaddr_t;


#define PAGE_SIZE 4096
#define NPAGES(num) (((num) + PAGE_SIZE - 1) / PAGE_SIZE)


#define MEM_VIRT_OFFSET 0xFFFF800000000000
#define VIRT_TO_PHYS(addr) (((physaddr_t)(addr)) - MEM_VIRT_OFFSET)
#define PHYS_TO_VIRT(addr) (((void*)(addr)) + MEM_VIRT_OFFSET)


void install_pmm();


bool pmm_alloc(physaddr_t, uint64_t);


physaddr_t pmm_get_page();
physaddr_t pmm_get_pages(uint64_t);


void pmm_free_page(physaddr_t);
void pmm_free_pages(physaddr_t, uint64_t);


void pmm_dump_stats();

#endif
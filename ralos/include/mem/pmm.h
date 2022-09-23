#ifndef RALOS_MEM_PMM_H
#define RALOS_MEM_PMM_H
#include <boot/limine.h>
#include <stdbool.h>

typedef uint64_t physaddr_t;

void install_pmm();

bool pmm_alloc(physaddr_t, uint64_t);

physaddr_t pmm_get_page();
physaddr_t pmm_get_pages(uint64_t);


void pmm_free_page(physaddr_t);
void pmm_free_pages(physaddr_t, uint64_t);

void pmm_dump_stats();

#endif
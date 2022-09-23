#include <mem/pmm.h>
#include <drivers/terminal.h>
#include <kernel/kernel.h>
#include <kernel/util.h>
#include <stddef.h>

static volatile struct limine_memmap_request mmap = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};


static uint8_t* bitmap;
physaddr_t phys_limit;
uint32_t total_memory;
uint32_t free_memory;


inline bool bmp_test(physaddr_t addr, uint64_t npages)
{
    bool result = true;

    for(uint64_t i = addr; i < addr + (npages * PAGE_SIZE); i += PAGE_SIZE)
    {
        result = bitmap[i / (PAGE_SIZE * 8)] & (1 << ((i / PAGE_SIZE) % 8));
        if (!result)
            break;
    }

    return result;
}


bool pmm_alloc(uint64_t addr, uint64_t npages)
{
    if(!bmp_test(addr, npages))
        return false;

    for(uint64_t i = addr; i < addr + (npages * PAGE_SIZE); i += PAGE_SIZE)
        bitmap[i / (PAGE_SIZE * 8)] &= ~(1 << ((i / PAGE_SIZE) % 8));

    free_memory -= npages * PAGE_SIZE;
    return true;
}


void pmm_free_pages(physaddr_t address, uint64_t npages)
{
    for(physaddr_t i = address; i < address + (npages * PAGE_SIZE); i += PAGE_SIZE)
    {
        if(!bmp_test(i, 1))
            free_memory += PAGE_SIZE;
        
        bitmap[i / (PAGE_SIZE * 8)] |= 1 << ((i / PAGE_SIZE) % 8);
    }
}
void pmm_free_page(physaddr_t addr)
{
    pmm_free_pages(addr, 1);
}


physaddr_t pmm_get_pages(uint64_t npages)
{
    static physaddr_t lastusedpage;

    for(physaddr_t i = lastusedpage; i < phys_limit; i += PAGE_SIZE)
    {
        if(pmm_alloc(i, npages))
            return i;
    }

    for(physaddr_t i = 0; i < lastusedpage; i += PAGE_SIZE)
    {
        if(pmm_alloc(i, npages))
            return i;
    }

    k_panic("Out of physical memory - TODO: Page out memory");
}
physaddr_t pmm_get_page()
{
    return pmm_get_pages(1);
}

void install_pmm()
{
    for(size_t i = 0; i < mmap.response->entry_count; i++)
    {
        struct limine_memmap_entry* entry = mmap.response->entries[i];

        if(entry->base + entry->length <= 0x100000)
            continue;

        physaddr_t new_phys_limit = entry->base + entry->length;
        if(new_phys_limit > phys_limit)
            phys_limit = new_phys_limit;

        if(entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE || entry->type == LIMINE_MEMMAP_ACPI_RECLAIMABLE || entry->type == LIMINE_MEMMAP_KERNEL_AND_MODULES)
            total_memory += entry->length;
    }

    uint64_t bm_size = phys_limit / (PAGE_SIZE * 8);
    for(size_t i = 0; i < mmap.response->entry_count; i++)
    {
        struct limine_memmap_entry* entry = mmap.response->entries[i];

        if(entry->base + entry->length <= 0x100000)
            continue;

        if(entry->length >= bm_size && entry->type == LIMINE_MEMMAP_USABLE)
        {
            bitmap = (uint8_t*)PHYS_TO_VIRT(entry->base);
            break;
        }
    }

    memset(bitmap, 0, bm_size);
    
    for(size_t i = 0; i < mmap.response->entry_count; i++)
    {
        struct limine_memmap_entry* entry = mmap.response->entries[i];

        if(entry->base + entry->length <= 0x100000)
            continue;

        if(entry->type == LIMINE_MEMMAP_USABLE || entry->type == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE)
            pmm_free_pages(entry->base, NPAGES(entry->length));
    }

    pmm_alloc(VIRT_TO_PHYS(bitmap), NPAGES(bm_size));
}

void pmm_dump_stats()
{
    uint64_t total = total_memory, free = free_memory, used = total - free;

    t_printf("\nTotal: %d KiB (%d MiB)\n", total / 1024, total / 1024 / 1024);
    t_printf("Free: %d KiB (%d MiB)\n", free / 1024, free / 1024 / 1024);
    t_printf("Used: %d KiB (%d MiB)\n", used / 1024, used / 1024 / 1024);
}
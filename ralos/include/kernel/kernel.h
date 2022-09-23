#ifndef RALOS_KERNEL_H
#define RALOS_KERNEL_H

void k_init(void);
void k_main(void);

__attribute__((noreturn))
void k_panic(const char*);

#endif
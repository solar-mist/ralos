#ifndef CPU_TSS_HPP
#define CPU_TSS_HPP
#include <stdint.h>

struct [[gnu::packed]] TSSDescriptor
{
    uint32_t res0;

	uint64_t rsp0;
	uint64_t rsp1;
	uint64_t rsp2;

	uint32_t res1;
	uint32_t res2;

	uint64_t ist1;
	uint64_t ist2;
	uint64_t ist3;
	uint64_t ist4;
	uint64_t ist5;
	uint64_t ist6;
	uint64_t ist7;

	uint32_t res3;
	uint32_t res4;

	uint32_t iomap_base;
};

void InitTSS();

#endif
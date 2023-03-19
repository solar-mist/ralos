#ifndef CPU_TSS_HPP
#define CPU_TSS_HPP
#include <stdint.h>

struct [[gnu::packed]] TSSDescriptor
{
    uint32_t res0;

	uint32_t rsp0_l;
	uint32_t rsp0_u;
	uint32_t rsp1_l;
	uint32_t rsp1_u;
	uint32_t rsp2_l;
	uint32_t rsp2_u;

	uint32_t res1;
	uint32_t res2;

	uint32_t ist1_l;
	uint32_t ist1_u;
	uint32_t ist2_l;
	uint32_t ist2_u;
	uint32_t ist3_l;
	uint32_t ist3_u;
	uint32_t ist4_l;
	uint32_t ist4_u;
	uint32_t ist5_l;
	uint32_t ist5_u;
	uint32_t ist6_l;
	uint32_t ist6_u;
	uint32_t ist7_l;
	uint32_t ist7_u;

	uint32_t res3;
	uint32_t res4;

	uint32_t iomap_base;
};

void InitTSS();

#endif
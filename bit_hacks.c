#include <stdio.h>
#include <inttypes.h>

static inline uint64_t set_k(uint64_t x, int k) { return x | (1<<k); }
static inline uint64_t clear_k(uint64_t x, int k) { return x & ~(1<<k); }
static inline uint64_t flip_k(uint64_t x, int k) { return x ^ (1<<k); }
static inline uint64_t get_mask_shift(uint64_t x, uint64_t mask, int shift) { return (x & mask) >> shift; }
static inline uint64_t set_mask_shift(uint64_t x, uint64_t mask, uint64_t y, int shift) { return (x & ~mask) | (y << shift); }
static inline uint64_t xor_min(uint64_t x, uint64_t y) { return y ^ ((x ^ y) & -(x < y)); }
static inline uint64_t mod_add(uint64_t x, uint64_t y, uint64_t n) { uint64_t z = x + y; return z - (n & -(z >= n)); }
static inline uint64_t roundup_2(uint64_t x) { 
	uint64_t y = x--; 
			 y |= y >> 1;
			 y |= y >> 2;
			 y |= y >> 4;
			 y |= y >> 8;
			 y |= y >> 16;
			 y |= y >> 32;
	return ++y;
}
static inline uint64_t least_1(uint64_t x) { return x & (~x+1); }

int main () {
	uint64_t x = 168, y = 73;
	printf("least 1 for x=%"PRIu64", %0x\n", x, least_1(x));
	printf("roundup 2 for x=%"PRIu64", %"PRIu64"\n", x, roundup_2(x));
	printf("mod add for x=%"PRIu64", y=%"PRIu64", %"PRIu64"\n", x, y, mod_add(x,y,200));

	return 0;
}
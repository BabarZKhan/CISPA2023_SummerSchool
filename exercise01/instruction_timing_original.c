#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define ITERATIONS_NO 10000
#define MAX_LINESIZE 500

void maccess(void* address) {
  asm volatile("movq (%0), %%rax\n" : : "r"(address) : "rax");
}

void mfence() {
  asm volatile("mfence" ::: "memory");
}

void lfence() {
  asm volatile("lfence" ::: "memory");
}

size_t rdtsc() {
  int64_t low, high;
  asm volatile("rdtsc\n" : "=a"(low), "=d"(high));
  return high << 32 | low;
}

void divide(int a, int b) {
  asm volatile (
    "movl %0, %%eax\n\t" \
    "div %1"             \
      : : "r" (a), "r" (b) : "memory", "eax"); 
}


void xor(int a, int b) {
    asm volatile(
      "xor %1, %0"
      : : "r" (a), "r" (b));
}

int min(int a, int b) {
  return a < b ? a : b;
}

void measure_timing_xor(FILE* logfile_fd) {
  char* line = malloc(MAX_LINESIZE);
  for (size_t i = 0; i < ITERATIONS_NO; i++) {
    int timing = 0;
    int random_operand1 = rand();
    int random_operand2 = rand();
    // TODO: measure the timing of a XOR instruction
    sprintf(line, "xor;%d\n", timing);
    fwrite(line, min(strlen(line), MAX_LINESIZE), 1, logfile_fd);
  }
  free(line);
}

void measure_timing_div(FILE* logfile_fd) {
  char* line = malloc(MAX_LINESIZE);
  for (size_t i = 0; i < ITERATIONS_NO; i++) {
    int timing = 0;
    int random_operand1 = rand();
    int random_operand2 = rand();
    // TODO: measure the timing of a DIV instruction
    sprintf(line, "div;%d\n", timing);
    fwrite(line, min(strlen(line), MAX_LINESIZE), 1, logfile_fd);
  }
  free(line);
}

int main(int argc, char* argv[]) {
  FILE* fd = fopen("timings.log", "w");
  measure_timing_xor(fd);
  measure_timing_div(fd);
  fclose(fd);
  return 0;
}

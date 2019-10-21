#define BIMODAL_TABLE_SIZE 16384
#define BIMODAL_PRIME 16381
#define MAX_COUNTER 3

#include <stdint.h>
#include <stdio.h>

int bimodal_table[BIMODAL_TABLE_SIZE];

extern "C" void initialize_branch_predictor()
{
  for (int i = 0; i < BIMODAL_TABLE_SIZE; i++)
    bimodal_table[i] = 0;
}

extern "C" void predict_branch(unsigned long long ip, unsigned char *pred)
{
    uint32_t hash = ip % BIMODAL_PRIME;
    *pred = (bimodal_table[hash] >= ((MAX_COUNTER + 1)/2)) ? 1 : 0;
}

extern "C" void update_branch(unsigned long long ip, unsigned char taken)
{
  uint32_t hash = ip % BIMODAL_PRIME;

  if (taken && (bimodal_table[hash] < MAX_COUNTER))
    bimodal_table[hash]++;
  else if ((taken == 0) && (bimodal_table[hash] > 0))
    bimodal_table[hash]--;
}

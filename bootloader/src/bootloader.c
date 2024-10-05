#include "common-defines.h"
#include <libopencm3/stm32/memorymap.h>

#define BOOTLOADER_SIZE (0x10000U)     // 64KB(2^16)--->HEX
#define MAIN_APP_START_ADDRESS (FLASH_BASE + BOOTLOADER_SIZE)

static void jump_to_main(void){
  typedef void (*void_fn)(void);  // A function pointer

  uint32_t* main_vector_table = (uint32_t*)(MAIN_APP_START_ADDRESS);
  uint32_t* reset_vector_entry = &main_vector_table[1];  // Reset vector is the second entry in the vector table(each entry->4 bytes)
  uint32_t* reset_vector = (uint32_t*)(*reset_vector_entry);

  void_fn jump_fn = (void_fn)reset_vector;

  jump_fn();

}

int main(void){
  
  jump_to_main();
  return 0;
}
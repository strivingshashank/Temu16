#include "t_disk.h"
#include "asm_bindings.h"

#define HEADS_PER_CYLINDER 2
#define SECTOR_PER_TRACK 18

bit8_t disk_read(bit16_t linear_block_address, bit16_t destination_segement, bit16_t destination_offset, bit16_t sectors_to_read) {
  bit8_t cylinder_index = linear_block_address / (HEADS_PER_CYLINDER * SECTOR_PER_TRACK);
  bit8_t head_index = (linear_block_address / SECTOR_PER_TRACK) % HEADS_PER_CYLINDER;
  bit8_t sector = (linear_block_address % SECTOR_PER_TRACK) + 1;

  return _disk_read(cylinder_index, head_index, sector, destination_segement, destination_offset, sectors_to_read);
}


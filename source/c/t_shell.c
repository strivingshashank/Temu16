#include "t_shell.h"
#include "t_types.h"
#include "t_disk.h"
#include "t_io.h"
#include "t_utils.h"
#include "asm_bindings.h"

#define MAX_COMMAND_BUFFER_SIZE 20

#define PROGRAM_SEGMENT 0x4000
#define MAX_PROGRAM_SECTORS 64
#define BASE_PROGRAM_LBA 129

static void process_command(bit8_t *command_buffer);
static void load_program(bit16_t program_index);

// bit8_t shell_prompt[10] = "TShell > ";
bit8_t shell_prompt[3] = "$ ";

void shell(void) {  
  bit8_t command_buffer[MAX_COMMAND_BUFFER_SIZE];
  
  while (TRUE) {
    write_string(shell_prompt);
    read_string(command_buffer, MAX_COMMAND_BUFFER_SIZE);
    process_command(command_buffer);
  }
}

void process_command(bit8_t *command_buffer) {
  // write_string(command_buffer);
  // write_char('\n');
  
  if (string_compare(command_buffer, "tshell")) {
    write_string("Okay, this works. Let's go from here.\n");
    return;
  }

  if (string_compare(command_buffer, "run")) {
    load_program(0);
    return;
  }
  
}

void load_program(bit16_t program_index) {
  bit16_t program_lba = (program_index + 1) * BASE_PROGRAM_LBA;
  bit8_t disk_read_status = disk_read(program_lba, PROGRAM_SEGMENT, 0x00, MAX_PROGRAM_SECTORS);

  if (disk_read_status == 0) {
    // write_string("Jumping...\n");
    _jump_far(PROGRAM_SEGMENT, 0x00);
    /* Un-reachabe code */
  }
  
  write_string("Program load failed.\n");
  write_dec(disk_read_status);
}


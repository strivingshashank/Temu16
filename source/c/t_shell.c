#include "t_shell.h"
#include "t_types.h"
#include "t_disk.h"
#include "t_io.h"
#include "t_utils.h"
#include "t_memory.h"
#include "asm_bindings.h"

#define MAX_COMMAND_BUFFER_SIZE 20

#define PROGRAM_SEGMENT 0x4000
#define MAX_PROGRAM_SECTORS 64
#define BASE_PROGRAM_LBA 129

static void process_command(bit8_t *command_buffer);
static void load_program(bit16_t program_index);

// bit8_t shell_prompt[12] = "%^^@%^*!#! ";
// bit8_t shell_prompt[10] = "TShell > ";
bit8_t shell_prompt[3] = "$ ";

void shell(void) {  
  bit8_t command_buffer[MAX_COMMAND_BUFFER_SIZE];
  
  while (TRUE) {
    write_str(shell_prompt);
    read_str(command_buffer, MAX_COMMAND_BUFFER_SIZE);
    process_command(command_buffer);
  }
}

void process_command(bit8_t *command_buffer) {
  if (str_cmp(command_buffer, "clear")) {
    clear_screen();
    return;
  }

  if (str_cmp(command_buffer, "help")) {
    write_str("----- Help -----\n");

    write_str("Commands:\n");
    write_str(" - help - shows *this list\n");
    write_str(" - clear - clear screen\n");
    write_str(" - time - get current time\n");
    write_str(" - date - get current date\n");
    write_str(" - shutdown - shut sytem down\n");

    write_str("Programs:\n");
    write_str(" - tinfo - get system info\n");
    write_str(" - tcalc - a simple calculator (uses shunting-yard)\n");
    
  }
  
  if (str_cmp(command_buffer, "shell")) {
    write_str("Okay, this works. Let's go from here.\n");
  }

  if (str_cmp(command_buffer, "date")) {
    bit8_t *time_date_str = time_get_date_str();
    write_str("Date: ");
    write_str(time_date_str);
    write_char('\n');
    heap_free(time_date_str, sizeof(bit8_t));
  }

  if (str_cmp(command_buffer, "time")) {
    bit8_t *time_str = time_get_time_str();
    write_str("Time: ");
    write_str(time_str);
    write_char('\n');
    heap_free(time_str, sizeof(bit8_t));
  }

  // if (str_cmp(command_buffer, "heapinfo") == 1) {
  //     bit16_t used_bytes = heap_get_used_bytes();
  //     bit16_t free = HEAP_SIZE - used_bytes;

  //     write_str("[Heap State]\n");

  //     heap_alloc(100);
      
  //     write_str(" Used: ");
  //     write_dec(used_bytes);
  //     write_str(" Bytes\n");
      
  //     write_str("Free: ");
  //     write_dec(free);
  //     write_str(" Bytes\n");

  //     write_str("Total: ");
  //     write_dec(HEAP_SIZE);
  //     write_str(" Bytes\n");
  //     return;
  // }

  if (str_cmp(command_buffer, "shutdown")) {
    write_str("Shutdown Temu?\n");
    write_str("y (shutdown) / Any other key (skip)\n");

    if ((bit8_t)(get_key_blocking()) == 'y') {
      _sys_shutdown();
    } 
  }
  
  if (str_cmp(command_buffer, "calc")) {
    load_program(0);
  }
  
  if (str_cmp(command_buffer, "info")) {
    load_program(1);
  }

  write_char('\n');
}

void load_program(bit16_t program_index) {
  bit16_t program_lba = BASE_PROGRAM_LBA + (program_index * LBA_SECTOR_SIZE);
  bit8_t disk_read_status = disk_read(program_lba, PROGRAM_SEGMENT, 0x00, MAX_PROGRAM_SECTORS);

  if (disk_read_status == 0) {
    _jump_far(PROGRAM_SEGMENT, 0x00);
    /* Un-reachabe code */
  }
  
  write_str("Program load failed.\n");
  write_dec(disk_read_status);
}


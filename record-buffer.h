#ifndef _MYSISDIG_RECORD_BUFFER_H
#define _MYSISDIG_RECORD_BUFFER_H
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>

#include "core-hook.h"
#include "dump-file.h"

#define PROC_MEMSHARE_DIR "memshare"
#define PROC_MEMSHARE_INFO "phymem_info"
#define PROC_MMAP_FILE "mmap"
#define FILE_BASE_DIR "/home/root/syscall-record"

/* alloc one page. 4096 bytes */
#define PAGE_ORDER 2
/* this value can get from PAGE_ORDER */
#define PAGES_NUMBER 4
#define BUF_SIZE (1024 * 1024 * 8)  // 8M

char real_buffer[BUF_SIZE];

static unsigned long real_offset = 0;
extern u64 syscall_count;
static bool check_offset(unsigned long str_len) {
  return real_offset + str_len < BUF_SIZE;
}

static void dump_to_file(void) {
  // definitely needs a lock...

  // lock start
  // WRITE_FILE_LOCK();

  printk(
      "it is time to dump records to file with offset: %ld, current stat: %lld",
      real_offset, syscall_count);
  // unregister_syscall_hook();
  save_to_file(real_buffer, real_offset);
  real_offset = 0;

  // lock end
  // WRITE_FILE_UNLOCK();
}

static void write_something_to_buffer(const char* src,
                                      const unsigned long str_len) {
  unsigned long offset_end = real_offset + str_len;
  strncpy(real_buffer + real_offset, src, str_len);
  real_offset = offset_end;
}

// static void write_something(const char *src, u64 *__offset) {
//   unsigned long str_len = strlen(src);
//   unsigned long offset_end = real_offset + str_len;

//   if (offset_end >= BUF_SIZE) {
//     printk("it is time to dump records to file");
//     // unregister_syscall_hook();
//     save_to_file(real_buffer, real_offset);
//     // register_syscall_hook();
//     real_offset = 0;
//     offset_end = str_len;
//     memset(real_buffer, 0, BUF_SIZE);
//   }
//   strncpy(real_buffer + real_offset, src, str_len);
//   real_offset = offset_end;
// }

// static int init_remapping(void) {
//   return 0;
// }

// static void exit_remapping(void) {
//   return;
// }

static void dump_buffer(void) {
  // char tmp_buf[128];
  unsigned long offset = 0;
  unsigned long a;
  while (1) {
    printk("%s\n", real_buffer + offset);
    a = strlen(real_buffer + offset);
    if (a == 0) {
      break;
    }
    offset += a + 1;
  }
}
#endif
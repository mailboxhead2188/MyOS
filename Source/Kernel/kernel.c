#include "system.h"
#include "boot.h"

void kmain(struct multiboot *mboot, unsigned int mboot_magic, unsigned int esp)
{
  const char *str = "Welcome to the kernel";
  struct multiboot *mboot_local = NULL;
  if (mboot_magic == MULTIBOOT_EAX_MAGIC) {
    mboot_local = mboot;
  }

  clear_screen();
  gdt_install(); 
  idt_install();
  isrs_install();
  irq_install();
  kb_install();
  timer_install();  
  serial_install();

  if (mboot_local != NULL) {
    // Do something with our memory
  }
  kprint(str);
  kprint_newline();
  
  while(1);
}

#include "system.h"

#define IDT_SIZE 256


struct IDT_entry {
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];


void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
  unsigned long func_address = (unsigned long)base;
  IDT[num].offset_lowerbits = func_address & 0xffff;
  IDT[num].selector = sel;
  IDT[num].zero = 0;
  IDT[num].type_attr = flags;
  IDT[num].offset_higherbits = (func_address & 0xffff0000) >> 16;
}

void idt_install(void)
{
  unsigned long idt_address;
  unsigned long idt_ptr[2];

  idt_address = (unsigned long)IDT;
  idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
  idt_ptr[1] = idt_address >> 16;

  load_idt(idt_ptr);
}

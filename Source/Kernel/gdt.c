#include "system.h"

extern void tss_flush();

struct gdt_entry {
  unsigned short limit_low;
  unsigned short base_low;
  unsigned char base_middle;
  unsigned char access;
  unsigned char granularity;
  unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr {
  unsigned short limit;
  unsigned int base;
} __attribute__((packed));

struct gdt_entry GDT[6];
struct gdt_ptr gp;
tss_entry_t tss_entry;

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
  GDT[num].base_low = (base & 0xffff);
  GDT[num].base_middle = (base >> 16) & 0xff;
  GDT[num].base_high = (base >> 24) & 0xff;

  GDT[num].limit_low = (limit & 0xffff);
  GDT[num].granularity = ((limit >> 16) & 0x0f);

  GDT[num].granularity |= (gran & 0xf0);
  GDT[num].access = access;
}


void gdt_install() {
  gp.limit = (sizeof(struct gdt_entry) * 6) - 1;
  gp.base = (unsigned int)&GDT;

  gdt_set_gate(0,0,0,0,0);

  gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);

  gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);

  gdt_set_gate(3, 0, 0xffffffff, 0xFA, 0xcf);

  gdt_set_gate(4, 0, 0xffffffff, 0xf2, 0xcf);

  write_tss(5, 0x10, 0x0);

  gdt_flush();

  tss_flush();
}

void write_tss(unsigned int num, unsigned short ss0, unsigned int esp0) {
  unsigned int base = (unsigned int)&tss_entry;
  unsigned int limit = base + sizeof(tss_entry);

  gdt_set_gate(num, base, limit, 0xe9, 0x00);

  tss_entry.ss0 = ss0;
  tss_entry.esp0 = esp0;
  tss_entry.cs = 0x0b;
  tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
  tss_entry.iomap_base = sizeof(tss_entry);
  
}

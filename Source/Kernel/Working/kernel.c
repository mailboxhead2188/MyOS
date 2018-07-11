#include "keyboardmap.h"
#include "string.h"

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

#define ENTER_KEY_CODE 0x1c

int timer_ticks = 0;

extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);
extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);
extern void load_idt(unsigned long *idt_ptr);
extern void gdt_flush();
extern int enableA20();

// Interrupt service routines
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

// Interrupts
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

void *irq_routines[16] = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0
};



unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;
const char *prompt = "local>";

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

struct IDT_entry {
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
};

struct regs {
  unsigned int gs, fs, es, ds;
  unsigned int edit, esi, ebp, esp, ebs, edx, ecs, eax;
  unsigned int int_no, err_code;
  unsigned int eip, cs, eflags, useresp, ss;
};

struct IDT_entry IDT[IDT_SIZE];
struct gdt_entry GDT[3];
struct gdt_ptr gp;

void timer_phase(int hz) {
  int divisor = 1193180 / hz;
  write_port(0x43, 0x36);
  write_port(0x40, divisor & 0xff);
  write_port(0x40, divisor >> 8 );
}

void timer_wait(int ticks) {
  unsigned long eticks;

  eticks = timer_ticks + ticks;
  while (timer_ticks < eticks);
}

void irq_install_handler(int irq, void (*handler)(struct regs *r)) {
  irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq) {
  irq_routines[irq] = 0;
}

void irq_remap(void) {
  write_port(0x20, 0x11);
  write_port(0xA0, 0x11);
  write_port(0x21, 0x20);
  write_port(0xA1, 0x28);
  write_port(0x21, 0x04);
  write_port(0xA1, 0x02);
  write_port(0x21, 0x01);
  write_port(0xA1, 0x01);
  write_port(0x21, 0x0);
  write_port(0xA1, 0x0);
}

void irq_handler(struct regs *r) {
  void (*handler)(struct regs *r);

  handler = irq_routines[r->int_no - 32];
  
  if (handler) {
    handler(r);
  }

  if (r->int_no >= 40) {
    write_port(0xA0, 0x20);
  }

  write_port(0x20, 0x20);
}

void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
  GDT[num].base_low = (base & 0xffff);
  GDT[num].base_middle = (base >> 16) & 0xff;
  GDT[num].base_high = (base >> 24) & 0xff;

  GDT[num].limit_low = (limit & 0xffff);
  GDT[num].granularity = ((limit >> 16) & 0x0f);

  GDT[num].granularity |= (gran & 0xf0);
  GDT[num].access = access;
}

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
  unsigned long func_address = (unsigned long)base;
  IDT[num].offset_lowerbits = func_address & 0xffff;
  IDT[num].selector = sel;
  IDT[num].zero = 0;
  IDT[num].type_attr = flags;
  IDT[num].offset_higherbits = (func_address & 0xffff0000) >> 16;
}

void gdt_install() {
  gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
  gp.base = (unsigned int)&GDT;

  gdt_set_gate(0,0,0,0,0);

  gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);

  gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);

  gdt_flush();
}

void isrs_install(void) {
  idt_set_gate(0, (unsigned)isr0, 0x08, 0x8e);
  idt_set_gate(1, (unsigned)isr1, 0x08, 0x8e);
  idt_set_gate(2, (unsigned)isr2, 0x08, 0x8e);
  idt_set_gate(3, (unsigned)isr3, 0x08, 0x8e);
  idt_set_gate(4, (unsigned)isr4, 0x08, 0x8e);
  idt_set_gate(5, (unsigned)isr5, 0x08, 0x8e);
  idt_set_gate(6, (unsigned)isr6, 0x08, 0x8e);
  idt_set_gate(7, (unsigned)isr7, 0x08, 0x8e);
  idt_set_gate(8, (unsigned)isr8, 0x08, 0x8e);
  idt_set_gate(9, (unsigned)isr9, 0x08, 0x8e);
  idt_set_gate(10, (unsigned)isr10, 0x08, 0x8e);
  idt_set_gate(11, (unsigned)isr11, 0x08, 0x8e);
  idt_set_gate(12, (unsigned)isr12, 0x08, 0x8e);
  idt_set_gate(13, (unsigned)isr13, 0x08, 0x8e);
  idt_set_gate(14, (unsigned)isr14, 0x08, 0x8e);
  idt_set_gate(15, (unsigned)isr15, 0x08, 0x8e);
  idt_set_gate(16, (unsigned)isr16, 0x08, 0x8e);
  idt_set_gate(17, (unsigned)isr17, 0x08, 0x8e);
  idt_set_gate(18, (unsigned)isr18, 0x08, 0x8e);
  idt_set_gate(19, (unsigned)isr19, 0x08, 0x8e);
  idt_set_gate(20, (unsigned)isr20, 0x08, 0x8e);
  idt_set_gate(21, (unsigned)isr21, 0x08, 0x8e);
  idt_set_gate(22, (unsigned)isr22, 0x08, 0x8e);
  idt_set_gate(23, (unsigned)isr23, 0x08, 0x8e);
  idt_set_gate(24, (unsigned)isr24, 0x08, 0x8e);
  idt_set_gate(25, (unsigned)isr25, 0x08, 0x8e);
  idt_set_gate(26, (unsigned)isr26, 0x08, 0x8e);
  idt_set_gate(27, (unsigned)isr27, 0x08, 0x8e);
  idt_set_gate(28, (unsigned)isr28, 0x08, 0x8e);
  idt_set_gate(29, (unsigned)isr29, 0x08, 0x8e);
  idt_set_gate(30, (unsigned)isr30, 0x08, 0x8e);
  idt_set_gate(31, (unsigned)isr31, 0x08, 0x8e); 
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

void irq_install() {
  irq_remap();
  
  idt_set_gate(32 ,(unsigned)irq0, 0x08, 0x8e);
  idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
  idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
  idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
  idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
  idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
  idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
  idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
  idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
  idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
  idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
  idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
  idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
  idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
  idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
  idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

void kprint(const char *str)
{
  unsigned int i = 0;
  while (str[i] != '\0')
  {
    vidptr[current_loc++] = str[i++];
    vidptr[current_loc++] = 0x07;
  }
}

void fault_handler(struct regs *r) {
  if (r->int_no < 32) {
    kprint("An interrupt occurred");
    kprint("System halted");
    for (;;);
  }
}

void scroll_screen(void)
{
  // start at line 2 (2 * 2 * 80), copy to the line above (current_loc - COLUMNS_IN_LINE)
  for(int i = (COLUMNS_IN_LINE * BYTES_FOR_EACH_ELEMENT); i < SCREENSIZE; i++)
  {
    vidptr[(i - (COLUMNS_IN_LINE * BYTES_FOR_EACH_ELEMENT))] = vidptr[i];
  }

  // Set the current_loc to be at the bottom line of the screen
  current_loc = ((COLUMNS_IN_LINE * BYTES_FOR_EACH_ELEMENT) * (LINES - 1));

  // clear the lowest line on the screen
  for(int j = current_loc; j < SCREENSIZE; j++) {
    vidptr[j] = ' ';
    vidptr[j++] = 0x07;
  }
}


void kprint_newline(void)
{
  unsigned int line_size = BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE;
  current_loc = current_loc + (line_size - current_loc % (line_size));
  if (current_loc >= SCREENSIZE) {
    scroll_screen();
  }
  kprint(prompt);
}

void clear_screen(void)
{
  unsigned int i = 0;
  while ( i < SCREENSIZE )
  {
    vidptr[i++] = ' ';
    vidptr[i++] = 0x07;
  }
}


void keyboard_handler_main(void)
{
  unsigned char status;
  char keycode;

  // Write EOI
  //write_port(0x20, 0x20);

  status = read_port(KEYBOARD_STATUS_PORT);

  // Lowest bit of status will be set if the buffer isn't empty
  if (status & 0x01)
  {
    keycode = read_port(KEYBOARD_DATA_PORT);
    if (keycode < 0)
      return;

    if (keycode == ENTER_KEY_CODE)
    {
      kprint_newline();
      return;
    }

    vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
    vidptr[current_loc++] = 0x07;
  }
}

void timer_handler(struct regs *r) {
  timer_ticks++;

    kprint("tick");
 
}

void kb_install(void)
{
  // 11111101 - Turns on the keyboard interrup
  irq_install_handler(1, keyboard_handler_main);

}

void timer_install(void) {
  irq_install_handler(0, timer_handler);
}

void kmain(void)
{
  const char *str = "Welcome to the kernel";
  const char *a20 = "Protected mode enabled";
  const char *a20Fail = "Moving to protected mode failed!";

  clear_screen();
  gdt_install(); 
  idt_install();
  isrs_install();
  irq_install();
  kb_install();
  timer_install();

  if (enableA20() == 0) {
    kprint(a20);
    kprint_newline();
  }
  else {
    kprint(a20Fail);
    kprint_newline();
  }

  kprint(str);
  kprint_newline();
  
  while(1);
}

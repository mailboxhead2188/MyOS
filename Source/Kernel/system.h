#ifndef __system_h
#define __system_h

#define NULL 0

extern char read_port(unsigned short port);
extern void write_port(unsigned short port, unsigned char data);

struct regs {
  unsigned int gs, fs, es, ds;
  unsigned int edit, esi, ebp, esp, ebs, edx, ecs, eax;
  unsigned int int_no, err_code;
  unsigned int eip, cs, eflags, useresp, ss;
};

struct multiboot {
  unsigned int flags;
  unsigned int mem_lower;
  unsigned int mem_upper;
  unsigned int boot_device;
  unsigned int cmdline;
  unsigned int mods_count;
  unsigned int mods_addr;
  unsigned int num;
  unsigned int size;
  unsigned int addr;
  unsigned int shndx;
  unsigned int mmap_length;
  unsigned int mmap_addr;
  unsigned int drives_length;
  unsigned int drives_addr;
  unsigned int config_table;
  unsigned int boot_loader_name;
  unsigned int apm_table;
  unsigned int vbe_control_info;
  unsigned int vbe_mode_info;
  unsigned int vbe_mode;
  unsigned int vbe_interface_seg;
  unsigned int vbe_interface_off;
  unsigned int vbe_interface_len;
} __attribute__ ((packed));

typedef struct {
  unsigned short int attributes;
  char winA, winB;
  unsigned short int granularity;
  unsigned short int winsize;
  unsigned short int segmentA, segmentB;
  unsigned int realFctPtr;
  unsigned short int pitch;

  unsigned short int Xres, Yres;
  char  Wchar, Ychar, planes, bpp, banks;
  char memory_model, bank_size, image_pages;
  char reserved0;

  char red_mask, red_position;
  char green_mask, green_position;
  char blue_mask, blue_position;
  char rsv_mask, rsv_position;
  char directcolor_attributes;

  unsigned int physbase;
  unsigned int reserved1;
  unsigned int reserved2;
} __attribute__ ((packed)) vbe_info_t;


// kb.h
void keyboard_handler_main(void);
void kb_install(void);

// irq.h

void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void irq_remap(void);
void irq_handler(struct regs *r);
void irq_install(void);

// console.h
void kprint(const char *str);
void scroll_screen(void);
void kprint_newline(void);
void clear_screen(void);

// idt.h
extern void load_idt(unsigned long *idt_ptr);
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install(void);

// isrs.h
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
void isrs_install(void);
void fault_handler(struct regs *r);

// gdt.h
extern void gdt_flush();
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
void gdt_install(void);

// multiboot.h
struct multiboot *copy_multiboot(struct multiboot *mboot_ptr);
void dump_multiboot(struct multiboot *mboot_ptr);

// tss
typedef struct tss_entry {
  unsigned int prev_tss;
  unsigned int esp0;
  unsigned int ss0;
  unsigned int esp1;
  unsigned int ss1;
  unsigned int esp2;
  unsigned int ss2;
  unsigned int cr3;
  unsigned int eip;
  unsigned int eflags;
  unsigned int eax;
  unsigned int ecs;
  unsigned int edx;
  unsigned int ebx;
  unsigned int esp;
  unsigned int ebp;
  unsigned int esi;
  unsigned int edi;
  unsigned int es;
  unsigned int cs;
  unsigned int ss;
  unsigned int ds;
  unsigned int fs;
  unsigned int gs;
  unsigned int ldt;
  unsigned int trap;
  unsigned int iomap_base;
} __attribute__ ((packed)) tss_entry_t;

void write_tss(unsigned int num, unsigned short ss0, unsigned int esp0);

// timer.h
void timer_wait(int ticks);
void timer_handler(struct regs *r);
void timer_install(void);

// serial.h
#define SERIAL_PORT_A 0xcf8
#define SERIAL_PORT_B 0x2f8
#define SERIAL_PORT_C 0x3e8
#define SERIAL_PORT_D 0x2e8

#define SERIAL_IRQ 4
void serial_enable(int device);
void serial_install();
int serial_rcvd(int device);
char serial_recv(int device);
char serial_recv_async(int device);
int serial_transmit_empty(int device);
void serial_send(int device, char out);
void serial_string(int device, char * out);

unsigned char inportb(unsigned short _port);
void outportb(unsigned short _port, unsigned char _data);

// multiboot.h
struct multiboot *copy_multiboot(struct multiboot *mboot_ptr);
void dump_multiboot(struct multiboot *mboot_ptr);

#endif

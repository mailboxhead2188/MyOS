#include "keyboardmap.h"

unsigned short int KEYBOARD_STATUS_PORT = 0x64;
unsigned short int KEYBOARD_DATA_PORT = 0x60;
char *vidptr;
unsigned short int current_loc;

extern void write_port(unsigned short port, unsigned char data);
extern char read_port(unsigned short port);
extern void load_idt(unsigned long *idt_ptr);
extern void keyboard_handler(void);
extern void idt_init();
extern void kb_init();

void kmain(void)
{
    const char *str = "Hello from kernel-land";
    vidptr = (char*)0xb8000; // Video memory starts here
    unsigned int i = 0;
    unsigned int j = 0;
    current_loc = 80 * 3;

    // Clear the screen of characters
    while (j < 80 * 25 * 2)
      {
	vidptr[j] = ' '; // Character byte
	vidptr[j+1] = 0x07; // Attribute byte - light grey on black screen
	j = j+2;
      }

    j = 0;

    while (str[j] != '\0')
      {
	vidptr[i] = str[j]; // Character
	vidptr[i+1] = 0x07; // Attribute byte, again
	++j;
	i = i + 2;
      }

    idt_init();
    kb_init();
}

void kb_init(void)
{
  write_port(0x21, 0xFD);
}

void keyboard_handler_main(void)
{
  unsigned char status;
  char keycode;

  write_port(0x20, 0x20);

  status = read_port(KEYBOARD_STATUS_PORT);
  if (status & 0x01)
    {
      keycode = read_port(KEYBOARD_DATA_PORT);
      if (keycode < 0) return;

      vidptr[current_loc++] = keyboard_map[keycode];
      vidptr[current_loc++] = 0x07;
    }
}

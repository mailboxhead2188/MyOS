#include "system.h"
#include "keyboardmap.h"

#define LINES 25
#define COLUMNS_IN_LINE 80
#define BYTES_FOR_EACH_ELEMENT 2
#define SCREENSIZE BYTES_FOR_EACH_ELEMENT * COLUMNS_IN_LINE * LINES

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define ENTER_KEY_CODE 0x1c


extern unsigned char keyboard_map[128];
extern void keyboard_handler(void);

unsigned int current_loc = 0;
char *vidptr = (char*)0xb8000;
const char *prompt = "local>";


void kprint(const char *str)
{
  unsigned int i = 0;
  while (str[i] != '\0')
  {
    vidptr[current_loc++] = str[i++];
    vidptr[current_loc++] = 0x07;
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
      serial_send(SERIAL_PORT_A, '\n');
      return;
    }

    vidptr[current_loc++] = keyboard_map[(unsigned char) keycode];
    vidptr[current_loc++] = 0x07;
    serial_send(SERIAL_PORT_A, keyboard_map[(unsigned char) keycode]);
  }
}

void kb_install(void)
{
  // 11111101 - Turns on the keyboard interrup
  irq_install_handler(1, keyboard_handler_main);

}

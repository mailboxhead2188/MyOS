#include <stdio.h>

#define MAX_LENGTH 1024

void itoa(int i, char *c);
void itoa2(int i, char *c);
void reverse(char str[], int length);

void main() {
  char end_string[MAX_LENGTH];
  int i = 100;

  itoa(0, end_string);
  printf("%s\n", end_string);

  itoa(100, end_string);
  printf("%s\n", end_string);

  itoa(12345, end_string);
  printf("%s\n", end_string);

  itoa(-2343, end_string);
  printf("%s\n", end_string);

  itoa2(12341234, end_string);
  printf("%s\n", end_string);

  itoa2(0, end_string);
  printf("%s\n", end_string);
}

void itoa2(int i, char *c) {
  int j = i;
  int places = 0;
  
  while( j != 0) {
    places++;
    j = j / 10;
  }

  char tmp[places];
  *c = &tmp;

  itoa(i, c);
}

void itoa(int i, char c[]) {
  if (i == 0) {
    c[0] = '0';
    c[1] = '\0';
    return;
  }

  short isNegative = 0;
  int currIndex = 0;
  
  if (i < 0) {
    isNegative = 1;
    i = -i;
  }

  while( i != 0) {
    int rem = i % 10;
    c[currIndex++] = (rem > 9)? (rem - 10) + 'a' : rem + '0';
    i = i / 10;
  }

  if (isNegative) {
    c[currIndex++] = '-';
  }

  c[currIndex] = '\0';

  reverse(c, currIndex);

  return;
}

void reverse(char str[], int length) {
  int start = 0;
  int end = length - 1;

  while(start < end) {
    char c = str[start];
    str[start] = str[end];
    str[end] = c;
    start++;
    end--;
  }
}

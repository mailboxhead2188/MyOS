#define NULL 0

int strlen(const char *str);

int strcmp(const char *str1, const char *str2) {
  if ((str1 == NULL) && (str2 != NULL)) {
    return -1;
  }

  if ((str2 == NULL) && (str1 != NULL)) {
    return 1;
  }

  if ((str1 == NULL) && (str2 == NULL)) {
    return 0;
  }

  int len1 = strlen(str1);
  int len2 = strlen(str2);
  
  if (len1 < len2) {
    return (len2 - len1);
  }

  if (len2 < len1) {
    return (len1 - len2);
  }

  int count = 0;
  while (str1[count] != '\0') {
    if (str1[count] < str2[count]) return -1;

    if (str2[count] < str1[count]) return 1;
  }

  return 0;
}

int strlen(const char *str) {
  if (str == NULL) return 0;

  int count = 0;

  while (str[count] != '\0') {
    count++;
  }

  return count;
}

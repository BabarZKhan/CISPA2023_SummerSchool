#ifndef UTILS_H
#define UTILS_H


int min(int* arr, size_t n) {
  int min_ele = INT_MAX;
  for (size_t i = 0; i < n; i++) {
    if (arr[i] < min_ele) {
      min_ele = arr[i];
    }
  }
  return min_ele;
}

char* strip_trailing_whitespace(char* str) {
  size_t len = strlen(str);
  for (int pos = len -1; pos >= 0; pos--) {
    if (isspace(str[pos])) {
      // just set a nullbyte
      str[pos] = '\0';
    } else {
      // if smth is not a whitespace we are done
      break;
    }
  }
  return str;
}

uint64_t lookup_kallsym(const char* symbol_name) {
  // requires root privilege
  FILE* fd = fopen("/proc/kallsyms", "r");
  if (fd == 0) {
    fprintf(stderr, "Could not open /proc/kallsyms. Aborting!\n");
    exit(1);
  }

  int bytes_read;
  size_t len = 100;
  char* line = malloc(100);
  int line_no = 0;
  while ((bytes_read = getline(&line, &len, fd)) != -1) {
    // lines look like this: <addr> <type> <symbolname> <...>
    // example: 0000000000000000 T acpi_wmi_init        [wmi]

    char* line_splitted = strtok(line, " ");
    if (line_splitted == NULL) continue;

    long long addr = strtoull(line_splitted, NULL, 16);
    //printf("addr: 0x%llx\n", addr);

    line_splitted = strtok(NULL, " ");  // skip over type
    if (line_splitted == NULL) continue;

    line_splitted = strtok(NULL, " ");  // get symbolname
    if (line_splitted == NULL) continue;

    char* kallsym = strip_trailing_whitespace(line_splitted);
    if (line_splitted == NULL) continue;

    if (strcmp(kallsym, symbol_name) == 0) {
      free(line);
      fclose(fd);
      if (addr == 0) {
        fprintf(stderr, "You need root privileges for the /proc/kallsyms interface! Aborting!\n");
        exit(1);
      }
      return addr;
    }
    if (line_no > 400) break;
  }
  free(line);
  fclose(fd);
  return 0;
}

#endif /* !UTILS_H */

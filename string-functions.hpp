#pragma once

template<typename Type1, typename Type2>
char *concatinate(Type1 to, Type2 from) {
  while (*to) {
    to++;
  }
  while (*to++ = *from++);
  return --to;
}

template <typename Type1, typename Type2>
bool compare(Type1 a, Type2 b) {
  while (*a && *a == *b) {
    a++;
    b++;
  }
  return (!*a && !*b);
}

template<typename Type>
int getSize(Type string) {
  int size = 0;
  while (*string++) {
    size++;
  }
  return size;
}

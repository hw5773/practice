#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv)
{
  int fd;
  char buf[BUFSIZE] = {0, };
  ssize_t sz;

  printf("Hello, World!\n");

  fd = open("./test.c", O_RDONLY);
  sz = read(fd, buf, BUFSIZE-1);
  buf[sz] = '\0';
  puts(buf);

  return 0;
}

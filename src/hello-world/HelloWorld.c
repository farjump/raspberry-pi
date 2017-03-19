#include <stdlib.h>
#include <stdio.h>

int main(void)
{
  // String prompt and show the terminal cursor
  printf("Enter a string: \e[?25h");
  fflush(stdout);

  // Read a string
  char buffer[20];
  if (scanf("%19s", &buffer) == 1)
  {
    // Say Hi
    printf("RPi says \"Hello %s!\"\n", buffer);
  }
}

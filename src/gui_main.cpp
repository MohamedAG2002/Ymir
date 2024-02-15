#include "gui.h"

#include <cstdio>

int main()
{
  if(!gui_init())
  {
    printf("ERROR: Failed to create a GUI\n");
    return 1;
  }
  
  gui_run();
  gui_shutdown();
}

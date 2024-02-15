#include "project.h"

#include <cstring>

int main(int argc, char** argv)
{
  if(strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-H") == 0)
  {
    project_show_cli_help(); 
    return 0;
  }

  project_create_cli(argc, argv);
  return 0;
}

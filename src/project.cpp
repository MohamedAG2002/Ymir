#include "project.h"

#include <cstdio>
#include <string>
#include <fstream>
#include <filesystem>

// Private functions
///////////////////////////////////////////////////////////////////
static void construct_cmake_file(project_desc_t& proj)
{
  std::fstream file((proj.path + proj.name + '/') + "CMakeLists.txt", std::ios::out);
  if(!file.is_open())
    printf("ERROR: Could not create CMakeLists file in directory \'%s\'", (proj.path + proj.name + '/').c_str());

  std::string source_file, proj_type;

  // Inserting the correct CMake command dpending on project type 
  if(proj.type == PROJ_TYPE_LIB)
    proj_type = "add_library(${PROJECT_NAME} ${PROJ_SOURCES})\n";
  else 
    proj_type = "add_executable(${PROJECT_NAME} ${PROJ_SOURCES})\n";
 
  // Creating the CMake file
  source_file += "cmake_minimum_required(VERSION 3.27)\n";
  source_file += "project(" + proj.name + ")\n\n";
  source_file += "set(PROJ_SOURCES src/main.cpp)\n\n";
  source_file += proj_type;
  source_file += "\ntarget_compile_definitions(${PROJECT_NAME} PRIVATE " + proj.compiler_defines + ")\n";
  source_file += "target_compile_options(${PROJECT_NAME} PRIVATE " + proj.compiler_flags + ")\n";
  source_file += "target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_" + std::to_string(proj.cpp_version) + ")\n";

  file << source_file;

  file.close();
}

static void construct_basic_cpp(project_desc_t& proj)
{
  std::fstream file((proj.path + proj.name + '/') + proj.src_folder + '/' + "main.cpp", std::ios::out);
  if(!file.is_open())
    printf("ERROR: Could not create cpp file in directory \'%s\'", proj.src_folder.c_str());

  std::string source_cpp = "#include <iostream>\n\nint main() {\n   std::cout << \"Hello, world\\n\"; \n}";
  file << source_cpp;

  file.close();
}
  
static void construct_shell_scripts(project_desc_t& proj)
{
  std::filesystem::create_directory((proj.path + proj.name + '/') + "scripts");
  std::fstream file((proj.path + proj.name + '/') + "scripts/" + "build_and_run.sh", std::ios::out);
  if(!file.is_open())
    printf("ERROR: Could not create cpp file in directory \'%s\'", proj.src_folder.c_str());

  std::string shell_source;
  shell_source += "cd ../build/\n"; 
  shell_source += "cmake ..\n"; 
  shell_source += "make && ./" + proj.name + '\n'; 
  shell_source += "cd ..\n"; 

  file << shell_source;

  file.close();
}

static void check_commands(int index, char** argv, project_desc_t& desc) 
{
  int curr_arg_count = index;
  std::string curr_arg = argv[curr_arg_count];

  if(curr_arg == "--src" || curr_arg == "-S")
  {
    // Move to the actual value after the argument
    curr_arg = argv[curr_arg_count + 1];  
   
    // Set the values... finally
    desc.src_folder = curr_arg;
    desc.header_folder = curr_arg;
   
    // No need to check any extra commands for this round
    return;
  }
  if(curr_arg == "--header" || curr_arg == "-H")
  {
    curr_arg = argv[curr_arg_count + 1];
    desc.header_folder = curr_arg;
    
    return;
  }
  if(curr_arg == "--deps" || curr_arg == "-L")
  {
    curr_arg = argv[curr_arg_count + 1];
    desc.deps_folder = curr_arg;
    
    return;
  }
  if(curr_arg == "--scripts" || curr_arg == "-S")
  {
    curr_arg = argv[curr_arg_count + 1];
    desc.has_scripts = std::stoi(curr_arg);
    
    return;
  }
  if(curr_arg == "--cpp" || curr_arg == "-V")
  {
    curr_arg = argv[curr_arg_count + 1];
    desc.cpp_version = std::stoi(curr_arg);
    
    return;
  }
  if(curr_arg == "--flags" || curr_arg == "-F")
  {
    curr_arg = argv[curr_arg_count + 1];
    desc.compiler_flags = curr_arg;
    
    return;
  }
  if(curr_arg == "--defs" || curr_arg == "-D")
  {
    curr_arg = argv[curr_arg_count + 1];
    desc.compiler_defines = curr_arg;
    
    return;
  }
  if(curr_arg == "--type" || curr_arg == "-T")
  {
    curr_arg = argv[curr_arg_count + 1];
    project_type_e value; 

    if(curr_arg == "PROJ_TYPE_LIB")
      value = PROJ_TYPE_LIB;
    else if(curr_arg == "PROJ_TYPE_EXEC")
      value = PROJ_TYPE_EXEC;
    else 
    {
      printf("ERROR: %s is not a valid project type\n", curr_arg.c_str());
      return;
    }
  
    desc.type = value;
    return;
  }
  else 
  {
    printf("ERROR: \'%s\' is an unknown command\n", argv[curr_arg_count]);
    project_show_cli_help();
    return;
  }
}
///////////////////////////////////////////////////////////////////

// Project functions
///////////////////////////////////////////////////////////////////
void project_show_cli_help()
{
  printf("\n+++++++++++++++++++++++++++++++++++++++++++\n");
  printf("Welcome to YMIR - The C++ project generator\n");
  printf("+++++++++++++++++++++++++++++++++++++++++++\n\n");

  printf("Basic use: ymir some/path/ proj_name\n\n");

  printf("Extra commands: \n");
  printf("  --src     -S = Folder to hold all of the .cpp files (default = src)\n");
  printf("  --header  -H = Folder to hold all of  the .h/.hpp files (default = src)\n");
  printf("  --deps    -L = Folder to hold all external dependencies (default = libs)\n");
  printf("  --scripts    = Should this project have utility scripts? (1 = yes, 0 = no)\n");
  printf("  --cpp     -V = C++ version this project will use (default = 17)\n");
  printf("  --flags   -F = Extra compiler flags (NOTE: put the falgs inside \"\")\n");
  printf("  --defs    -D = Extra project definitions (NOTE: put the defs inside \"\")\n");
  printf("  --type    -T = Type of this project (PROJ_TYPE_EXEC or PROJE_TYPE_LIB; default = PROJ_TYPE_EXEC)\n");
}

void project_create(project_desc_t& proj)
{
  std::string full_path = (proj.path + proj.name + '/');

  // Create project directory
  printf("Creating the root directory...\n"); 
  std::filesystem::create_directory(full_path);

  // Create some important directories
  printf("Creating a src folder...\n");
  std::filesystem::create_directory(full_path + proj.src_folder);

  // The header files can either be in the src folder or 
  // in their own folder
  if(proj.header_folder != proj.src_folder)
  {
    printf("Creating a header folder...\n");
    std::filesystem::create_directory(full_path + proj.header_folder);
  }
  
  printf("Creating a dependencies folder...\n");
  std::filesystem::create_directory(full_path + proj.deps_folder);
  
  printf("Creating a build folder...\n");
  std::filesystem::create_directory(full_path + "build");

  // Creating the CMakeLists.txt file 
  printf("Configuring CMake...\n");
  construct_cmake_file(proj);

  // Creating a basic C++ file
  printf("Writing a simple C++ file for you...\n");
  construct_basic_cpp(proj);

  // Creating some useful shell scripts... if allowed
  if(proj.has_scripts)
  {
    printf("Writing some useful scripts...\n");
    construct_shell_scripts(proj);
  }
}

void project_create_cli(int argc, char** argv)
{
  if(argc < 3)
  {
    printf("ERROR: Not enough arguments given\n");
    project_show_cli_help();
  
    return;
  }

  // Giving the project default values
  project_desc_t desc;
  desc.path = argv[1];
  desc.name = argv[2];
  desc.src_folder = "src";
  desc.header_folder = desc.src_folder;
  desc.deps_folder = "libs";
  desc.has_scripts = true; 
  desc.cpp_version = 20;
  desc.compiler_flags = "-lm -Wall";
  desc.compiler_defines = "NO_DEFS";
  desc.type = PROJ_TYPE_EXEC;

  // Go through every argument and value pair if there are any
  for(int i = 3; i < argc; i += 2)
    check_commands(i, argv, desc);

  project_create(desc);
}
///////////////////////////////////////////////////////////////////


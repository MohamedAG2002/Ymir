#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

// Project type enum
///////////////////////////////////////////////////////////////////
enum project_type_e 
{
  PROJ_TYPE_LIB  = 0, 
  PROJ_TYPE_EXEC = 1
};
///////////////////////////////////////////////////////////////////

// Project desc struct
///////////////////////////////////////////////////////////////////
struct project_desc_t 
{
  std::string path, name;  
  std::string src_folder, header_folder, deps_folder; 
  bool has_scripts;
  int cpp_version;
  std::string compiler_flags, compiler_defines;
  project_type_e type;
};
///////////////////////////////////////////////////////////////////

// Project struct
///////////////////////////////////////////////////////////////////
struct project_t 
{
  std::string path, name;  
  std::string src_folder, header_folder, deps_folder; 
  bool has_scripts;
  int cpp_version;
  std::string compiler_flags, compiler_defines;
  project_type_e type;
  std::string full_path;

  std::vector<std::string> extra_folders;
};
///////////////////////////////////////////////////////////////////

// Project functions
///////////////////////////////////////////////////////////////////
void project_show_cli_help();
project_t project_create(const project_desc_t& desc);
project_t project_create_cli(int argc, char** argv);
void project_add_dir(project_t& proj, const std::string& dir_name);
///////////////////////////////////////////////////////////////////


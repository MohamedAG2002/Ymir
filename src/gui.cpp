#include "gui.h"
#include "project.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_opengl3_loader.h>

#include <cstdio>

// Globals
////////////////////////////////////////////////////
static GLFWwindow* s_window = nullptr;
static const int WINDOW_WIDTH = 1280; 
static const int WINDOW_HEIGHT = 720;
////////////////////////////////////////////////////

// Callbacks
////////////////////////////////////////////////////
void err_callback(int err_code, const char* desc)
{
  printf("GLFW-ERROR: %d - %s\n", err_code, desc);
}
////////////////////////////////////////////////////

// Private functions
////////////////////////////////////////////////////
static bool create_gl_context(int window_width, int window_height)
{
  if(!gladLoadGL(glfwGetProcAddress))
  {
    printf("ERROR: Failed to create a GL context\n");
    return false;
  }
  
  glad_glViewport(0, 0, window_width, window_height);
  glfwMakeContextCurrent(s_window);

  return true;
}

static bool create_window(int window_width, int window_height, const char* title)
{
  // GLFW init 
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  // Setting callbacks 
  glfwSetErrorCallback(err_callback);

  // Creating the window 
  s_window = glfwCreateWindow(window_width, window_height, title, nullptr, nullptr);
  if(!s_window)
  {
    printf("ERROR: Failed to create a GLFW window\n");
    return false;
  }
  
  glfwMakeContextCurrent(s_window);

  return true;
}

static bool init_imgui()
{
  // Set up ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO io = ImGui::GetIO(); (void)io;

  // Setting context flags
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.Fonts->AddFontFromFileTTF("assets/JetBrainsMonoNerd-bold.ttf", 16);

  // Dark mode WOOOOOOOAH! 
  ImGui::StyleColorsDark();

  // Setting up the glfw backend
  if(!ImGui_ImplGlfw_InitForOpenGL(s_window, true))
  {
    printf("ERROR: Failed to create ImGui GLFW backend\n");
    return false;
  }
  
  // Setting up the opengl backend
  if(!ImGui_ImplOpenGL3_Init("#version 460 core"))
  {
    printf("ERROR: Failed to create ImGui OpenGL backend\n");
    return false;
  }

  return true;
}

// MY EYES!!!! OH MY EYES!!!! WHAT A HORRIBLE THING TO WITNESS!!!!!
//////////////////////////////////////////////////////////
static char path[128]       = "/absolute/path/to/project/";
static char name[64]        = "ProjectName";
static char src_dir[24]     = "src";
static char header_dir[24]  = "src";
static char deps_dir[32]    = "libs";
static bool has_scripts     = true;
static int  cpp_version     = 20;
static char comp_flags[128] = "-Wall -lm";
static char comp_defs[128]  = "NO_DEFS";

static const char* types[2] = {"Executable", "Library"};
static int current_type = 0;
  
// Setting window flags 
static ImGuiWindowFlags win_flags = 0;
//////////////////////////////////////////////////////////

static void project_setting_panel(project_desc_t& desc)
{
  win_flags |= ImGuiWindowFlags_NoResize;
  win_flags |= ImGuiWindowFlags_NoMove;
  win_flags |= ImGuiWindowFlags_NoTitleBar; 
  win_flags |= ImGuiWindowFlags_NoCollapse; 
  
  ImGui::Begin("YMIR", nullptr, win_flags);

  ImGui::SeparatorText("Project Settings");
  ImGui::InputText("Path", path, 128);
  ImGui::InputText("Name", name, 64);
  ImGui::InputText("Source Folder", src_dir, 24);
  ImGui::InputText("Header Folder", header_dir, 24);
  ImGui::InputText("External Libraries Folder", deps_dir, 32);
  ImGui::Combo("Project Type", &current_type, types, 2);

  ImGui::SeparatorText("\nCompiler Settings");
  ImGui::InputText("Flags", comp_flags, 128);
  ImGui::InputText("Definitions", comp_defs, 128);
  ImGui::SliderInt("C++ Version", &cpp_version, 11, 23);
  
  ImGui::SeparatorText("\nExtra Settings");
  ImGui::Checkbox("Utility Scripts", &has_scripts);
    
  // @TODO: Do other things when the project has been created
  ImGui::Text("\n"); 
  if(ImGui::Button("Create"))
  {
    desc.path = path;
    desc.name = name;
    desc.src_folder = src_dir;
    desc.header_folder = header_dir;
    desc.deps_folder = deps_dir; 
    desc.has_scripts = has_scripts; 
    desc.cpp_version = cpp_version; 
    desc.compiler_flags = comp_flags;
    desc.compiler_defines = comp_defs; 
    desc.type = (project_type_e)current_type;
    
    project_create(desc);
  }
  
  ImGui::End();
}
////////////////////////////////////////////////////

// GUI functions
////////////////////////////////////////////////////
bool gui_init()
{
  // Window init 
  if(!create_window(WINDOW_WIDTH, WINDOW_HEIGHT, "Ymir GUI"))
    return false;
  
  // GLAD init 
  if(!create_gl_context(WINDOW_WIDTH, WINDOW_HEIGHT))
    return false;

  // ImGui init 
  if(!init_imgui())
    return false;

  return true;
}

void gui_shutdown()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  
  glfwDestroyWindow(s_window);
  glfwTerminate();
}

void gui_begin()
{
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void gui_end()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui_run()
{
  project_desc_t desc;
  while(!glfwWindowShouldClose(s_window))
  {
    gui_begin();
    project_setting_panel(desc);
    gui_end();

    glfwPollEvents();
    glfwSwapBuffers(s_window);
  }
}
////////////////////////////////////////////////////

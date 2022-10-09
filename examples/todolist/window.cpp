#include "window.hpp"
#include <cstring>
#include <iostream>
#include <string.h>

std::vector<const char *, std::allocator<const char *>> items;
char * added[15];
int indexForAdd;
static int selectedItem = 0; 


void Window::onCreate() {
  items = {"Go to the market", "Create UI with ImGui app until sunday"};
  indexForAdd = 0;
  
}
  char addList[256] = "";
  std::vector<std::string> addedItems = { };

  void Window::onPaintUI() {
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};


  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    // Window begin
    ImGui::Begin("Todo list!", nullptr, flags);

    // Menu + clear bar
    {
      bool restartSelected{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Tasks")) {
          ImGui::MenuItem("Clear all", nullptr, &restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) {
            std::vector<const char *, std::allocator<const char *>>().swap(items);
            for(int i=0; i<IM_ARRAYSIZE(added); i++) {
              delete added[i]; 
            }
            indexForAdd = 0;
      }
    }

    ImGui::Spacing(); 

    static char buf[256] = "";
    {
      if( ImGui::BeginListBox("")){
       
        for ( int i=0; i < items.size(); ++i) {
            const bool isSelected = (selectedItem == i);

            if (IM_ARRAYSIZE(items[i]) > 0 && ImGui::Selectable(items[i], isSelected)) {
                selectedItem = i;
            }
        
          if (isSelected) {
              ImGui::SetItemDefaultFocus();
          }
        }
        ImGui::EndListBox();
      }

      ImGui::InputText("", buf, 256);
      ImGui::SameLine();
      ImGui::Button("Create");
      if (ImGui::IsItemClicked()) {
        added[indexForAdd] = (char*) malloc(IM_ARRAYSIZE(buf));
        strcpy( added[indexForAdd], buf);
        items.insert(items.end(), added[indexForAdd]);
        indexForAdd++;
        for(int i=0; i< IM_ARRAYSIZE(buf); i++) {
           buf[i] = '\0';
        }
      }
    }


    ImGui::End();
  }
}
#include "window.hpp"

#include "imfilebrowser.h"

void Window::onEvent(SDL_Event const &event) {
      if (event.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w){
        m_gameData.m_input.set(gsl::narrow<size_t>(Input::Up));}
      if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
        m_gameData.m_input.set(gsl::narrow<size_t>(Input::Down));}
      if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a){
        m_gameData.m_input.set(gsl::narrow<size_t>(Input::Left));}
      if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d){
        m_gameData.m_input.set(gsl::narrow<size_t>(Input::Right));}
  }

  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w){
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Up));}
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s){
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Down));}
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a){
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Left));}
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d) {
      m_gameData.m_input.reset(gsl::narrow<size_t>(Input::Right));
      }
  }


}
void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);
  abcg::glEnable(GL_DEPTH_TEST);

  auto const path{assetsPath + "shaders/" + "cuberefract"};
    auto const program{abcg::createOpenGLProgram(
       {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
        {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}})};

    auto const pathFood{assetsPath + "shaders/" + "normal"};
    auto const foodProgram{abcg::createOpenGLProgram(
       {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
        {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}})};

  m_program = program;
  m_food_program = foodProgram;
  // Load default model
  loadModel(assetsPath + "box.obj");

  loadFoodModel();

  createSkybox();
}

void Window::loadModel(std::string_view path) {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  m_model.destroy();

  m_model.loadDiffuseTexture(assetsPath + "maps/pattern.png");
  m_model.loadNormalTexture(assetsPath + "maps/pattern_normal.png");
  m_model.loadCubeTexture(assetsPath + "maps/cube/");
  m_model.loadObj(path);
  m_model.setupVAO(m_program);
  //m_model.setupVAO(m_programs.at(m_currentProgramIndex));
  m_trianglesToDraw = m_model.getNumTriangles();

  // Use material prosties from the loaded model
  m_Ka = m_model.getKa();
  m_Kd = m_model.getKd();
  m_Ks = m_model.getKs();
  m_shininess = m_model.getShininess();
}

void Window::loadFoodModel() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  //trocar por m_food
  m_food.destroy();

  m_food.loadDiffuseTexture(assetsPath + "maps/pattern.png");
  m_food.loadNormalTexture(assetsPath + "maps/pattern_normal.png");
  m_food.loadCubeTexture(assetsPath + "maps/cube/");
  m_food.loadObj(assetsPath+"box.obj");
  m_food.setupVAO(m_food_program);

  m_FOODtrianglesToDraw = m_food.getNumTriangles();

  // Use material prosties from the loaded model
  m_FoodKa = m_food.getKa();
  m_FoodKd = m_food.getKd();
  m_FoodKs = m_food.getKs();
  m_Foodshininess = m_food.getShininess();
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  // Use currently selected program
  //auto const program{m_program};
  //auto const program{m_programs.at(m_currentProgramIndex)};
  abcg::glUseProgram(m_program);

  // Get location of uniform variables
  auto const viewMatrixLoc{abcg::glGetUniformLocation(m_program, "viewMatrix")};
  auto const projMatrixLoc{abcg::glGetUniformLocation(m_program, "projMatrix")};
  // auto const normalMatrixLoc{
  //     abcg::glGetUniformLocation(program, "normalMatrix")};
  auto const lightDirLoc{
      abcg::glGetUniformLocation(m_program, "lightDirWorldSpace")};
  auto const shininessLoc{abcg::glGetUniformLocation(m_program, "shininess")};
  auto const IaLoc{abcg::glGetUniformLocation(m_program, "Ia")};
  auto const IdLoc{abcg::glGetUniformLocation(m_program, "Id")};
  auto const IsLoc{abcg::glGetUniformLocation(m_program, "Is")};
  auto const KaLoc{abcg::glGetUniformLocation(m_program, "Ka")};
  auto const KdLoc{abcg::glGetUniformLocation(m_program, "Kd")};
  auto const KsLoc{abcg::glGetUniformLocation(m_program, "Ks")};
  auto const diffuseTexLoc{abcg::glGetUniformLocation(m_program, "diffuseTex")};
  auto const normalTexLoc{abcg::glGetUniformLocation(m_program, "normalTex")};
  auto const cubeTexLoc{abcg::glGetUniformLocation(m_program, "cubeTex")};
  auto const mappingModeLoc{abcg::glGetUniformLocation(m_program, "mappingMode")};
  auto const texMatrixLoc{abcg::glGetUniformLocation(m_program, "texMatrix")};

  // Set uniform variables that have the same value for every model
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(diffuseTexLoc, 0);
  abcg::glUniform1i(normalTexLoc, 1);
  abcg::glUniform1i(cubeTexLoc, 2);
  abcg::glUniform1i(mappingModeLoc, m_mappingMode);
  glm::mat3 const texMatrix{m_rotation};

  abcg::glUniformMatrix3fv(texMatrixLoc, 1, GL_TRUE, &texMatrix[0][0]);

  auto const lightDirRotated{m_rotation * m_lightDir};
  abcg::glUniform4fv(lightDirLoc, 1, &lightDirRotated.x);
  abcg::glUniform4fv(IaLoc, 1, &m_Ia.x);
  abcg::glUniform4fv(IdLoc, 1, &m_Id.x);
  abcg::glUniform4fv(IsLoc, 1, &m_Is.x);

  // Set uniform variables for the current model

  abcg::glUniform4fv(KaLoc, 1, &m_Ka.x);
  abcg::glUniform4fv(KdLoc, 1, &m_Kd.x);
  abcg::glUniform4fv(KsLoc, 1, &m_Ks.x);
  abcg::glUniform1f(shininessLoc, m_shininess);

  
  //abcg::glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, &m_modelMatrix[0][0]);


  m_model.render(m_trianglesToDraw);


  abcg::glUseProgram(0);


/////

//   abcg::glUseProgram(m_food_program);

//   // Get location of uniform variables
//   auto const viewFoodMatrixLoc{abcg::glGetUniformLocation(m_food_program, "viewMatrix")};
//   auto const projFoodMatrixLoc{abcg::glGetUniformLocation(m_food_program, "projMatrix")};
//   auto const lightFoodDirLoc{
//       abcg::glGetUniformLocation(m_food_program, "lightDirWorldSpace")};
//   auto const shininessFoodLoc{abcg::glGetUniformLocation(m_food_program, "shininess")};
//   auto const IaFoodLoc{abcg::glGetUniformLocation(m_food_program, "Ia")};
//   auto const IdFoodLoc{abcg::glGetUniformLocation(m_food_program, "Id")};
//   auto const IsFoodLoc{abcg::glGetUniformLocation(m_food_program, "Is")};
//   auto const KaFoodLoc{abcg::glGetUniformLocation(m_food_program, "Ka")};
//   auto const KdFoodLoc{abcg::glGetUniformLocation(m_food_program, "Kd")};
//   auto const KsFoodLoc{abcg::glGetUniformLocation(m_food_program, "Ks")};
//   auto const diffuseFoodTexLoc{abcg::glGetUniformLocation(m_food_program, "diffuseTex")};
//   auto const normalFoodTexLoc{abcg::glGetUniformLocation(m_food_program, "normalTex")};
//   auto const cubeFoodTexLoc{abcg::glGetUniformLocation(m_food_program, "cubeTex")};
//   auto const mappingFoodModeLoc{abcg::glGetUniformLocation(m_food_program, "mappingMode")};
//   auto const texFoodMatrixLoc{abcg::glGetUniformLocation(m_food_program, "texMatrix")};

//   // Set uniform variables that have the same value for every model
//   abcg::glUniformMatrix4fv(viewFoodMatrixLoc, 1, GL_FALSE, &m_viewMatrix[0][0]);
//   abcg::glUniformMatrix4fv(projFoodMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
//   abcg::glUniform1i(diffuseFoodTexLoc, 0);
//   abcg::glUniform1i(normalFoodTexLoc, 1);
//   abcg::glUniform1i(cubeFoodTexLoc, 2);
//   abcg::glUniform1i(mappingFoodModeLoc, m_mappingMode);

//   abcg::glUniformMatrix3fv(texFoodMatrixLoc, 1, GL_TRUE, &texMatrix[0][0]);

//   auto const lightFoodDirRotated{m_rotation * m_lightDir};
//   abcg::glUniform4fv(lightFoodDirLoc, 1, &lightFoodDirRotated.x);
//   abcg::glUniform4fv(IaFoodLoc, 1, &m_Ia.x);
//   abcg::glUniform4fv(IdFoodLoc, 1, &m_Id.x);
//   abcg::glUniform4fv(IsFoodLoc, 1, &m_Is.x);

//   // Set uniform variables for the current model

//   abcg::glUniform4fv(KaFoodLoc, 1, &m_FoodKa.x);
//   abcg::glUniform4fv(KdFoodLoc, 1, &m_FoodKd.x);
//   abcg::glUniform4fv(KsFoodLoc, 1, &m_FoodKs.x);
//   abcg::glUniform1f(shininessFoodLoc, m_shininess);

//   m_food.renderFood(m_FOODtrianglesToDraw);

//   abcg::glUseProgram(0);


// /////

  if (m_currentProgramIndex == 0 || m_currentProgramIndex == 1) {
    renderSkybox();
  }
}

void Window::onUpdate() {
  m_modelMatrix = m_rotation;
   
  m_viewMatrix =
      glm::lookAt(glm::vec3(0.0f, 7.5f, 7.5f),
                  glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  m_model.onUpdate(m_gameData);
  m_food.onUpdate(m_gameData);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  auto const scaledWidth{gsl::narrow_cast<int>(m_viewportSize.x * 0.8f)};
  auto const scaledHeight{gsl::narrow_cast<int>(m_viewportSize.y * 0.8f)};

  // File browser for models
  static ImGui::FileBrowser fileDialogModel;
  fileDialogModel.SetTitle("Load 3D Model");
  fileDialogModel.SetTypeFilters({".obj"});
  fileDialogModel.SetWindowSize(scaledWidth, scaledHeight);

  // File browser for textures
  static ImGui::FileBrowser fileDialogDiffuseMap;
  fileDialogDiffuseMap.SetTitle("Load Diffuse Map");
  fileDialogDiffuseMap.SetTypeFilters({".jpg", ".png"});
  fileDialogDiffuseMap.SetWindowSize(scaledWidth, scaledHeight);

  // File browser for normal maps
  static ImGui::FileBrowser fileDialogNormalMap;
  fileDialogNormalMap.SetTitle("Load Normal Map");
  fileDialogNormalMap.SetTypeFilters({".jpg", ".png"});
  fileDialogNormalMap.SetWindowSize(scaledWidth, scaledHeight);

#if defined(__EMSCRIPTEN__)
  auto const assetsPath{abcg::Application::getAssetsPath()};
  fileDialogModel.SetPwd(assetsPath);
  fileDialogDiffuseMap.SetPwd(assetsPath + "/maps");
  fileDialogNormalMap.SetPwd(assetsPath + "/maps");
#endif

  // Create main window widget
  {
    auto widgetSize{ImVec2(222, 190)};

    if (!m_model.isUVMapped()) {
      // Add extra space for static text
      widgetSize.y += 26;
    }

    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5, 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin("Widget window", nullptr,
                 ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDecoration);

    // Menu
    {
      bool loadModel{};
      bool loadDiffuseMap{};
      bool loadNormalMap{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
          ImGui::MenuItem("Load 3D Model...", nullptr, &loadModel);
          ImGui::MenuItem("Load Diffuse Map...", nullptr, &loadDiffuseMap);
          ImGui::MenuItem("Load Normal Map...", nullptr, &loadNormalMap);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (loadModel)
        fileDialogModel.Open();
      if (loadDiffuseMap)
        fileDialogDiffuseMap.Open();
      if (loadNormalMap)
        fileDialogNormalMap.Open();
    }

    // Projection combo box
    {
      auto const aspect{gsl::narrow<float>(m_viewportSize.x) /
                        gsl::narrow<float>(m_viewportSize.y)};
      m_projMatrix = glm::perspective(glm::radians(120.0f), aspect, 0.5f, 1000.0f);
    }

    ImGui::End();
  }

  // Create window for light sources
  if (m_currentProgramIndex >= 2 && m_currentProgramIndex <= 6) {
    auto const widgetSize{ImVec2(222, 244)};
    ImGui::SetNextWindowPos(ImVec2(m_viewportSize.x - widgetSize.x - 5,
                                   m_viewportSize.y - widgetSize.y - 5));
    ImGui::SetNextWindowSize(widgetSize);
    ImGui::Begin(" ", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::Text("Light properties");

    // Slider to control light properties
    ImGui::PushItemWidth(widgetSize.x - 36);
    ImGui::ColorEdit3("Ia", &m_Ia.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Id", &m_Id.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Is", &m_Is.x, ImGuiColorEditFlags_Float);
    ImGui::PopItemWidth();

    ImGui::Spacing();

    ImGui::Text("Material properties");

    // Slider to control material properties
    ImGui::PushItemWidth(widgetSize.x - 36);
    ImGui::ColorEdit3("Ka", &m_Ka.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Kd", &m_Kd.x, ImGuiColorEditFlags_Float);
    ImGui::ColorEdit3("Ks", &m_Ks.x, ImGuiColorEditFlags_Float);
    ImGui::PopItemWidth();

    // Slider to control the specular shininess
    ImGui::PushItemWidth(widgetSize.x - 16);
    ImGui::SliderFloat(" ", &m_shininess, 0.0f, 500.0f, "shininess: %.1f");
    ImGui::PopItemWidth();

    ImGui::End();
  }

  fileDialogModel.Display();
  if (fileDialogModel.HasSelected()) {
    loadModel(fileDialogModel.GetSelected().string());
    fileDialogModel.ClearSelected();

    if (m_model.isUVMapped()) {
      // Use mesh texture coordinates if available...
      m_mappingMode = 3;
    } else {
      // ...or triplanar mapping otherwise
      m_mappingMode = 0;
    }
  }

  fileDialogDiffuseMap.Display();
  if (fileDialogDiffuseMap.HasSelected()) {
    m_model.loadDiffuseTexture(fileDialogDiffuseMap.GetSelected().string());
    fileDialogDiffuseMap.ClearSelected();
  }

  fileDialogNormalMap.Display();
  if (fileDialogNormalMap.HasSelected()) {
    m_model.loadNormalTexture(fileDialogNormalMap.GetSelected().string());
    fileDialogNormalMap.ClearSelected();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
}

void Window::onDestroy() {
  m_model.destroy();
  abcg::glDeleteProgram(m_program);
    abcg::glDeleteProgram(m_food_program);
  
}

void Window::createSkybox() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Create skybox program
  auto const path{assetsPath + "shaders/" + m_skyShaderName};
  m_skyProgram = abcg::createOpenGLProgram(
      {{.source = path + ".vert", .stage = abcg::ShaderStage::Vertex},
       {.source = path + ".frag", .stage = abcg::ShaderStage::Fragment}});

  // Generate VBO
  abcg::glGenBuffers(1, &m_skyVBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyPositions),
                     m_skyPositions.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_skyProgram, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_skyVAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_skyVAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::renderSkybox() {
  abcg::glUseProgram(m_skyProgram);

  auto const viewMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "viewMatrix")};
  auto const projMatrixLoc{
      abcg::glGetUniformLocation(m_skyProgram, "projMatrix")};
  auto const skyTexLoc{abcg::glGetUniformLocation(m_skyProgram, "skyTex")};

  auto const viewMatrix{m_rotation};
  abcg::glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, &viewMatrix[0][0]);
  abcg::glUniformMatrix4fv(projMatrixLoc, 1, GL_FALSE, &m_projMatrix[0][0]);
  abcg::glUniform1i(skyTexLoc, 0);

  abcg::glBindVertexArray(m_skyVAO);

  abcg::glActiveTexture(GL_TEXTURE0);
  abcg::glBindTexture(GL_TEXTURE_CUBE_MAP, m_model.getCubeTexture());

  abcg::glEnable(GL_CULL_FACE);
  abcg::glFrontFace(GL_CW);
  abcg::glDepthFunc(GL_LEQUAL);
  abcg::glDrawArrays(GL_TRIANGLES, 0, m_skyPositions.size());
  abcg::glDepthFunc(GL_LESS);

  abcg::glBindVertexArray(0);
  abcg::glUseProgram(0);
}

void Window::destroySkybox() const {
  abcg::glDeleteProgram(m_skyProgram);
  abcg::glDeleteBuffers(1, &m_skyVBO);
  abcg::glDeleteVertexArrays(1, &m_skyVAO);
}
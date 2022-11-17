#include "window.hpp"

#include <unordered_map>
#include <glm/gtc/random.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

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
  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  std::uniform_int_distribution randomFoodLocation(0, 16);

  auto const &assetsPath{abcg::Application::getAssetsPath()};

  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);


  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  m_ground.create(m_program);


  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");

  loadModelFromFile(assetsPath + "box.obj");  

  // SNAKE UNIT

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_vertices.at(0)) * m_vertices.size(),
                     m_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_indices.at(0)) * m_indices.size(),
                     m_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);


  m_snakes_positions.clear();
  
  m_snakes_positions.push_back(glm::vec2(8,7));
  m_snakes_positions.push_back(glm::vec2(7,7));
  m_snakes_positions.push_back(glm::vec2(6,7));
  
  // createFood();
}

void Window::loadModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_vertices.clear();
  m_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_vertices.size();
        // Add this vertex
        m_vertices.push_back(vertex);
      }

      m_indices.push_back(hash[vertex]);
    }
  }
}



void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_VAO);
  
  // x, z, y

  for ( auto const pos : m_snakes_positions) {
    
    glm::mat4 model{0.1f};
     model = glm::translate(model, glm::vec3(-8.0f+(pos.x), 0.5f, -8.0f+(pos.y)));

    abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
     //cor azul
    abcg::glUniform4f(m_colorLocation, 0.0f, 0.8f, 1.0f, 1.0f);

    abcg::glDrawElements(GL_TRIANGLES, m_indices.size(),   GL_UNSIGNED_INT, nullptr);

  }


    glm::mat4 model{0.1f};
    model = glm::translate(model, glm::vec3(-8.0f+(food_loc.x), 0.5f, -8.0f+(food_loc.y)));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1, 0, 1));
    model = glm::scale(model, glm::vec3(0.75f));
    // model = glm::rotate(model,m_angle, glm::sphericalRand(1.0f));
    
    abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
     //cor azul
    abcg::glUniform4f(m_colorLocation, 1.0f, 0.8f, 0.0f, 1.0f);

    abcg::glDrawElements(GL_TRIANGLES, m_indices.size(),   GL_UNSIGNED_INT, nullptr);
 

  abcg::glBindVertexArray(0);

  m_ground.paint();
  
  abcg::glUseProgram(0);
}

void Window::onPaintUI() { 
  abcg::OpenGLWindow::onPaintUI();
  {
    auto const size{ImVec2(875, 850)};
    auto const position{ImVec2((m_viewportSize.x - size.x)/2.0f,
                               (m_viewportSize.y- size.y)/2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flags{ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);

    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");

    }

    ImGui::PopFont();
    ImGui::End();
  }
 }

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}


void Window::createFood() {

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  std::uniform_real_distribution<float> realDistribution(-8.0f, 8.0f);

  auto const &assetsPath{abcg::Application::getAssetsPath()};

  loadAppleModelFromFile(assetsPath + "Apple.obj");
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER,
                     sizeof(m_apple_vertices.at(0)) * m_apple_vertices.size(),
                     m_apple_vertices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(m_apple_indices.at(0)) * m_apple_indices.size(),
                     m_apple_indices.data(), GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Create VAO
  abcg::glGenVertexArrays(1, &m_apple_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_apple_VAO);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_apple_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::loadAppleModelFromFile(std::string_view path) {
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(path.data())) {
    if (!reader.Error().empty()) {
      throw abcg::RuntimeError(
          fmt::format("Failed to load model {} ({})", path, reader.Error()));
    }
    throw abcg::RuntimeError(fmt::format("Failed to load model {}", path));
  }

  if (!reader.Warning().empty()) {
    fmt::print("Warning: {}\n", reader.Warning());
  }

  auto const &attributes{reader.GetAttrib()};
  auto const &shapes{reader.GetShapes()};

  m_apple_vertices.clear();
  m_apple_indices.clear();

  // A key:value map with key=Vertex and value=index
  std::unordered_map<Vertex, GLuint> hash{};

  // Loop over shapes
  for (auto const &shape : shapes) {
    // Loop over indices
    for (auto const offset : iter::range(shape.mesh.indices.size())) {
      // Access to vertex
      auto const index{shape.mesh.indices.at(offset)};

      // Vertex position
      auto const startIndex{3 * index.vertex_index};
      auto const vx{attributes.vertices.at(startIndex + 0)};
      auto const vy{attributes.vertices.at(startIndex + 1)};
      auto const vz{attributes.vertices.at(startIndex + 2)};

      Vertex const vertex{.position = {vx, vy, vz}};

      // If map doesn't contain this vertex
      if (!hash.contains(vertex)) {
        // Add this index (size of m_vertices)
        hash[vertex] = m_apple_vertices.size();
        // Add this vertex
        m_apple_vertices.push_back(vertex);
      }

      m_apple_indices.push_back(hash[vertex]);
    }
  }
}

void Window::paintFood(){

   abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glBindVertexArray(m_apple_VAO);
  
    
    glm::mat4 model{0.1f};
     model = glm::translate(model, glm::vec3(-8.0f, 0.5f, -8.0f));

    abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
     //cor azul
    abcg::glUniform4f(m_colorLocation, 0.0f, 0.8f, 1.0f, 1.0f);

    abcg::glDrawElements(GL_TRIANGLES, m_indices.size(),   GL_UNSIGNED_INT, nullptr);
 

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Window::checkGameOver(){  
  glm::vec2 head = m_snakes_positions.back();
  if (head.x > 15.4f || head.x < 1 || head.y > 15.4f || head.y < 1 ) {
      m_gameData.m_state = State::GameOver;       
       m_snakes_positions.clear();
       food_loc = glm::vec2(-100, -100);
  } else {
    m_restartWaitTimer.restart(); 
  }
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};
  if(m_rotationTimer.elapsed() > 1) {
    m_rotationTimer.restart();
    m_angle = glm::wrapAngle(m_angle + glm::radians(90.0f));
  }

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
  checkGameOver();
  checkFoodHasEaten();

  if (m_gameData.m_state == State::GameOver &&
    m_restartWaitTimer.elapsed() > 5) {
    m_restartWaitTimer.restart();
    restartGame();
    return;
  }

  if (m_gameData.m_state == State::Playing && m_stepTimer.elapsed() > 0.2) {

    m_stepTimer.restart();
    if (m_lastInput == Input::Right) {
            glm::vec2 pos = m_snakes_positions.back();
            m_snakes_positions.push_back(glm::vec2(pos.x+0.5, pos.y));

            m_snakes_positions.pop_front();
    }

  if (m_lastInput == Input::Left) {
      glm::vec2 pos = m_snakes_positions.back();
      m_snakes_positions.push_back(glm::vec2(pos.x-0.5, pos.y));
            m_snakes_positions.pop_front();

        }

        if (m_lastInput == Input::Up) {

              glm::vec2 pos = m_snakes_positions.back();
            m_snakes_positions.push_back(glm::vec2(pos.x, pos.y-0.5));
            m_snakes_positions.pop_front();
        }

          if (m_lastInput == Input::Down) {
             glm::vec2 pos = m_snakes_positions.back();
            m_snakes_positions.push_back(glm::vec2(pos.x, pos.y+0.5));
            m_snakes_positions.pop_front();
        }
  }

  if (m_gameData.m_state == State::Playing ) {
  if (m_gameData.m_input[gsl::narrow<size_t>(Input::Right)]) {
            m_lastInput = Input::Right;
            return;
        }

  if (m_gameData.m_input[gsl::narrow<size_t>(Input::Left)]) {
            m_lastInput = Input::Left;
            return;
        }

  if (m_gameData.m_input[gsl::narrow<size_t>(Input::Up)]) {
                m_lastInput = Input::Up;
            return;
        }

          if (m_gameData.m_input[gsl::narrow<size_t>(Input::Down)]) {
                            m_lastInput = Input::Down;
            return;
        }
  }


}

void Window::restartGame() {
  m_gameData.m_state = State::Playing;
  
  m_snakes_positions.clear();
  
  m_snakes_positions.push_back(glm::vec2(8,7));
  m_snakes_positions.push_back(glm::vec2(7,7));
  m_snakes_positions.push_back(glm::vec2(6,7));

  food_loc = glm::vec2(4,13);
}


void Window::checkFoodHasEaten(){
  for (auto const &unit :  m_snakes_positions) {
    if (unit.x == food_loc.x && unit.y == food_loc.y) {
        std::uniform_int_distribution randomFoodLocation(1, 15);
        food_loc = glm::vec2(randomFoodLocation(m_randomEngine), randomFoodLocation(m_randomEngine));
        increaseSize();
      }
   }
}

void Window::increaseSize() {
    if (m_lastInput == Input::Right) {
        glm::vec2 pos = m_snakes_positions.front();
        m_snakes_positions.push_front(glm::vec2(pos.x-1, pos.y));
    }

    if (m_lastInput == Input::Left) {
        glm::vec2 pos = m_snakes_positions.front();
        m_snakes_positions.push_front(glm::vec2(pos.x+1, pos.y));
    }

    if (m_lastInput == Input::Up) {
        glm::vec2 pos = m_snakes_positions.front();
        m_snakes_positions.push_front(glm::vec2(pos.x, pos.y+1));
    }

    if (m_lastInput == Input::Down) {
        glm::vec2 pos = m_snakes_positions.front();
        m_snakes_positions.push_front(glm::vec2(pos.x, pos.y-1));
    }
}
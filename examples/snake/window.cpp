#include "window.hpp"
#include <string>
#include <stdio.h>

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

  // Load a new font
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  m_objectsProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});  
    
  abcg::glClearColor(0, 0, 0, 1);

  #if !defined(__EMSCRIPTEN__)
    abcg::glEnable(GL_PROGRAM_POINT_SIZE);
  #endif

  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::Playing;

  m_snake.create(m_objectsProgram);

 m_food.create(m_objectsProgram);
}

void Window::onUpdate() {
  checkGameOver();
  checkFoodHasEaten();

  if (m_gameData.m_state != State::Playing &&
  m_restartWaitTimer.elapsed() > 5) {
    m_restartWaitTimer.restart();
    restart();
    return;
  }
    m_snake.update(m_gameData);
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  m_snake.paint(m_gameData);
  m_food.paint(m_gameData);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const size{ImVec2(850, 675)};
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

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_snake.destroy();
  m_food.destroy();
}


void Window::checkGameOver(){  
  glm::vec2 head = m_snake.m_snakes_positions.back();

  if (head.x > 39 || head.x < 0 || head.y > 39 || head.y < 0 ) {
      m_gameData.m_state = State::GameOver;
      m_snake.destroy();
      m_food.destroy();
  }
   m_restartWaitTimer.restart();
}

void Window::checkFoodHasEaten(){
  glm::vec2 food = m_food.m_food_positions;
  for (auto const &unit :  m_snake.m_snakes_positions) {
    if (unit.x == food.x && unit.y == food.y) {
        m_food.destroy();
        m_snake.increaseSize();
        m_food.create(m_objectsProgram);
    }
   }

}
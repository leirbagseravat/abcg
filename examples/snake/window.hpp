#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "snake.hpp"
#include "food.hpp"

#include "gamedata.hpp"
#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void checkGameOver();
  void checkFoodHasEaten();


private:
  glm::ivec2 m_viewportSize{};

  GLuint m_objectsProgram{};

  GameData m_gameData;

  Snake m_snake;
  int snakeSize{2};
  Food m_food;


  abcg::Timer m_restartWaitTimer;


  std::default_random_engine m_randomEngine;

    ImFont *m_font{};


  void restart();

  GLuint m_VAO{};
  GLuint m_VBOVertices{};
  GLuint m_program{};

  std::array<glm::vec2, 3> const m_points{{{0, 1}, {-1, -1}, {1, -1}}};
  glm::vec2 m_P{};

  void setupModel();
};

#endif
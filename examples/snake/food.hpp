#ifndef FOOD_HPP_
#define FOOD_HPP_

#include "abcgOpenGL.hpp"
#include <random>
#include <list>

#include "gamedata.hpp"

class Food {
public:
  void create(GLuint program);
  void paint(GameData const &gameData);
  void destroy();
    float m_scale{1.0f};
      glm::vec4 m_color{1};

  glm::vec2 m_translation{};
  std::array<glm::vec2, 4> getRandomPositions(int x, int y);   
  glm::vec2 m_food_positions;

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};


  std::default_random_engine m_randomEngine;
};
#endif
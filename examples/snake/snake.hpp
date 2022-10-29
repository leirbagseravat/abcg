#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include "abcgOpenGL.hpp"
#include <list>
#include "gamedata.hpp"

class Snake {
public:
  void create(GLuint program);
  void paint(GameData const &gameData, int snakeSize);
  void destroy();
  void update(GameData const &gameData, float deltaTime);
  void checkGameOver(GameData &gameData);
  void increaseSize();


  Input m_lastInput{Input::Right}; 

  float step = 0.05f;


  abcg::Timer m_trailBlinkTimer;
  abcg::Timer m_bulletCoolDownTimer;
  abcg::Timer m_stepTimer;


 struct SnakeUnit {
    GLuint m_VAO{};
    GLuint m_VBO{};
    GLuint m_EBO{};

  glm::vec4 m_color{1};
  float m_scale{1.0f};
    glm::vec2 m_translation{};
  };

  std::list<SnakeUnit> m_snakes;

  std::list<glm::vec2> m_snakes_positions;

  SnakeUnit makeSnakeUnit(int x, int y);

  int m_grid[40][40];

private:

  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
};
#endif
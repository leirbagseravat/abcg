#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include <list>
#include <random>

#include "camera.hpp"
#include "ground.hpp"
#include "gamedata.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;
  void paintFood();
  void createFood();
  void loadAppleModel();

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


    int m_grid[17][17];
  float step = 0.05f;
  float rotation_step = 0.01f;
  Input m_lastInput{Input::Right}; 

private:
  glm::ivec2 m_viewportSize{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};

  GLuint m_apple_VAO{};
  GLuint m_apple_VBO{};
  GLuint m_apple_EBO{};

  
  GLuint m_program{};
    float m_angle{};


  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
  GameData m_gameData;

  abcg::Timer m_restartWaitTimer;
  abcg::Timer m_stepTimer;
  abcg::Timer m_rotationTimer;


  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};

  Ground m_ground;

  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  glm::vec2 food_loc = glm::vec2(4, 13);

  std::vector<Vertex> m_apple_vertices;
  std::vector<GLuint> m_apple_indices;
  ImFont *m_font{};
  std::default_random_engine m_randomEngine;

  // void destroyGame();
  void restartGame();
  void checkGameOver();
  void loadModelFromFile(std::string_view path);
  void loadAppleModelFromFile(std::string_view path);
  void increaseSize();
  void checkFoodHasEaten();
};

#endif
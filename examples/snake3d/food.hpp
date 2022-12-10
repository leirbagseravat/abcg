#ifndef FOOD_HPP_
#define FOOD_HPP_

#include "abcgOpenGL.hpp"
#include <random>
#include <list>

#include "gamedata.hpp"
#include "camera.hpp"

class Food {
public:
  void create(GLuint program);
  void paint(GameData const &gameData, Camera const &camera);
  void destroy();
  float m_scale{1.0f};
  glm::vec4 m_color{1};
  bool m_hasNormals{false};

  void computeNormals();

  glm::vec2 m_translation{};
  std::array<glm::vec2, 4> getRandomPositions(int x, int y);   
  glm::vec2 m_food_positions;

private:

  glm::ivec2 m_viewportSize{};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};

  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  Camera m_camera;

  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

    GLuint m_program{};


  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  void loadAppleModelFromFile(std::string_view path);


  std::default_random_engine m_randomEngine;
};
#endif
#ifndef MODEL_HPP_
#define MODEL_HPP_

#include "abcgOpenGL.hpp"
#include "gamedata.hpp"
#include <list>
#include <random>


struct Vertex {
  glm::vec3 position{};
  glm::vec3 normal{};
  glm::vec2 texCoord{};
  glm::vec4 tangent{};

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Model {
public:
  void loadCubeTexture(std::string const &path);
  void loadDiffuseTexture(std::string_view path);
  void loadNormalTexture(std::string_view path);
  void loadObj(std::string_view path, bool standardize = true);
  void render(int numTriangles = -1) const;
  //void renderFood(int numTriangles =-1) const;
  void setupVAO(GLuint program);
  void destroy();
  void onUpdate(GameData m_gameData);

  [[nodiscard]] int getNumTriangles() const {
    return gsl::narrow<int>(m_indices.size()) / 3;
  }

  [[nodiscard]] glm::vec4 getKa() const { return m_Ka; }
  [[nodiscard]] glm::vec4 getKd() const { return m_Kd; }
  [[nodiscard]] glm::vec4 getKs() const { return m_Ks; }
  [[nodiscard]] float getShininess() const { return m_shininess; }

  [[nodiscard]] bool isUVMapped() const { return m_hasTexCoords; }

  [[nodiscard]] GLuint getCubeTexture() const { return m_cubeTexture; }

private:
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GameData m_gameData;
  Input m_lastInput{Input::Right}; 
  std::list<glm::vec2> m_snakes_positions;
  glm::vec2 food_loc = glm::vec2(5, 13);

  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};
  GLuint m_diffuseTexture{};
  GLuint m_normalTexture{};
  GLuint m_cubeTexture{};
  GLuint m_modelMatrixLoc{};
    GLuint m_normalMatrixLoc{};

    abcg::Timer m_stepTimer;


  std::vector<Vertex> m_vertices;
  std::vector<GLuint> m_indices;
  std::default_random_engine m_randomEngine;

  bool m_hasNormals{false};
  bool m_hasTexCoords{false};

  void computeNormals();
  void computeTangents();
  void createBuffers();
  void standardize();
   void increaseSize();
  void checkFoodHasEaten();
};

#endif
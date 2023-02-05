#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include "model.hpp"
#include "gamedata.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;

private:
  glm::ivec2 m_viewportSize{};
  GameData m_gameData;

  Model m_model;
  Model m_food;
  int m_trianglesToDraw{};
  int m_FOODtrianglesToDraw{};

  float m_zoom{};

  glm::mat4 m_modelMatrix{1.0f};
  glm::mat4 m_viewMatrix{1.0f};
  glm::mat4 m_projMatrix{1.0f};
  glm::mat4 m_rotation{1.0f};

  GLuint m_program;
  GLuint m_food_program;
  int m_currentProgramIndex{};

  // Mapping mode
  // 0: triplanar; 1: cylindrical; 2: spherical; 3: from mesh
  int m_mappingMode{};

  // Light and material properties
  glm::vec4 m_lightDir{-1.0f, -1.0f, -1.0f, 0.0f};
  glm::vec4 m_Ia{1.0f};
  glm::vec4 m_Id{1.0f};
  glm::vec4 m_Is{1.0f};
  glm::vec4 m_Ka{};
  glm::vec4 m_Kd{};
  glm::vec4 m_Ks{};
  float m_shininess{};

    glm::vec4 m_FoodKa{};
  glm::vec4 m_FoodKd{};
  glm::vec4 m_FoodKs{};
  float m_Foodshininess{};

  // Skybox
  std::string const m_skyShaderName{"skybox"};
  GLuint m_skyVAO{};
  GLuint m_skyVBO{};
  GLuint m_skyProgram{};

  // clang-format off
  std::array<glm::vec3, 36> const m_skyPositions{{
      // Front
      {-1, -1, +1}, {+1, -1, +1}, {+1, +1, +1},
      {-1, -1, +1}, {+1, +1, +1}, {-1, +1, +1},
      // Back
      {+1, -1, -1}, {-1, -1, -1}, {-1, +1, -1},
      {+1, -1, -1}, {-1, +1, -1}, {+1, +1, -1},
      // Right
      {+1, -1, -1}, {+1, +1, -1}, {+1, +1, +1},
      {+1, -1, -1}, {+1, +1, +1}, {+1, -1, +1},
      // Left
      {-1, -1, +1}, {-1, +1, +1}, {-1, +1, -1},
      {-1, -1, +1}, {-1, +1, -1}, {-1, -1, -1},
      // Top
      {-1, +1, +1}, {+1, +1, +1}, {+1, +1, -1},
      {-1, +1, +1}, {+1, +1, -1}, {-1, +1, -1},
      // Bottom
      {-1, -1, -1}, {+1, -1, -1}, {+1, -1, +1},
      {-1, -1, -1}, {+1, -1, +1}, {-1, -1, +1}}};
  // clang-format on

  void createSkybox();
  void renderSkybox();
  void destroySkybox() const;
  void loadModel(std::string_view path);
    void loadFoodModel();

};

#endif

#include "food.hpp"

void Food::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

std::array<glm::vec2, 4> Food::getRandomPositions(int x, int y) {
  float step = 0.05f;
  m_food_positions = glm::vec2(x,y);

  return std::array{
    glm::vec2(-1.0f+step+(step*y*1.0f), 1.0f-(step*x*1.0f)), 
    glm::vec2(-1.0f+(step*y*1.0f), 1.0f-(step*x*1.0f)),
    glm::vec2(-1.0f+(step*y*1.0f), 1.0f-step-(step*x*1.0f)),
    glm::vec2(-1.0f+step+(step*y*1.0f), 1.0f-step-(step*x*1.0f))
    };
}

void Food::create(GLuint program) {
  destroy();

  m_program = program;

  auto const seed{std::chrono::steady_clock::now().time_since_epoch().count()};
  m_randomEngine.seed(seed);

  std::uniform_real_distribution<float> realDistribution(-0.9f, 0.9f);

  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");
  
  m_translation = glm::vec2(0);
    std::uniform_int_distribution randomIndex(0, 39);

  std::array<glm::vec2, 4> positions = getRandomPositions(randomIndex(m_randomEngine), randomIndex(m_randomEngine));   



  // for (auto &position : positions) {
  //   position /= glm::vec2{2.0f, 2.0f};
  // }
  
  std::array const indices{0, 1, 2, 3, 0 , 2
                           };

  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  abcg::glGenVertexArrays(1, &m_VAO);

  abcg::glBindVertexArray(m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  abcg::glBindVertexArray(0);
}

void Food::paint(const GameData &gameData){
 if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_VAO);

  abcg::glUniform1f(m_scaleLoc, m_scale);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}


#include "food.hpp"
#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

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

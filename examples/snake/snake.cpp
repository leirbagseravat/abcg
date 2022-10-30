#include "snake.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>


void Snake::create(GLuint program) {
  destroy();
 
  m_snakes_positions.clear();
  m_snakes_positions.push_back(glm::vec2(19,19));
  m_snakes_positions.push_back(glm::vec2(19,20));
  
  m_program = program;


  // Get location of uniforms in the program
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  for(auto const &pos : m_snakes_positions) {
     m_snakes.push_back(makeSnakeUnit(pos.x,pos.y));
  }

//   // Reset ship attributes
//   m_translation = glm::vec2(0);

// std::array<glm::vec2, 4> positions;
//      for(int i=0;i<40;i++) {
//        for(int j=0;j<40;j++) {
//           if (m_grid[i][j] == 1){
//                 positions = std::array{glm::vec2(-1.0f+step+(step*j*1.0f), 1.0f-(step*i*1.0f)), 
//                       glm::vec2(-1.0f+(step*j*1.0f), 1.0f-(step*i*1.0f)),
//                       glm::vec2(-1.0f+(step*j*1.0f), 1.0f-step-(step*i*1.0f)),
//                       glm::vec2(-1.0f+step+(step*j*1.0f), 1.0f-step-(step*i*1.0f))};
//           }
//       }
//      }


//   std::array const indices{0, 1, 2, 0 , 2, 3};

//   // Generate VBO
//   abcg::glGenBuffers(1, &m_VBO);
//   abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//   abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
//                      GL_STATIC_DRAW);
//   abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

//   // Generate EBO
//   abcg::glGenBuffers(1, &m_EBO);
//   abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
//   abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
//                      GL_STREAM_DRAW);
//   abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//   // Get location of attributes in the program
//   GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};
  

//   // Create VAO
//   abcg::glGenVertexArrays(1, &m_VAO);

//   // Bind vertex attributes to current VAO
//   abcg::glBindVertexArray(m_VAO);

//   abcg::glEnableVertexAttribArray(positionAttribute);
//   abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//   abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
//                               nullptr);
//   abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

//   abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

//   // End of binding to current VAO
  // abcg::glBindVertexArray(0);
}

Snake::SnakeUnit Snake::makeSnakeUnit(int x, int y) {
  SnakeUnit unit; 
  unit.m_translation = glm::vec2(0);


  std::array<glm::vec2, 4> positions;   
  positions = std::array{
    glm::vec2(-1.0f+step+(step*y*1.0f), 1.0f-(step*x*1.0f)), 
    glm::vec2(-1.0f+(step*y*1.0f), 1.0f-(step*x*1.0f)),
    glm::vec2(-1.0f+(step*y*1.0f), 1.0f-step-(step*x*1.0f)),
    glm::vec2(-1.0f+step+(step*y*1.0f), 1.0f-step-(step*x*1.0f))
    };

  std::array const indices{0, 1, 2, 0 , 2, 3};


  // Generate VBO
  abcg::glGenBuffers(1, &unit.m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, unit.m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  //   // Generate EBO
  abcg::glGenBuffers(1, &unit.m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unit.m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STREAM_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &unit.m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(unit.m_VAO);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, unit.m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, unit.m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);

  return unit;

}

void Snake::paint(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);


  for (auto const &unit : m_snakes) {
    abcg::glBindVertexArray(unit.m_VAO);

  abcg::glUniform1f(m_scaleLoc, unit.m_scale);
    //abcg::glUniform1f(m_rotationLoc, &unit.m_rotation);
  abcg::glUniform2fv(m_translationLoc, 1, &unit.m_translation.x);
  abcg::glUniform4fv(m_colorLoc, 1, &unit.m_color.r);

  abcg::glDrawElements(GL_TRIANGLES, 2*3,   GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);
  }

  

  abcg::glUseProgram(0);
}

void Snake::destroy() {
  for (auto const &unit : m_snakes) {
  abcg::glDeleteBuffers(1, &unit.m_VBO);
  abcg::glDeleteBuffers(1, &unit.m_EBO);
  abcg::glDeleteVertexArrays(1, &unit.m_VAO);
  }

}

void Snake::increaseSize() {
  
    if (m_lastInput == Input::Right) {
            glm::vec2 pos = m_snakes_positions.front();
            m_snakes_positions.push_front(glm::vec2(pos.x, pos.y+1));

    }

      if (m_lastInput == Input::Left) {
            glm::vec2 pos = m_snakes_positions.front();
            m_snakes_positions.push_front(glm::vec2(pos.x, pos.y-1));

        }

      if (m_lastInput == Input::Up) {
            glm::vec2 pos = m_snakes_positions.front();
            m_snakes_positions.push_front(glm::vec2(pos.x-1, pos.y));

        }

          if (m_lastInput == Input::Down) {
            glm::vec2 pos = m_snakes_positions.front();
            m_snakes_positions.push_front(glm::vec2(pos.x+1, pos.y));

        }
  
}

void Snake::update(GameData const &gameData) {

  if (gameData.m_state == State::Playing && m_stepTimer.elapsed() > 0.1) {
    m_stepTimer.restart();
    if (m_lastInput == Input::Right) {
            glm::vec2 pos = m_snakes_positions.back();
            m_snakes_positions.push_back(glm::vec2(pos.x, pos.y+1));
            m_snakes_positions.pop_front();
    }

  if (m_lastInput == Input::Left) {
            glm::vec2 pos = m_snakes_positions.back();
            m_snakes_positions.push_back(glm::vec2(pos.x, pos.y-1));
            m_snakes_positions.pop_front();
        }

  if (m_lastInput == Input::Up) {
            glm::vec2 pos = m_snakes_positions.back();
            m_snakes_positions.push_back(glm::vec2(pos.x-1, pos.y));
            m_snakes_positions.pop_front();
        }

          if (m_lastInput == Input::Down) {
            glm::vec2 pos = m_snakes_positions.back();
            m_snakes_positions.push_back(glm::vec2(pos.x+1, pos.y));

            //m_snakes_positions.push_back(glm::vec2(pos.x, pos.y-1));
            m_snakes_positions.pop_front();
        }
  }

  if (gameData.m_state == State::Playing ) {
  if (gameData.m_input[gsl::narrow<size_t>(Input::Right)]) {
            m_lastInput = Input::Right;
            return;
        }

  if (gameData.m_input[gsl::narrow<size_t>(Input::Left)]) {
            m_lastInput = Input::Left;
            return;
        }

  if (gameData.m_input[gsl::narrow<size_t>(Input::Up)]) {
                m_lastInput = Input::Up;
            return;
        }

          if (gameData.m_input[gsl::narrow<size_t>(Input::Down)]) {
                            m_lastInput = Input::Down;
            return;
        }
  }

  destroy();
  m_snakes.clear();
  for(auto const &pos : m_snakes_positions) {
     m_snakes.push_back(makeSnakeUnit(pos.x,pos.y));
  }
}

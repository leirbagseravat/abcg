Gabriel Tavares Frota de Azevedo 
RA 11201722230

LAB 2:
    - a ideia foi criar um Jogo da cobrinha, temos 3 entidades renderizadas nesse food, snake, window e GameState que contém os Enums (Input, State)

      a window vai controlar nossa snake e food, criamos a função onEvent() para lidar com os eventos de entrada e saída do teclado, atualizamos o nosso Input com a tecla apertada. Na função Window::OnCreate importamos nossa fonte que vai ser para o texto "Game Over" e criamos uma cobra com tamanho igual a 2 e uma food, para a cobra mudar de tamanho ela tem que comer uma food e assim o seu tamanho é atualizado em 1 pelo fim da cobra. Duas funções importantes da window que controlam o estado do jogo é Window::checkFoodHasEaten() e Window::checkGameOver() a primeira verifica se o par (x,y) da comida é igual ao par (x,y) da cabeça da cobra, caso verdadeiro a comida é destruida e recriada e a cobra atualiza seu tamano em um. Já a segunda função verifica se a cobra saiu do nosso espaço matricial, verificando se o seu par (x,y) execede o limite do quadro.

    a nossa Snake é formada por uma lista de SnakeUnit que é basicamente 2 GL_TRIANGLES do openGl equiláteros que formam um quadrado, na função  Snake::update é responsável por verificar o Input atual que foi atualizado na nossa window e setar na nossa variável m_lastInput isso só acontece após a checagem do State sendo igual ao Playing, além disso ela faz o movimento da nossa cobra que é composto por um timer, ou seja, a cada 100ms a cobra é atualizada em função do nosso Enum Input, removemos o último elemento da cobra e colocamos um na frente da cabeça em função da nossa direção. Dentro da cobra também existe a função increaseSize() que é chamada pela nossa window quando a cobra come a comida. 

    Na classe Food temos basicamente a random_engine para colocar a comida em uma posição (x,y) aleatória e a food é criada também por 2 GL_TRIANGLES equiláteros que formam um quadrado que é representada pela nossa comida, ela é estática e muda de posição caso a cobra coma a comida ou o jogo acabe. Quando o nosso jogo acaba criamos um timer e em 5 segundos o jogo é reiniciado automaticamente.

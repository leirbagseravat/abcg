Gabriel Tavares Frota de Azevedo 
RA 11201722230

- LAB 3:  a ideia foi implementar o jogo da cobrinha em 3D, com os mesmo estados que tivemos no jogo 2D, mas com as principais mudanças nos objetos renderizados. 

- a window nossa window vai ser responsável pelos objetos renderizados (Snake, Food, Ground e CameraView) criamos a função onEvent() para lidar com os eventos de entrada e saída do teclado, atualizamos o nosso Input com a tecla apertada. Na função Window::OnCreate importamos nossa fonte que vai ser para o texto "Game Over" e criamos uma cobra com tamanho igual a 2 e uma food, para a cobra mudar de tamanho ela tem que comer uma food e assim o seu tamanho é atualizado em 1 pelo fim da cobra. Duas funções importantes da window que controlam o estado do jogo é Window::checkFoodHasEaten() e Window::checkGameOver() a primeira verifica se o par (x,y) da comida é igual ao par (x,y) da cabeça da cobra, caso verdadeiro a comida gerada um novo local e a cobra atualiza seu tamano em um. Já a segunda função verifica se a cobra saiu do nosso espaço matricial, verificando se o seu par (x,y) execede o limite do quadro.

-  Para criamos a nossa Camera, precisamos realizar uma transformação linear em todo objeto que for renderizado no nosso espaço mundo. Primeiro definimos o ponto onde nossa camera vai estar, o ponto onde a mesma estará olhando, assim temos um vetor direção e definos um vetor para cima para em função dos nossos dois pontos. Após essa definição temos uma matriz que utilizamos ao renderizar cada objeto como o ground, a snake e a food.

-  a nossa Snake é formada por uma lista de SnakeUnit que é um box.obj que lê-mos na função onCreate() e injetamos no nosso código. Basicamente nele existem 8 pontos e 36 index, que formam 12 GL_TRIANGLES do openGl equiláteros que formam um cubo, na função  OnUpdate é responsável por verificar o Input atual que foi atualizado na nossa window e setar na nossa variável m_lastInput isso só acontece após a checagem do State sendo igual ao Playing, além disso ela faz o movimento da nossa cobra que é composto por um timer, ou seja, a cada 200ms a cobra é atualizada em função do nosso Enum Input, removemos o último elemento da cobra e colocamos um na frente da cabeça em função da nossa direção. Dentro da cobra também existe a função increaseSize() que é chamada pela nossa window quando a cobra come a comida. 

-  Para criarmos nosso item Food temos basicamente a random_engine para colocar a comida em uma posição (x,y) aleatória e a food é criada também porsomente um box.obj especial que é mudado a cor para amarelo e aplicamos duas transformações lineares nele, uma para sua escala ficar 75% da real e outra para realizar uma translação no eixo x e y 45% para termos um efeito de um símbolo de ouros das cartas. 

- Uma diferença interessante na cobra hoje é, na sua versão 2d criavamos e destruiamos os objetos com frequência, principalmente a class food, como estamos em um cenário 3d onde temos diversas novas entidades e mais uma dimensão decidi parar de utilizar as funções destroy/create e passei a utilizar somente a função de translação e atualizar os pontos (x,y) que eu guardo em uma variável para a comida. Utilizo essa mesma lógica no ao realizar o Game Over, ao invés de destruir tudo e recriar 5 s depois eu só escondo na tela em uma posição não vista pela camera e depois eu atualizo a posição quando eu voltar a jogar.

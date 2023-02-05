Gabriel Tavares Frota de Azevedo 
RA 11201722230

- LAB 4:  a ideia foi adicionar textura e iluminação no jogo que foi entregue no lab 3. Fiz um tentativa no código que não deu muito certo de criar models com programas diferentes para renderizar com o shaders normal a comida e o shaders cuberefract o corpo da cobra, não deu muito certo e optei por renderizar ambas no mesmo program cuberefract, mas deixei o código comentando caso identifique no seu feedback o erro, fiquei curioso para ajustar.

# Window
- Classe responsável por chamar as funções públicas de create, paint, resize, update destroy em seus respectivos onCreate, onPaint, onResize e onDestroy, além de no Create carregar os sharders da pasta assets e implementar o onEvent para capturar os eventos de entrada do teclado.

# Funções

- onCreate: Cria o programa para renderizar o Board e chamar a função create do model, skyBox e executar a função loadObj de Cube;
- onUpdate: Responsável por chamar a função update de Model para recalcular a matriz de animação passando o estado do jogo como parâmetro;
- onPaint: Executa a função paint do Model, pegar a variáveis uniformes e rederizar a skyBox;
- onPaintUI: Responsável por criar a UI (interface);
- onResize: Quando a janela é mudada de tamanho pelo usuário, essa função é responsável por adequar a dimensão com a variável m_viewportSize;
- onDestroy: Responsável por chamar a função destroy de Ground e Cube, deletar o programa e o destroy da skybox.
- onEvent: Responsável por capturar os eventos de entrada do teclado, as setas. São responsáveis por setar o estado da última seta no m_game_data para direcionar a cobra;
- createSkybox: Cria o VBO e VAO da skyBox, salva a localização das variávies uniformes do programa de renderização, cria toda geometria do cubo;
- renderSkybox: Atualiza o valor das variáveis uniformes e pinta o skyBox;
- loadSkyTexture: Carrega a textura da skyBox;
- destroySkybox: Libera da memória o VBO e VAO da skyBox;

# Atributos

- m_viewportSize: Guarda a resolução da janela;
- m_program: Guarda o programa para renderizar os modelos;
- m_viewMatrix: Matriz de visão;
- m_viewMatrixLoc: Localização da matriz de visão no programa de renderização;
- m_projMatrix: Matriz de visão;
- m_viewMatrixLoc: Localização da matriz de visão no programa de renderização;
- m_colorLoc: Localização do vetor de cor no programa de renderização;
- m_model: Objeto do tipo Model que é responsável por criar/renderizar o corpo da cobra e a comida;
- m_food: Obejeto do tipo Model que seria responsável por criar/renderizar somente a comida;
- m_gameData: Objeto que guarda o estado do jogo e a lista de possíveis inputs do usuário;
- m_skyPositions: Posições dos vértices do skyBox;
- m_skyShaderName: Nome do shader da skyBox;
- m_skyVAO: VAO para skyBox;
- m_skyVBO: VBO para skyBox;
- m_mappingMode: variável inteira pre-definida como 2 para selecionarmos o mapeamento esférico;
- m_skyProgram: Programa de renderização para a skyBox;
- m_skyTexture: Textura da skyBox;
- m_IaLoc: Localização do valor da propriedade ambiente da luz no programa de renderização;
- m_IdLoc: Localização do valor da propriedade difuza da luz no programa de renderização;
- m_IsLoc: Localização do valor da propriedade especular da luz no programa de renderização;
- m_Ia: Valor da propriedade ambiente da luz;
- m_Ia: Valor da propriedade difusa da luz;
- m_Ia: Valor da propriedade especular da luz.  

# Model
- Classe responsável por renderizar a box que é uma unidade do corpo da cobra e o a comida antes das transformações matricias. 

# Funções

- create: Cria o VBO, EBO e VAO do Box;
- loadObj: Carrega o modelo da Box da pasta de assets;
- createBuffers: Carrega o VBO e EBO do Box;
- render: Responsável por atualizar o valor das variáveis uniformes e continuamento pintar o Box;
- onUpdate: Resposável por realizar o passo da cobra, que consiste em verificar caso o timer tenha excedido o seu tempo, ele entra dentro de um bloco if que ele verifica qual foi o último input e em função desse último input ele remove o "rabo" da cobra e adiciona uma nova "cabeça" em um coordenada x,y a depender do m_lastInput;
- destroy: Libera a memória utilizada pelo VBO, VAO e EBO;
- computeNormals: Calcula as normais dos vértices de acordo com os triângulos que fazem parte.
- checkFoodHasEaten: é chamado dentro da onUpdate e é checkado se existe uma colisão entre a posição da comida e a posição de alguma parte do corpo da cobra. Caso verdadeiro ele chamada a função increaseSize() e gera uma nova posição para a comida;
- increaseSize: a função increaseSize é reponsável por adicionar no "rabo" da cobra uma unidade a mais em função do m_lastInput;

# Atributos

- m_program: Programa de renderização da Box;
- m_VAO: VAO da box;
- m_VBO: VBO da box;
- m_EBO: EBO para os índices da box;
- m_vertices: Vertices do modelo;
- m_indices: Indices do modelo;
- m_modelMatrixLoc: Guarda a posição da variável uniforme modelMatrix do programa de renderização;
- m_positionMatrix: Matriz responsável por transladar o cubo para a sua posição (m_position);
- m_modelMatrix: Matriz de modelo do Cubo, no paint ela recebe o valor de m_positionMatrix * m_animationMatix (Primeiro o cubo é animado e depois posto na posição no mundo);
- m_Ka: Valor da propriedade ambiente do material;
- m_Kd: Valor da propriedade difusa do material;
- m_Ks: Valor da propriedade ambiente do material.
- m_gameData: entidade que guarda o enum do status do jogo e da lista de possíveis inputs;
- m_lastInput: variável do tipo input que guarda a última entrada do usuário, por default começa com Input::right; 
- m_snakes_positions: lista de duplas que guardam a posição discreta da cobra, com essa lista de posições é possível aplicar transformações matriciais na hora de pintar a box e fazer com que ambas fiquem enfileiradas para criar o corpo da cobra;
- food_loc: guarda a localização da cobra que passa pelo mesmo processo de transformação do corpo da cobra, sua posição inicial num plano x,y é (5,13);
- m_stepTimer: timer responsável pelo "passo" da cobra, ou seja quando sua posição é atualizada;
- m_randomEngine: determina uma posição x,y aleatória para ser gerada a comida após ser comida;



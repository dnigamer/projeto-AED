# Diretório ``src/gui``
Este diretório contém o todos os ficheiros de código fonte da interface gráfica do programa.

### Estrutura do diretório
1. gui.cpp - Ficheiro principal da interface gráfica.
   - Contém a função startGui() que inicia a interface gráfica.
     - Esta função recebe como argumentos o mesmo da função main() do programa principal e adicionalmente um pointer para o atual stock da loja. 
     - Inicia a janela do Main Menu e define certas propriedades da janela.
2. gui.h - Ficheiro de cabeçalho da interface gráfica.
   - Contém a declaração da função startGui() para poder ser chamada no ficheiro principal.
3. mainMenu.cpp - Ficheiro com a implementação da janela do Main Menu.
4. mainMenu.h - Ficheiro de cabeçalho da janela do Main Menu.
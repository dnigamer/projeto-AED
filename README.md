# Trabalho AED 2023/2024
## Tema escolhido
O tema escolhido para o trabalho foi o de sistema de gestão de uma loja de bens essenciais. Outro tema poderia ter sido escolhido, como o desenvolvimento de um jogo. <br/>
Porém, o tema escolhido foi o de uma loja de bens essenciais por facilidade de implementação e por ser um tema que pode ser facilmente implementado para um ambiente real.

## Objetivo do trabalho
Pretende-se desenvolver uma aplicação para a gestão de dados de contexto livre, ou seja, com liberdade na escolha temática do tipo de negócio (loja de bens essenciais, loja de informática, loja de animais, âmbito eleitoral, estatísticas de guerra, monitorização de atividade vírica/bacteriana, etc.).
Em termos de funcionalidades, devem prever-se as seguintes:
 - criar linhas de produtos
 - adicionar/editar/remover produtos inerentes a uma linha
 - pesquisar e ordenar produtos. 

Deve ter-se o cuidado de seguir as boas práticas da programação na organização do código (usar bibliotecas .h para englobar e seccionar grupos de funções de um mesmo tipo de operações – e.g. manipular listas –, reduzir a utilização de variáveis globais ao mínimo ou até mesmo evitar usar, recorrer a um programa main num ficheiro próprio que sirva de “cola lógica” entre a linha central de execução e as bibliotecas, se existirem funções no main, separar adequadamente protótipos e implementações, etc.). <br/>
O foco no desenvolvimento deste trabalho deve ser o conteúdo programático delineado para a unidade curricular de AED (pesquisa e ordenação eficientes, apontadores, estruturas de comprimento fixo, alocação dinâmica e estruturas de dados de comprimento varável tais como filas, pilhas e listas, etc.).

## Objetivos específicos
Mais concretamente, para além do objetivo geral do trabalho, deve ser considerado o seguinte conjunto de aspetos base:
1. Criar funcionalidade de gestão de dados, começando pela definição dos mesmos, para o contexto-alvo do trabalho (por exemplo, para produtos, ter o cuidado de definir structs, com ID, nome, preço, número em stock);
2. Mapear cada linha numa estrutura dinâmica à escolha: filas, pilhas, listas;
3. Permitir editar os dados de cada elemento criado (e.g. stock);
4. Permitir inserir novos elementos e eliminar;
5. Aplicar estratégias de pesquisa, ordenação e apresentação de informação (por exemplo, converter estrutura dinâmica para vetor e aplicar quick sort);
6. Produzir saídas que reflitam cuidado no visual na apresentação de conteúdos e dados.
7. Codificar refletindo cuidado na organização do código (main, funções e bibliotecas .h criadas para seccionar grupos funcionalidades, etc.).

Como funcionalidades extra, podem ser consideradas as seguintes opções, que podem ser valorizadas na avaliação do trabalho:
1. Exploração de mais do que 1 estrutura dinâmica;
2. Incorporação de métodos de contagem de tempo e instruções (N);
3. Exportação de dados (e.g. CSV);
4. Exploração de mais do que uma estratégia de ordenação (e.g. bubble sort para listas compostas por até 20 elementos e quick sort para listas maiores);
5. Outros que possam ser valorizáveis no contexto do trabalho e objetivos da unidade curricular.

## Estrutura do relatório sobre o trabalho
O relatório sobre o trabalho deve ser estruturado de acordo com o seguinte esquema:
 - Capa: elementos de identificação do trabalho e do grupo.
 - Índice: estrutura do relatório.
 - Introdução: identificação da temática/contexto, descrição do problema e dos objetivos.
 - Planeamento da solução: identificação dos requisitos do programa, diagramas de especificação (e.g. arquitetura geral, funcionalidades/casos-de-uso, estruturas de ficheiros, formatos e campos, etc.), assim como uma descrição das estruturas de dados (vetores, filas, pilhas, listas, etc.) utilizadas como estratégia para lidar com a multiplicidade de elementos (e.g. produtos, pessoas, entre outras entidades relevantes que façam sentido para a solução proposta).
 - Proposta de solução: apresentação, por linhas gerais, da solução implementada, dos ficheiros que o constituem e da estrutura do código C implementado.
 - Utilização de ferramentas de geração de código (e.g. ChatGPT): clarificação acerca do uso de ferramentas de inteligência artificial generativas e das partes do código que beneficiaram da ajuda destas ferramentas, divulgação do processo de prompting e taxa estimada de utilização (percentagem de código feito de raiz vs. código gerado).
 - Conclusões e perspetivas de desenvolvimento futuro: notas finais, principais conclusões e aspetos que podem ser melhorados.
 - Bibliografia: livros, artigos, páginas Web usados para desenvolver o trabalho.

## Entrega do trabalho
Recorrendo à BlackBoard para proceder à submissão, da entrega deve constar um ficheiro zip com:
 - Código C (comentado) da aplicação;
 - Relatório em PDF e fontes (Word/LaTeX).
# Projeto-Estrutura-de-Dados

# Conversão e Manipulação de Arquivos Binários

Este projeto implementa um sistema em C++ capaz de converter arquivos CSV para binários e realizar diversas operações sobre os dados convertidos, incluindo ordenação externa, visualização e manipulação de registros. 

## Finalidade

A finalidade deste trabalho é criar um sistema que:
- Converta uma base de dados em formato CSV (fornecida pelo Campus Virtual) para arquivo binário;
- Realize ordenação externa usando partições;
- Permita ao usuário realizar operações como inserção, alteração, troca e visualização de registros diretamente sobre o arquivo binário.

## Estrutura do Projeto

## Estrutura `Info`
A base do projeto é a estrutura `Info`, que representa cada registro e contém os seguintes campos:
- `id` (int)
- `nome` (char[100])
- `equipe` (char[100])
- `jogos` (char[50])
- `ano` (int)
- `estacao` (char[20])

Etapas do Sistema

1. **Conversão CSV → Binário**
   - A classe `LeitorCSV` lê o arquivo `.csv`, trata os campos e escreve os dados no arquivo binário `csv_em_binario.dat`.

2. **Visualização Binário → CSV**
   - A classe `ConversorBinarioCSV` imprime os registros do arquivo binário no formato CSV, para validação visual.

3. **Ordenação Externa**
   - Implementada pela classe `GerenciadorDados`, que divide o arquivo binário em partições ordenadas.
   - As partições são fundidas em um arquivo final ordenado `ordenado.dat` usando um heap máximo (merge multiway).

4. **Operações Interativas**
   - Menu com as seguintes opções:
     - Imprimir todos os registros ordenados
     - Visualizar intervalo de posições
     - Alterar registro em posição específica
     - Trocar registros de posição
     - Inserir novo registro em posição escolhida

Como Compilar e Executar

1. Compile os arquivos com um compilador C++:
   
2. Execute a conversão:

3. Em seguida, execute o sistema interativo:
  

Certifique-se de que o arquivo `data_athlete_game.csv` está na mesma pasta dos executáveis.


Conclusão

Este projeto demonstrou como é possível aplicar técnicas de ordenação externa, manipulação de arquivos binários e estruturas de dados em C++ para construir um sistema eficiente, capaz de lidar com grandes volumes de dados.O uso de merge com heap foram pontos chave para alcançar esse objetivo. O sistema é modular, interativo e adaptável a diferentes conjuntos de dados.

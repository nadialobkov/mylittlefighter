# 🦄✨ My Little Fighter 🌈💖

## 📃 Contexto

Esta é a minha implementação do **trabalho final da disciplina de Programação** 2 (CI10802) do curso de BCC da UFPR. 
O objetivo do trabalho foi programar um jogo no **estilo do Street Fighter na linguagem C** usando a biblioteca de interface
gráfica **Allegro**.

## 🔧 Compilação e Execução do Projeto

### Instalação da Biblioteca Allegro

Para poder jogar o My Little Fighter você precisará **instalar a biblioteca Allegro**. Para isso você precisa executar no terminal
(Linux) o seguinte comando:

> sudo apt-get update

> sudo apt-get install liballegro5-dev

*Mais informações sobre a biblioteca Allegro podem ser encontradas no site https://liballeg.org/*

### Regras Makefile

Em seguida será necessário compilar o programa:

> make

Para remover os arquivos objetos gerados:

> make clean

Para remover os arquivos objetos e o executável:

> make purge

### Execução

Uma vez compilado, para executar o jogo:

> ./main


## 🎮 Jogo

### ⚙️ Configurações

Para ajustar o tamanho da janela, você pode alterar as macros ```X_SCREEN```, ```Y_SCREEN``` e ```RESIZE_SCREEN``` definidas no arquivo ```mylittlefighter.h```.
Por default, as dimensões estão definidas como 1920 x 1080 px. O ```RESIZE_SCREEN``` corresponde à relação ```Y_SCREEN / 1080```.

```c
  #define X_SCREEN 1920
  #define Y_SCREEN 1080
  #define RESIZE_SCREEN 1
```

### 💻 Menu

Para clicar nos botões e se movimentar pelos menus, você deve usar o **mouse**. Ao iniciar o jogo, na tela inicial há um botão **START** que permite seguir 
para os proximos menus.

### 🔄 Modos

O jogo possui dois modos que podem ser escolhidos no menu de seleção de personagem:

1. **Player Versus Player (PVP):** permite que dois jogadores lutem ao mesmo tempo entre si.
2. **Player Versus Bot (BOT):** permite que um jogador lute contra um bot programado.

### ✅ Regras

O My Little Fighter tem **três rounds** e ganha quem vencer duas rodadas. O objetivo e tirar a vida do outro player através de golpes

### 🖲️ Ações dos Jogadores

#### Player 1 
- **W**: Pula
- **S**: Abaixa
- **D**: Anda para direita
- **A**: Anda para esquerda
- **5**: Ataque 1 (soco)
- **6**: Ataque 2 (coice)
- **5 + 7**: Ataque combo
- **D ou A + R**: Dash

#### Player 2 
- **⬆️**: Pula
- **⬇️**: Abaixa
- **➡️**: Anda para direita
- **⬅️**: Anda para esquerda
- **1️⃣**: Ataque 1 (soco)
- **2️⃣**: Ataque 2 (coice)
- **1️⃣ + 3️⃣**: Ataque combo
- **➡️ ou ⬅️ + 0️⃣**: Dash

   *Setas e numeros do numpad*


 
 

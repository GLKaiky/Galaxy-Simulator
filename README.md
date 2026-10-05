## 👨‍💻 Criador

**Kaiky França dos Reis Silva**

*Desenvolvedor de Software | Estudante na PUC Minas*

* 💼 **LinkedIn:** [linkedin.com/in/glkaiky](https://www.linkedin.com/in/glkaiky)
* 🌐 **Site/Portfólio:** [kaikyfrs.com.br](https://kaikyfrs.com.br)
* 🐙 **GitHub:** [github.com/GLKaiky](https://github.com/GLKaiky)

---

## 🙏 Agradecimentos

### Em memória

> De **José Soares dos Reis** meu avô que se foi em setembro de 2026, por todo apoio e motivação que ele pode me dar nos últimos anos, que sua lembrança fique eternizada aqui como o melhor cozinheiro que a Fiat teve, e o melhor avô que alguém pôde ter.

### 🌟 Também lembrados

* **[Giusfds](https://github.com/Giusfds):** Pela indicação na EloGroup.
* **Suzane**: Por ter me aguentado explicando sobre galáxia e gravidade.
* **Isaac Newton:** Pela equação da gravidade, e pelos livros que eu usei nesse projeto.
* **Romário Faria:** Pela copa de 94.
* **Gemini:** Pela RayLib, porque eu não tenho saco de mexer com biblioteca gráfica (créditos a ele por essa parte).
* **Jesus Cristo:** À ele toda honra e glória por todos os séculos, dos séculos.


# 🌌 Simulador de Galáxia 3D (N-Body / Barnes-Hut)

Um simulador astrofísico de N-corpos de alta performance escrito em **C++17** e renderizado com **Raylib**. O projeto simula a gravidade interativa de dezenas de milhares de estrelas em tempo real, utilizando otimizações de estrutura de dados espaciais e processamento paralelo.

## 🚀 Principais Tecnologias e Arquitetura

- **Algoritmo de Barnes-Hut ($O(N \log N)$):** Substitui o cálculo gravitacional de força bruta por uma *Octree* dinâmica com *Memory Arena* (Zero Allocation no laço principal), permitindo rodar +30.000 partículas inteiramente na CPU.
- **Multithreading (OpenMP):** Distribuição do cálculo de força gravitacional em múltiplos núcleos do processador.
- **Física Estável (Leapfrog & Matéria Escura):** Integrador numérico de tempo fixo (Kick-Drift-Kick) aliado a um modelo analítico de Halo de Matéria Escura (esfera isotérmica) para prevenir a dissipação dos braços espirais e manter órbitas estáveis.
- **Pipeline Visual (Multi-pass Bloom):** Renderização avançada via GPU Instancing e cascatas de *downsampling* para simular emissão de luz e poeira estelar de forma aditiva.

---

## 🛠️ Pré-requisitos

Para compilar o projeto nativamente no Linux, você precisará do compilador GCC (com suporte a C++17 e OpenMP).

```bash
# Atualize os pacotes
sudo apt update

# Instale o compilador e ferramentas essenciais
sudo apt install build-essential g++ make

```

*(Nota: Certifique-se de ter os binários/bibliotecas da Raylib na pasta `./lib` ou instalados no sistema, conforme configurado no `Makefile`).*

---

## ⚙️ Como Compilar e Executar

O projeto utiliza um `Makefile` configurado com as flags de otimização extremas (`-O3`, `-ffast-math`, `-flto`) cruciais para a performance do simulador de N-corpos.

* Para compilar a versão de lançamento (Release) otimizada:
```bash
make

```


* Para compilar e executar o simulador automaticamente:
```bash
make run

```


* Para limpar os arquivos de build gerados:
```bash
make clean

```

---

## 🎮 Controles da Câmera

A simulação e a navegação rodam em *threads* lógicas separadas (Acumulador de Tempo Fixo), permitindo que você navegue suavemente independente do peso do cálculo físico.

* **Zoom (Aproximar/Afastar):** Gire a **Roda do Mouse (Scroll)** para mergulhar nos braços espirais.

---

## 🧬 Como Customizar a Galáxia (`GalaxyParams.hpp`)

O simulador foi desenhado em um formato *Data-Driven*. Toda a morfologia e o comportamento da galáxia são definidos no arquivo **`include/GalaxyParams.hpp`**. Basta alterar os valores na *struct*, recompilar (`make`) e ver as mudanças ao vivo.

Aqui estão os principais parâmetros que você pode modificar:

### 1. Desempenho e Densidade

* `numStars`: Quantidade total de corpos celestes. (Padrão: `30000`). Aumente por sua conta e risco dependendo do seu processador!
* `theta`: O limite de precisão do Barnes-Hut. (Padrão: `1.5`).
* Valores baixos (`0.5`) são extremamente precisos, mas muito lentos.
* Valores altos (`1.5` a `2.0`) são rápidos, tratando braços espirais inteiros como um único super-planeta.



### 2. Física e Matéria Escura

* `G`: A Constante Gravitacional do sistema.
* `haloVelocity` (Padrão: `14.0`): Simula a força da Matéria Escura. Aumentar este valor faz a galáxia girar mais rápido de forma rígida, "segurando" as pontas do disco. Se for muito baixo, as estrelas externas se dissiparão para o espaço vazio.
* `epsilon` (Padrão: `2.0`): *Softening* gravitacional. Impede que duas estrelas acelerem para o infinito caso colidam exatamente no mesmo pixel.

### 3. Morfologia (O Visual do Disco)

* `nArms`: O número de braços espirais. Tente mudar para `4` ou `5` para galáxias mais complexas!
* `armTwist` (Padrão: `0.7`): O quão enrolados estão os braços. Valores altos criam braços super fechados e apertados ao redor do núcleo. Valores baixos criam braços esticados.
* `bulgeFraction` (Padrão: `0.25`): A porcentagem das estrelas que nascem concentradas no núcleo incandescente da galáxia.
* `innerRadius`: A distância onde o buraco negro termina e o disco começa de fato.

## 🤝 CoCriadores (Contribuidores)

Este projeto tem foco acadêmico e é feito para evoluir! Quer me ajudar a escrever o algoritmo em Compute Shaders na GPU? Achou um gargalo na Octree? Quer adicionar simulação térmica ou colisões reais de matéria?

Faça um *fork*, abra um *Pull Request* e o seu nome ficará eternizado no nosso universo:

*Esta seção aguarda o primeiro explorador!*

* 🚀 `[Seu Nome/GitHub Aqui]` - *[Sua contribuição (ex: Portou a física para GLSL, Otimizou a Arena)]*
* 🌌 `[Seu Nome/GitHub Aqui]` - *[Sua contribuição]*

---

*Desenvolvido como um estudo intensivo de arquitetura C++, Otimização de Memória e Astrofísica Computacional.*

```text
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⡀⠒⠒⠦⣄⡀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⢀⣤⣶⡾⠿⠿⠿⠿⣿⣿⣶⣦⣄⠙⠷⣤⡀⠀⠀⠀⠀
⠀⠀⠀⣠⡾⠛⠉⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⣿⣷⣄⠘⢿⡄⠀⠀⠀
⠀⢀⡾⠋⠀⠀⠀⠀⠀⠀⠀⠀⠐⠂⠠⢄⡀⠈⢿⣿⣧⠈⢿⡄⠀⠀
⢀⠏⠀⠀⠀⢀⠄⣀⣴⣾⠿⠛⠛⠛⠷⣦⡙⢦⠀⢻⣿⡆⠘⡇⠀⠀
⠀⠀⠀⠀⡐⢁⣴⡿⠋⢀⠠⣠⠤⠒⠲⡜⣧⢸⠄⢸⣿⡇⠀⡇⠀⠀
⠀⠀⠀⡼⠀⣾⡿⠁⣠⢃⡞⢁⢔⣆⠔⣰⠏⡼⠀⣸⣿⠃⢸⠃⠀⠀
⠀⠀⢰⡇⢸⣿⡇⠀⡇⢸⡇⣇⣀⣠⠔⠫⠊⠀⣰⣿⠏⡠⠃⠀⠀⢀
⠀⠀⢸⡇⠸⣿⣷⠀⢳⡈⢿⣦⣀⣀⣀⣠⣴⣾⠟⠁⠀⠀⠀⠀⢀⡎
⠀⠀⠘⣷⠀⢻⣿⣧⠀⠙⠢⠌⢉⣛⠛⠋⠉⠀⠀⠀⠀⠀⠀⣠⠎⠀
⠀⠀⠀⠹⣧⡀⠻⣿⣷⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⡾⠃⠀⠀
⠀⠀⠀⠀⠈⠻⣤⡈⠻⢿⣿⣷⣦⣤⣤⣤⣤⣤⣴⡾⠛⠉⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠈⠙⠶⢤⣈⣉⠛⠛⠛⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠖⠃⠀⠀⠀⡁⠀⠀⠀⠀⠀⠐⠆⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡠⢔⡤⠊⠁⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⠀⠀⠁⠀⠀⠘⠁⢀⠀⠀⠀⠀⢈⠓⠂⠠⡄⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣶⠿⠞⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠒⠁⠀⠠⡚⠁⢀⣙⣀⣈⡩⠬⢁⠀⢑⠶⠤⡆⠤⡀⠀⠀⠀⠀⠀⠀⢀⠴⢲⣋⣽⣷⠟⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠀⢠⠀⠀⣶⠃⠗⣡⣶⣮⣿⡿⠿⠿⢿⣿⣷⣶⣤⣤⠤⠴⠦⠬⣤⣤⠄⣉⠉⠝⢲⣿⡷⠻⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠀⠀⠀⠁⡀⡸⠁⣰⣿⡿⠛⠋⣁⡀⠤⠤⢄⡀⠈⠛⢯⣿⣟⣾⣶⣶⣮⣭⣵⣾⣿⣟⠿⠉⢨⠖⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠀⢠⠳⡧⣻⡿⠋⢀⠒⠉⠀⠀⠀⠀⠀⠀⠉⠢⠀⠀⠙⠛⣻⣿⣿⣿⢿⣿⣿⠟⡱⠖⠊⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⢠⣧⠓⣾⣿⠁⠀⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⢦⣠⣾⣿⠿⣿⣿⣿⡿⣫⠏⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠂⢃⣸⣿⠇⢠⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣿⠟⢿⠁⠸⡿⣿⣯⡶⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⢘⡄⠘⣿⣿⠀⠸⡀⠀⠀⠀⠀⠀⢀⣀⣴⣾⣿⡿⡟⡋⠐⡇⠀⢸⣿⣿⠃⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢡⠘⢰⣿⡿⡆⠀⣇⠀⣀⣠⣤⣶⣿⢷⢟⠻⠀⠈⠀⠀⠀⡇⠀⣼⣿⣿⠂⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠔⢀⡴⢯⣾⠟⡏⢀⣠⣿⣿⣿⣟⢟⡋⠅⠘⠉⠀⠀⠀⠀⢀⠀⠁⢠⣿⣟⠃⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⠞⣻⣷⡿⢙⣩⣶⡿⠿⠛⠉⠑⢡⡁⠀⠀⠀⠀⠀⠀⢀⠔⠁⠀⣰⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣡⣾⣥⣾⢫⡦⠾⠛⠙⠉⠀⠀⢀⣀⠀⠈⠙⠓⠦⠤⠤⠀⠘⠁⢀⡤⣾⡿⠏⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠔⣴⣾⣿⣿⢟⢝⠢⠃⢀⣤⢴⣾⣮⣷⣶⢿⣶⡤⣐⡀⠀⣠⣤⢶⣪⣿⣿⡿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⡀⣦⣾⡿⡛⠵⠺⢈⡠⠶⠿⠥⠥⡭⠉⠉⢱⡛⠻⠿⣿⣿⣿⣿⣿⠿⠿⠿⠟⠭⠛⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⢀⢴⠕⣋⠝⠕⠐⠀⠔⠉⠀⠀⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠁⠉⠁⠁⠁⠁⠈⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡟⡄⠀
⢀⣠⠁⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠖⠒⢒⡶⠂⣠⣖⣲⠄⣠⠤⡧⣷⡇⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠠⠶⠯⠓⠒⠂⠳⠤⠖⠘⠓⠒⠛⠞⠃
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⡒⡄⡲⡂⢷⠲⢸⡲⢒⣘⡒⡀⠀

```

```

```

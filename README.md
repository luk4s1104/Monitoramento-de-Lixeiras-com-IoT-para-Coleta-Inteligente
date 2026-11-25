# 🗑️ Monitoramento de Lixeiras com IoT para Coleta Inteligente

Projeto desenvolvido no **Centro Universitário Facens** para a disciplina **UPX 1 (Usina de Projetos Experimentais)**, com o objetivo de otimizar a logística de coleta de resíduos utilizando Internet das Coisas (IoT) e comunicação de longo alcance via LoRa.

---

## 📋 Sobre o Projeto

A gestão ineficiente de resíduos urbanos ou institucionais frequentemente resulta em lixeiras transbordando ou equipes de limpeza e coleta realizando rotas desnecessárias em pontos ainda vazios. Este sistema monitora o nível de preenchimento das lixeiras e transmite esses dados sem fio para uma central, gerando históricos e gráficos para apoiar a tomada de decisão na coleta inteligente.

---

## ⚙️ Arquitetura e Funcionamento

O fluxo de dados do projeto é dividido em três etapas principais:

1. **Transmissão (`loraTrasmissor.ino`):** Instalado na lixeira, realiza a leitura do nível de resíduos por meio de sensores e envia os pacotes de dados via radiofrequência (LoRa).
2. **Recepção (`loraReceptor.ino`):** Recebe os pacotes enviados pelo módulo transmissor e os repassa ao computador via porta serial (USB).
3. **Processamento e Registro (`Comunicacao serial.cpp`):** Lê os dados da porta serial no computador, processa as informações recebidas e exporta o histórico de medições para análise em planilhas.

---

## 📂 Estrutura do Repositório

* **`loraTrasmissor.ino`**: Código-fonte embarcado responsável pela leitura dos sensores na lixeira e envio via LoRa.
* **`loraReceptor.ino`**: Código-fonte embarcado da base receptora que captura os dados LoRa e os envia pela interface serial.
* **`Comunicacao serial.cpp`**: Aplicação em C++ que captura os dados da porta serial do sistema operacional e salva os registros.
* **`Comunicacao serial.exe`**: Executável compilado para captura dos dados seriais em ambiente Windows.
* **`historico_coletas(conexao).csv`**: Base de dados em formato CSV contendo os registros históricos capturados pelo sistema.
* **`grafico_de_dados.xlsx`**: Planilha com tratamento visual e gráficos gerados a partir do histórico de coletas.

---

## 🛠️ Tecnologias e Componentes Utilizados

### Hardware
* Microcontroladores Utilizados: `[ ESP32 / Arduino Uno ]`
* Módulos de Comunicação: `[ Transceptor LoRa ]`
* Sensor de Nível: `[ Sensor Ultrassônico HC-SR04 ]`
* Alimentação: `[ Bateria 9V]`

### Software
* **C++ / Arduino IDE** (Programação dos microcontroladores e comunicação serial)
* **Microsoft Excel / CSV** (Armazenamento e visualização de dados)

---

## 🚀 Como Executar o Projeto

### 1. Configuração do Hardware
1. Carregue o código `loraTrasmissor.ino` no microcontrolador acoplado à lixeira.
2. Carregue o código `loraReceptor.ino` no microcontrolador conectado ao computador central.
3. Certifique-se de que ambos os módulos LoRa estejam configurados na mesma frequência (ex: `915 MHz` ou `433 MHz`).

### 2. Captura de Dados no Computador
1. Conecte o microcontrolador receptor via cabo USB e verifique a porta COM atribuída (ex: `COM3`).
2. Caso precise ajustar a porta COM ou taxa de transmissão (*baud rate*), edite o arquivo `Comunicacao serial.cpp` e recompile-o, ou execute diretamente o `Comunicacao serial.exe`.
3. Os dados capturados serão armazenados no arquivo `historico_coletas(conexao).csv` e podem ser visualizados na planilha `grafico_de_dados.xlsx`.

---

## 🎓 Instituição

**Centro Universitário Facens** – Sorocaba/SP  
Disciplina: **UPX 1 – Usina de Projetos Experimentais** `[1°Semestre de 2026]`
Orientadores: `[Isaias Goldshimdt / Marcelo Aguiar]`

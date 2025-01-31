# Projeto de Controle de LEDs com Raspberry Pi Pico W

Este projeto demonstra como controlar três LEDs (verde, azul e vermelho) usando um botão em uma Raspberry Pi Pico W. O código foi desenvolvido em C e utiliza a SDK da Raspberry Pi Pico.

## Funcionalidades

- **Controle de LEDs**: O projeto permite ligar e desligar três LEDs em sequência, com base no pressionamento de um botão.
- **Estados dos LEDs**:
  - **Todos ligados**: Quando o botão é pressionado, todos os LEDs são ligados.
  - **Dois ligados**: Após 3 segundos, o LED vermelho é desligado, deixando apenas o verde e o azul ligados.
  - **Um ligado**: Após mais 3 segundos, o LED azul é desligado, deixando apenas o verde ligado.
  - **Todos desligados**: Após mais 3 segundos, todos os LEDs são desligados.
- **Rotina Não Interrompível**: A rotina de acionar os LEDs não pode ser interrompida uma vez iniciada, garantindo que a sequência de estados dos LEDs seja executada completamente.
- **Debounce**: O código inclui uma lógica de debounce para evitar leituras falsas do botão.


## Componentes Necessários

- Raspberry Pi Pico W
- 3 LEDs (verde, azul e vermelho)
- 1 botão
- Resistores e fios conforme necessário.

## Esquema de Conexão

| Componente  | Pino Raspberry Pi Pico W |
|-------------|--------------------------|
| LED Verde   | GPIO 11                  |
| LED Azul    | GPIO 12                  |
| LED Vermelho| GPIO 13                  |
| Botão       | GPIO 5                   |

- **LEDs**: Conecte o ânodo (perna longa) de cada LED ao pino correspondente na Raspberry Pi Pico W. Conecte o cátodo (perna curta) a um resistor, e o outro lado do resistor ao GND.
- **Botão**: Conecte um lado do botão ao pino GPIO 5 e o outro lado ao GND. Use um resistor de pull-up para garantir leituras estáveis.

## Configuração do Ambiente

1. **Instale a SDK da Raspberry Pi Pico**:
   - Siga as instruções oficiais para configurar o ambiente de desenvolvimento: [Getting Started with Raspberry Pi Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).

2. **Clone este repositório**:
   ```bash
   git clone https://github.com/EnzoLisboa/Temporizador_OneShot.git
   cd seu-repositorio
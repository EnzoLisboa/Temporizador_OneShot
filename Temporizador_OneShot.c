#include <stdio.h>
#include "pico/stdlib.h"

// Definição dos pinos dos LEDs
#define GREEN_PIN 11
#define BLUE_PIN 12
#define RED_PIN 13

// Definição do pino do botão
#define BUTTON_PIN 5

// Definição do tempo de debounce em milissegundos
#define DEBOUNCE_DELAY_MS 50

// Enumeração dos estados possíveis dos LEDs
typedef enum {
    STATE_OFF,
    STATE_ALL_ON,
    STATE_TWO_ON,
    STATE_ONE_ON
} LedState;

// Variável que armazena o estado atual dos LEDs
volatile LedState currentState = STATE_OFF;

// Variável que indica se o botão foi pressionado
volatile bool buttonPressed = false;

// Variável que indica se o temporizador está em execução
volatile bool timerRunning = false;

// Função para definir o estado dos LEDs
void set_leds(bool green, bool blue, bool red) {
    // Define o estado do LED verde
    gpio_put(GREEN_PIN, green);
    // Define o estado do LED azul
    gpio_put(BLUE_PIN, blue);
    // Define o estado do LED vermelho
    gpio_put(RED_PIN, red);
}

// Função de callback chamada quando o alarme (temporizador) expira
int64_t alarm_callback(alarm_id_t id, void *user_data) {
    switch (currentState) {
        case STATE_ALL_ON:
            // Após 3 segundos, desliga o LED vermelho e mantém verde e azul ligados
            set_leds(true, true, false); // GREEN e BLUE ligados, RED desligado
            currentState = STATE_TWO_ON;
            break;
        case STATE_TWO_ON:
            // Após mais 3 segundos, desliga o LED azul e mantém apenas o verde ligado
            set_leds(true, false, false); // GREEN ligado, BLUE e RED desligados
            currentState = STATE_ONE_ON;
            break;
        case STATE_ONE_ON:
            // Após mais 3 segundos, desliga todos os LEDs
            set_leds(false, false, false); // Todos os LEDs desligados
            currentState = STATE_OFF;
            timerRunning = false; // Indica que o temporizador parou de rodar
            break;
        default:
            break;
    }

    // Se o estado atual não for OFF, configura um novo alarme para 3 segundos
    if (currentState != STATE_OFF) {
        add_alarm_in_ms(3000, alarm_callback, NULL, false);
    }

    return 0;
}

// Função de callback chamada quando o botão é pressionado
void button_callback(uint gpio, uint32_t events) {
    static uint64_t last_time = 0;  // Armazena o tempo da última leitura do botão
    uint64_t current_time = time_us_64();  // Obtém o tempo atual em microssegundos

    // Verifica se o tempo desde a última leitura é menor que o tempo de debounce
    if (current_time - last_time < DEBOUNCE_DELAY_MS * 1000) {
        return; // Ignora a leitura do botão
    }

    last_time = current_time;  // Atualiza o tempo da última leitura válida

    // Verifica se o botão foi pressionado e se o temporizador não está em execução
    if (!timerRunning && gpio_get(BUTTON_PIN)) {
        buttonPressed = true;  // Indica que o botão foi pressionado
    }
}

int main() {
    stdio_init_all();  // Inicializa a comunicação serial (para debug, se necessário)

    // Inicializa os pinos dos LEDs e do botão
    gpio_init(GREEN_PIN);
    gpio_init(BLUE_PIN);
    gpio_init(RED_PIN);
    gpio_init(BUTTON_PIN);

    // Configura os pinos dos LEDs como saída
    gpio_set_dir(GREEN_PIN, GPIO_OUT);
    gpio_set_dir(BLUE_PIN, GPIO_OUT);
    gpio_set_dir(RED_PIN, GPIO_OUT);

    // Configura o pino do botão como entrada
    gpio_set_dir(BUTTON_PIN, GPIO_IN);

    // Garante que o pino seja lido como alto
    gpio_pull_up(BUTTON_PIN);

    // Habilita a interrupção no pino do botão para detectar borda de subida (botão pressionado)
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    while (true) {
        // Verifica se o botão foi pressionado e se o temporizador não está em execução
        if (buttonPressed && !timerRunning) {
            // Liga todos os LEDs
            set_leds(true, true, true); // Todos os LEDs ligados
            // Atualiza o estado atual para indicar que todos os LEDs estão ligados
            currentState = STATE_ALL_ON;
            // Indica que o temporizador está em execução
            timerRunning = true;
            // Adiciona um alarme para desligar os LEDs após 3000 milissegundos (3 segundos)
            add_alarm_in_ms(3000, alarm_callback, NULL, false);
            // Reseta a variável que indica se o botão foi pressionado
            buttonPressed = false;
        }
        // Aguarda 10 milissegundos antes de verificar novamente
        sleep_ms(10);
    }

    return 0;
}
#include <stdio.h> // Biblioteca padrão de entrada e saída
#include "pico/stdlib.h" // Biblioteca padrão do SDK Pico C
#include "hardware/pwm.h" // Biblioteca com as funções de PWM


const uint SERVO_PIN = 22; // Pino do servo motor

// Valores para adquirir 50 Hz, baseado nos 125 MHz do clock
const uint16_t PERIOD = 39602; // Valor de WRAP
const float DIVISOR_CLK = 64.0; // Divisor fracionário do clock

const float degrees_0 = PERIOD * 0.025; // Duty Cycle de 2.5 %: posição de 0 graus do servo
const float degrees_90 = PERIOD * 0.073; // Duty Cycle de 7.3 %: posição de 0 graus do servo
const float degrees_180 = PERIOD * 0.12; // Duty Cycle de 12 %: posição de 0 graus do servo
const double step_servo = PERIOD * 0.00025; // Duty Cycle de 0.025 %: intervalo suave de movimento do servo
double dc_level = 0; // Define o nível do Duty Cycle 

// Configuração do PWM
void setup_pwm() {

    // Obtenção do slice e configuração da função de PWM para o pino do servo motor
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    
    // Obtenção da frequência e definição do PWM em 2.5%  (0 graus)
    pwm_set_clkdiv(slice, DIVISOR_CLK);
    pwm_set_wrap(slice, PERIOD);
    pwm_set_gpio_level(SERVO_PIN, degrees_0);
    
    // Ativa o PWM do slice adquirido
    pwm_set_enabled(slice, true);
}

/*Rotina inicial - ajusta o servo motor para a sequência de graus do servo motor com
5 segundos de espera para cada ângulo: 180°, 90°, 0°.*/
void init_routine() {
    pwm_set_gpio_level(SERVO_PIN, degrees_180);
    sleep_ms(5000);
    pwm_set_gpio_level(SERVO_PIN, degrees_90);
    sleep_ms(5000);
    pwm_set_gpio_level(SERVO_PIN, degrees_0);
    sleep_ms(5000);
}


int main() {
    stdio_init_all(); // Inicialização da entrada e saída
    setup_pwm();

    // Ativa a rotina inicial
    init_routine(); 
    
    // Define o sentido do duty cycle. 1 = subir; 0 = descer
    uint up_down = 1;

    // Inicia o servo motor em 0 graus
    dc_level = degrees_0;

    while (1) {
        pwm_set_gpio_level(SERVO_PIN, dc_level);
        
        // Se o sentido for para subir, o duty cycle aumenta em 0.025 %
        if(up_down){
            dc_level += step_servo;

            // Define o sentido de descida se o nível do duty cycle atingir os 180 graus
            if (dc_level >= degrees_180)
                up_down = 0; 
        }

        // Senão, o sentido será para descer. O duty cycle diminui em 0.025 %
        else{
            dc_level -= step_servo;

            // Define o sentido de subida se o nível do duty cycle atingir os 0 graus
            if (dc_level <= degrees_0)
                up_down = 1; 
        }
        
        // Atraso de 10 milissegundos
        sleep_ms(10);
    }
}

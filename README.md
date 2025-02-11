# Projeto de Aprendizado de PWM com Raspberry Pi Pico W

## Descrição
Este projeto tem como objetivo o aprendizado de PWM (Pulse Width Modulation) utilizando o Raspberry Pi Pico W, a placa educacional Bitdoglab e o simulador Wokwi. O sistema controla um servo motor através do PWM, permitindo a movimentação do eixo do servo em diferentes graus.

## PWM: Definição
PWM (Pulse Width Modulation, ou Modulação por Largura de Pulso) é uma técnica usada para controlar a potência entregue a dispositivos eletrônicos, variando a duração dos pulsos de um sinal digital. Em um sinal PWM, a frequência permanece constante, enquanto o tempo em que o sinal fica em nível alto (duty cycle) é ajustado para controlar a energia média enviada ao dispositivo. Na imagem, é ilustrado o cálculo do duty cycle.

![dutycycle](https://github.com/user-attachments/assets/928786e3-e2b1-451a-8948-57b63a9e37e4)

t = Tempo que o sinal fica em alto;

T = Perídodo total do sinal.

No caso de um servo motor, a posição do eixo é determinada pelo duty cycle do sinal PWM. Por exemplo, um duty cycle de 2,5% pode corresponder a 0°, enquanto um de 12% pode indicar 180°.

## Componentes Utilizados
- **Raspberry Pi Pico W**
- **Servo motor**
- **Simulador Wokwi**
- **Placa Bitdoglab**

## Funcionamento
O programa inicia com uma rotina de movimentação do servo motor para três posições fixas: 180°, 90° e 0°, com um intervalo de 5 segundos entre cada uma. Em seguida, entra em um loop infinito onde o servo oscila entre 0° e 180° de forma suave, aumentando ou diminuindo o duty cycle gradualmente.

## Configuração do PWM
Antes de configurar o PWM, vale destacar a fórmula para gerar a frequência desejada. Neste projeto, utilizou-se da frequência do clock de 125 MHz para gerar 50 Hz para o pino PWM do servo motor. A seguir, a fórmula: 

$$
f_{PWM} = \left( \frac{f_{clock}}{\left( \text{divisor inteiro} + \frac{\text{divisor fracionário}}{16} \right) \cdot \text{wrap}} \right)
$$

Passo a passo da configuração do PWM: 
1. Definir todos os valores para gerar 50 Hz. Wrap: 39602; Divisor fracionário: 64.0.
2. Obter o slice do pino e configurar o pino como PWM. Para o pino **GPIO 22**, o slice será **3A** (Este dado pode ser encontrado na página 522 do datasheet do RP2040).
3. Definir os valores de **wrap** e **divisor fracionário** e o nível inicial do **duty cycle**.
4. Ativar o PWM do slice.

Código utilizado no projeto para configurar o PWM:
```c
    // Obtenção do slice e configuração da função de PWM para o pino do servo motor
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    
    // Obtenção da frequência e definição do PWM em 2.5%  (0 graus)
    pwm_set_clkdiv(slice, DIVISOR_CLK);
    pwm_set_wrap(slice, PERIOD);
    pwm_set_gpio_level(SERVO_PIN, degrees_0);
    
    // Ativa o PWM do slice adquirido
    pwm_set_enabled(slice, true);
```

## Estrutura do Código

O código é organizado da seguinte maneira:

1. **Inclusão de Bibliotecas**  
   - São importadas bibliotecas essenciais para manipulação de GPIO e PWM no Raspberry Pi Pico W.

2. **Definição de Constantes**  
   - Configuração do pino do servo motor.  
   - Cálculo do período do PWM para obter uma frequência de 50 Hz.  
   - Definição dos valores de *duty cycle* para posicionamento do servo em 0°, 90° e 180°.

3. **Função `setup_pwm()`**  
   - Configura o PWM no pino especificado.  
   - Define a frequência e inicializa o servo na posição de 0°.

4. **Função `init_routine()`**  
   - Move o servo motor para 180°, depois 90° e, por fim, retorna a 0°, aguardando 5 segundos em cada posição.

5. **Função `main()`**  
   - Inicializa a comunicação serial.  
   - Chama a configuração do PWM e executa a rotina inicial.  
   - Entra em um loop infinito onde o servo oscila suavemente entre 0° e 180°.


## Execução no Wokwi
Para testar o projeto no simulador Wokwi:
1. Acesse o site [Wokwi](https://wokwi.com/).
2. Selecione a plataforma Raspberry Pi Pico.
3. Configure o pino 22 para o servo motor.
4. Copie e cole o código no editor do Wokwi.
5. Inicie a simulação e observe o movimento do servo motor.

## Vídeos de Demonstração


## Datasheet do RP2040: [[RP2040 Datasheet – Raspberry Pi](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)




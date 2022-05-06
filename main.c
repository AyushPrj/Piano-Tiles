#include "stm32f0xx.h"
#include "lcd.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void init_tiles(void);
void nano_wait(unsigned int n);
void move_tiles(int tile_num);

int score = 0; // global variable to keep track of the score
char score_string[10]; // global variable to store the score as a string for display on the LCD

// Function to initialize the LCD display using SPI communication
void init_lcd_spi(void){
    // enable clock for GPIOB
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    // configure GPIOB pins for SPI communication
    GPIOB->MODER &= ~0x30c30000;
    GPIOB->MODER |= 0x10410000;
    GPIOB->ODR |= 0x4900;

    // configure GPIOB pin for LCD reset
    GPIOB->MODER &= ~0xc0;
    GPIOB->MODER |= 0x80;

    // configure GPIOB pin for LCD DC
    GPIOB->MODER &= ~0xc00;
    GPIOB->MODER |= 0x800;

    // configure alternate function for SPI pins
    GPIOB->AFR[0] &= ~0xf0f000;

    // enable clock for SPI1
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    // disable SPI1
    SPI1->CR1 &= ~SPI_CR1_SPE;
    // configure SPI1 as master
    SPI1->CR1 &= ~0x3f;
    SPI1->CR1 |= SPI_CR1_MSTR;
    // configure data size for SPI1
    SPI1->CR2 = SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0;
    // configure software slave management and internal slave select for SPI1
    SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;
    // enable SPI1
    SPI1->CR1 |= SPI_CR1_SPE;
}

// Function to initialize timer 7
void init_tim7(void){
    // enable clock for timer 7
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
    // set prescaler value for timer 7
    TIM7->PSC = 0;
    // set auto-reload value for timer 7
    TIM7->ARR = 480-1;
    // enable update interrupt for timer 7
    TIM7->DIER |= TIM_DIER_UIE;
    // enable timer 7
    TIM7->CR1 |= TIM_CR1_CEN;
    // enable interrupt in NVIC for timer 7
    NVIC->ISER[0] = 1<<18;

}

// Function to initialize timer 6
void init_tim6(void){
    // enable clock for timer 6
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
     // set prescaler value for timer 6
     TIM6->PSC = 0;
     // set auto-reload value for timer 6
     TIM6->ARR = 480-1;
     // enable update interrupt for timer 6
     TIM6->DIER |= TIM_DIER_UIE;
     // enable timer 6
     TIM6->CR1 |= TIM_CR1_CEN;
     // enable interrupt in NVIC for timer 6
     NVIC->ISER[0] = 1<<17;

}

// interrupt handler for timer 6 DAC interrupt (shared with timer 6)
void TIM6_DAC_IRQHandler(void){
    int start = (GPIOA->IDR) & 1; // read input from GPIOA pin and mask with bit mask to get the value of a specific pin

    if (start == 1){ 
    init_tim7(); // if the value of the specific pin is high, initialize timer 7 
    TIM6->CR1 &= ~TIM_CR1_CEN; // and disable timer 6 
    }
}

// function to initialize timer 15
void init_tim15(void){
    // enable clock for timer 15
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    // set prescaler value for timer 15
    TIM15->PSC = 0;
    // set auto-reload value for timer 15
    TIM15->ARR = 4800-1;
    // enable update interrupt for timer 15
    TIM15->DIER |= TIM_DIER_UIE;
    // enable timer 15
    TIM15->CR1 |= TIM_CR1_CEN;
    // enable interrupt in NVIC for timer 15
    NVIC->ISER[0] = 1<<20;
}

// interrupt handler for timer 7
void TIM7_IRQHandler(void){

    // clear update interrupt flag for timer 7
    TIM7->SR &= ~(1<<0);

    int r = rand() % 10; // generate a random number between 0 and 9
    move_tiles(r); // move tiles based on the random number generated
}

#define STEP22
#if defined(STEP22)
// interrupt handler for timer 15
void TIM15_IRQHandler(void){
    // clear update interrupt flag for timer 15
    TIM15->SR &= ~(1<<0);
    int a = (GPIOA->IDR) & 1; // read input from GPIOA pin and mask with bit mask to get the value of a specific pin

    if (a == 1){
        spi1_display1("Hello"); // display "Hello" on the LCD if a certain condition is met
    }
}
#endif

// function to setup buttons connected to GPIOC pins
void setup_buttons(void){
    // enable clock for GPIOC
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    // configure GPIOC pins as inputs with pull-up resistors
    GPIOC->MODER &= ~0xffff;
    GPIOC->MODER |= 0x55 << (4*2);
    GPIOC->OTYPER &= ~0xf0;
    GPIOC->OTYPER |= 0xf0;
    GPIOC->PUPDR &= ~0xff;
    GPIOC->PUPDR |= 0x55;

    // enable clock for GPIOA and configure a specific pin as input with pull-up resistor
    RCC->AHBENR |= 1<<17;
    GPIOA->PUPDR |= 1<<1;
}

// function to initialize SPI1 communication
void init_spi1() {
     // enable clock for GPIOB and configure pins for SPI communication
     RCC->AHBENR |= 1<<19;
     RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
     GPIOB->MODER |= GPIO_MODER_MODER13_1;
     GPIOB->MODER &= ~GPIO_MODER_MODER13_0;
     GPIOB->MODER |= GPIO_MODER_MODER15_1;
     GPIOB->MODER &= ~GPIO_MODER_MODER15_0;
     GPIOB->MODER |= GPIO_MODER_MODER12_1;
     GPIOB->MODER &= ~GPIO_MODER_MODER12_0;

     // enable clock for SPI2 and configure it as master with specific baud rate and data size
     RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

     SPI2->CR1 &= ~SPI_CR1_SPE;

     SPI2->CR1 |= SPI_CR1_BR;

     SPI2->CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_0 | SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_TXDMAEN;

     SPI2->CR1 |= SPI_CR1_MSTR;

     SPI2->CR1 |= SPI_CR1_SPE;

}

// function to send a command to the LCD using SPI communication
void spi_cmd(unsigned int data) {
    while(!(SPI2->SR & SPI_SR_TXE)) {} // wait until transmit buffer is empty
    SPI2->DR = data; // send data to the LCD
}

// function to send data to the LCD using SPI communication
void spi_data(unsigned int data) {
    spi_cmd(data | 0x200); // send data with specific bit set to indicate that it is data and not a command 
}

// function to initialize the OLED display connected to the microcontroller using SPI communication
void spi1_init_oled() {
    nano_wait(1000000); // wait for 1 second
    spi_cmd(0x38); // send function set command to the LCD
    spi_cmd(0x08); // send display off command to the LCD
    spi_cmd(0x01); // send clear display command to the LCD
    nano_wait(2000000); // wait for 2 seconds
    spi_cmd(0x06); // send entry mode set command to the LCD
    spi_cmd(0x02); // send return home command to the LCD
    spi_cmd(0x0c); // send display on command to the LCD
}

// function to display a string on the first line of the LCD
void spi1_display1(const char *string) {
    spi_cmd(0x02); // send return home command to the LCD
    while(*string != '\0') { // loop through each character in the string
        spi_data(*string); // send character data to the LCD
        string++; // move to the next character in the string
    }
}

// function to display a string on the second line of the LCD
void spi1_display2(const char *string) {
    spi_cmd(0xc0); // send set DDRAM address command to the LCD to move cursor to the second line
    while(*string != '\0') { // loop through each character in the string
        spi_data(*string); // send character data to the LCD
        string++; // move to the next character in the string
    }
}

// main function of the program
int main(void){
    setup_buttons(); // call function to setup buttons connected to GPIOC pins
    LCD_Setup(); // call function to setup LCD display connected to microcontroller using GPIO pins
    init_tiles(); // call function to initialize tiles

    init_spi1(); // call function to initialize SPI1 communication with OLED display connected to microcontroller using GPIO pins 
    spi1_init_oled(); // call function to initialize OLED display connected to microcontroller using SPI communication 
    spi1_display1("Press any button to start"); // call function to display a message on the first line of OLED display 
    spi1_display2("Done with the project"); // call function to display a message on the second line of OLED display 

    init_tim15(); // call function to initialize timer 15 
}

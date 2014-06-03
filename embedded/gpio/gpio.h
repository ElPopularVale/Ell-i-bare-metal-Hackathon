#ifndef GPIO_H
#define GPIO_H

#define IO6                 GPIOA, GPIO_Pin_2
#define IO7                 GPIOA, GPIO_Pin_3
#define IO8                 GPIOB, GPIO_Pin_10
#define IO9                 GPIOB, GPIO_Pin_11
#define IO10                 GPIOA, GPIO_Pin_15
#define IO11                 GPIOB, GPIO_Pin_5
#define IO12                 GPIOB, GPIO_Pin_4
#define IO13                 GPIOB, GPIO_Pin_3

void LED_init();
void initPulsioximeter();
void readPulsioximeter();
uint16_t getOxygenSaturation(void);
uint16_t getBPM(void);
uint8_t segToNumber(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G );
int toInt(char a[]);
void printNumber(int number);
inline void delay_us(uint32_t us);

#endif

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_exti.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_syscfg.h"
#include "gpio.h"

uint16_t SPO2;
uint16_t BPM;


void initPulsioximeter() {
	GPIO_InitTypeDef gpio;

	//Enable GPIOA and GPIOB peripheral clock
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_StructInit(&gpio);

	//Port A
	//IO7
	gpio.GPIO_Pin = GPIO_Pin_3;
	gpio.GPIO_Mode = GPIO_Mode_IN;
	gpio.GPIO_Speed = GPIO_Speed_Level_1;
	GPIO_Init(GPIOA, &gpio);

	//IO10
	gpio.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &gpio);

	//IO6
	gpio.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &gpio);

	//Port B
	//IO8
	gpio.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &gpio);

	//IO9
	gpio.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &gpio);

	//IO11
	gpio.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &gpio);

	//IO12
	gpio.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB, &gpio);

	//IO13
	gpio.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &gpio);

	//Interrupt at PA2 (IO6)
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource2);
	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void readPulsioximeter() {
	uint16_t digito[6];

	uint8_t A = 0;
	uint8_t B = 0;
	uint8_t C = 0;
	uint8_t D = 0;
	uint8_t E = 0;
	uint8_t F = 0;
	uint8_t G = 0;
	int i = 0;
	uint8_t intPin = 0;
	uint8_t lastIntPin = 1;

	for (i = 0; i < 6; i++) {
		A = !GPIO_ReadInputDataBit(IO13);
		B = !GPIO_ReadInputDataBit(IO12);
		C = !GPIO_ReadInputDataBit(IO11);
		D = !GPIO_ReadInputDataBit(IO10);
		E = !GPIO_ReadInputDataBit(IO9);
		F = !GPIO_ReadInputDataBit(IO8);
		G = !GPIO_ReadInputDataBit(IO7);

		digito[i] = segToNumber(A, B, C, D, E, F, G);
		delay_us(2800 / 6);
	}
	SPO2 = 10 * digito[5] + digito[4];
	BPM = 100 * digito[2] + 10 * digito[1] + digito[0];
}

uint16_t getOxygenSaturation() {
	return SPO2;
}

uint16_t getBPM() {
	return BPM;
}

uint8_t segToNumber(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E,
		uint8_t F, uint8_t G) {
	if ((A == 1) && (B == 1) && (C == 1) && (D == 0) && (E == 1) && (F == 1)
			&& (G == 1)) {
		return 0;

	} else if ((A == 0) && (B == 1) && (C == 0) && (D == 0) && (E == 1)
			&& (F == 0) && (G == 0)) {
		return 1;

	} else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 0)
			&& (F == 1) && (G == 1)) {
		return 2;

	} else if ((A == 1) && (B == 1) && (C == 0) && (D == 1) && (E == 1)
			&& (F == 0) && (G == 1)) {
		return 3;

	} else if ((A == 0) && (B == 1) && (C == 1) && (D == 1) && (E == 1)
			&& (F == 0) && (G == 0)) {
		return 4;

	} else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1)
			&& (F == 0) && (G == 1)) {
		return 5;

	} else if ((A == 1) && (B == 0) && (C == 1) && (D == 1) && (E == 1)
			&& (F == 1) && (G == 1)) {
		return 6;

	} else if ((A == 1) && (B == 1) && (C == 0) && (D == 0) && (E == 1)
			&& (F == 0) && (G == 0)) {
		return 7;

	} else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1)
			&& (F == 1) && (G == 1)) {
		return 8;

	} else if ((A == 1) && (B == 1) && (C == 1) && (D == 1) && (E == 1)
			&& (F == 0) && (G == 1)) {
		return 9;

	} else {
		return 0;
	}
}

int toInt(char a[]) {
	int c, sign, offset, n;

	if (a[0] == '-') { // Handle negative integers
		sign = -1;
	}

	if (sign == -1) { // Set starting position to convert
		offset = 1;
	} else {
		offset = 0;
	}

	n = 0;

	for (c = offset; a[c] != '\0'; c++) {
		n = n * 10 + a[c] - '0';
	}

	if (sign == -1) {
		n = -n;
	}

	return n;
}

void printNumber(int number) {
	char revertedstr[32];
	char str[32];
	int length = 0;

	while (number > 0) {
		int a = number % 10;
		revertedstr[length++] = a | '0';
		number /= 10;
	}

	length--;
	int rev = 0;
	while (length >= 0) {
		str[rev++] = revertedstr[length--];
	}
	usartSendString(str);
}

inline void delay_us(uint32_t us) {
	/* So (2^32)/12 micros max, or less than 6 minutes */
	us *= 12;
	us -= 2; //offset seems around 2 cycles
	/* fudge for function call overhead */
	us--;
	__asm volatile(" mov r0, %[us] \n\t"
			".syntax unified \n\t"
			"1: subs r0, #1 \n\t"
			".syntax divided \n\t"
			" bhi 1b \n\t"
			:
			: [us] "r" (us)
			: "r0");
}


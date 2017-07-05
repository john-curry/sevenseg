#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "cmsis/cmsis_device.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "ctype.h"
#include "diag/Trace.h"
#include <sys/stat.h>

void init_gpio();
void delay();
void write_num(int n);
bool button_up();

static uint16_t segs[10] = {
		0b1111110,
		0b0110000,
		0b1101101,
		0b1111001,
		0b0110011,
		0b1011011,
		0b1011111,
		0b1110000,
		0b1111111,
		0b1111011
};

int main(void) {
	init_gpio();
	int i = 0;
	while(true) {
		write_num(i);
		delay();
		while (button_up());
		i = i < 9 ? i + 1: 0;
	}
	return 0;
}

bool button_up() {
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
}
void delay() {
	volatile int count = 10000000;
	while(count--);
}

void write_num(int n) {
	HAL_GPIO_WritePin(GPIOA, 0b1111111, DISABLE);
	HAL_GPIO_WritePin(GPIOA, segs[n], ENABLE);
}

void init_gpio(void) {
	HAL_Init();
	__GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init;
	GPIO_Init.Pin = 0b1111111;
	GPIO_Init.Speed = GPIO_SPEED_MEDIUM;
	GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Alternate = 0;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	__GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_Init_In;
	GPIO_Init_In.Pin = GPIO_PIN_0;
	GPIO_Init_In.Speed = GPIO_SPEED_MEDIUM;
	GPIO_Init_In.Mode = GPIO_MODE_INPUT;
	GPIO_Init_In.Pull = GPIO_NOPULL;
	GPIO_Init_In.Alternate = 0;
	HAL_GPIO_Init(GPIOB, &GPIO_Init_In);
}

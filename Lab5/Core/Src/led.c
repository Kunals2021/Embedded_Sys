#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern RTC_HandleTypeDef hrtc;
extern RTC_TimeTypeDef sTime = { 0 };
extern RTC_DateTypeDef sDate = { 0 };
extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart2;
extern ADC_ChannelConfTypeDef sConfig = { 0 };
extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch1;
extern captureDone;
//https://stackoverflow.com/questions/28334435/stm32-printf-float-variable

void lon(void) {
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

void lof(void) {
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}

void lon_command(char *arguments) {
	lon();
	printf("OK\n\r\n\r");
}

void lof_command(char *arguments) {
	lof();
	printf("OK\n\r\n\r");
}

void ts_command(char *arguments) {
	//printf("Set time real command\n\r");
	char *arg;
	int arg_cnt = 0;
	arg = strtok(arguments, ",");
	while (arg) {
		switch (arg_cnt) {
		case 0:
			sTime.Hours = (int) strtol(arg, NULL, 10);
			break;
		case 1:
			sTime.Minutes = (int) strtol(arg, NULL, 10);
			break;
		case 2:
			sTime.Seconds = (int) strtol(arg, NULL, 10);
			break;
		}
		arg_cnt++;
		arg = strtok(NULL, ",");
	}
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}
	printf("OK\n\r\n\r");
}

void ds_command(char *arguments) {
	//printf("Set date real command\n\r");
	char *arg;
	int arg_cnt = 0;
	arg = strtok(arguments, ",");

	while (arg) {
		switch (arg_cnt) {
		case 0:
			sDate.Month = (int) strtol(arg, NULL, 10);
			break;
		case 1:
			sDate.Date = (int) strtol(arg, NULL, 10);
			break;
		case 2:
			sDate.Year = (int) strtol(arg, NULL, 10);
			break;
		}
		arg_cnt++;
		arg = strtok(NULL, ",");
	}
	if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
	}

	printf("OK\n\r\n\r");
}

void sensor_command(char *arguments) {
	//printf("Light Sensor Frequency\n\r");
	float frequency = 0;
	uint32_t captures[2];

	uint32_t diffCapture = 0;
	char msg[30];
	printf("captureDone: %d\n\r", captureDone);
	HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t*) captures, 2);
	//while (1) {
		if (captureDone != 0) {
			if (captures[1] >= captures[0])
				diffCapture = captures[1] - captures[0];
			else
				diffCapture = (htim2.Instance->ARR - captures[0]) + captures[1];

			frequency = HAL_RCC_GetHCLKFreq() / (htim2.Instance->PSC + 1);
			frequency = (float) frequency / diffCapture;

			//sprintf(msg, "Input frequency: %.2f\r\n", frequency);
			//HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg),
			//		HAL_MAX_DELAY);
			//while(1);
		}
	//} //end while
	printf("%0.2f hz\n\r", frequency);
	printf("OK\n\r\n\r");
	//captureDone = 0;
}

void temperature_command(char *arguments) {
	//printf("Processor Temperature\n\r");
	char msg[20];
	uint16_t rawValue;
	float temp;

	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_Start(&hadc1); // starts ADC
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	rawValue = HAL_ADC_GetValue(&hadc1);
	temp = (float) rawValue;
	temp = ((float) rawValue) / 4095 * 3300;
	temp = ((temp - 760.0) / 2.5) + 25;

	//sprintf(msg, "ADC rawValue: %hu\r\n", rawValue);
	//HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
	sprintf(msg, "%.1f C\r\n", temp);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	HAL_ADC_Stop(&hadc1);
	printf("OK\n\r\n\r");
}

void battery_command(char *arguments) {
	//printf("Battery Voltage\n\r");

	char msg[30];
	uint32_t rawVintref;
	uint32_t vref;
	float batt;

	sConfig.Channel = ADC_CHANNEL_VREFINT;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_Start(&hadc1); // starts ADC
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	rawVintref = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	vref = __HAL_ADC_CALC_VREFANALOG_VOLTAGE(rawVintref, ADC_RESOLUTION_12B);
	batt = vref / 1000.0;

	//sprintf(msg, "ADC rawVintref: %hu\r\n", rawVintref);
	//HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);
	sprintf(msg, "%.2f V\r\n", batt);
	HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	printf("OK\n\r\n\r");
}






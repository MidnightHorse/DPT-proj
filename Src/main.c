#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "GUI_Paint.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bme280.h"
#include "epd2in13.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t adc_val = 0;
//unsigned char image[250 * 122 / 8];
UBYTE *BlackImage;
UWORD Imagesize;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM2_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void I2C_Scanner(void){
	char msg[64];
	uint8_t devices_found = 0;

	sprintf(msg, "\r\n === I2C Scanner ===\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	for(uint8_t i = 0; i < 128; i++){
		if(HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10) == HAL_OK){
			sprintf(msg, "\r\n Device found at 0x%02X\r\n", i);
			HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
			devices_found++;
		}
	}
	if(devices_found == 0){
		sprintf(msg, "No I2C devices found\r\n");
	}
	else{
		sprintf(msg, "\r\nTotal Devices found: %d\r\n", devices_found);
	}
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  /*HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_Base_Start_IT(&htim2);*/
  char msg[100];

  sprintf(msg, "\r\n=== Weather Station Starting ===\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  // Initialize BME280 sensor
  sprintf(msg, "Initializing BME280...\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  BME280_Init(&hi2c1);
  HAL_Delay(1000);

  // Initialize E-Paper display
  sprintf(msg, "Initializing E-Paper...\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  DEV_Module_Init();
  EPD_2in13_V4_Init();

  //image buffer
  Imagesize = ((EPD_2in13_V4_WIDTH % 8 == 0) ?
               (EPD_2in13_V4_WIDTH / 8) : (EPD_2in13_V4_WIDTH / 8 + 1)) * EPD_2in13_V4_HEIGHT;

  BlackImage = (UBYTE *)malloc(Imagesize);
  if(BlackImage == NULL) {
      sprintf(msg, "ERROR: Memory allocation failed!\r\n");
      HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
      while(1);
  }

  sprintf(msg, "Clearing display...\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  EPD_2in13_V4_Clear();
  HAL_Delay(500);

  //state change
  EPD_2in13_V4_Init_Fast();

  sprintf(msg, "Weather station ready!\r\n\r\n");
  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

  /*BME280_Init(&hi2c1);
  HAL_Delay(1000);
  BME280_ReadTemperature();
  BME280_ReadPressure();
  BME280_ReadHumidity();
  HAL_Delay(500);*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  float temperature = BME280_ReadTemperature();
	  float pressure = BME280_ReadPressure();
	  float humidity = BME280_ReadHumidity();

	  float temperature_f = (temperature * 9.0f / 5.0f) + 32.0f;

	  //log to UART
	  sprintf(msg, "T:%.1fC P:%.1fhPa H:%.1f%%\r\n", temperature, pressure, humidity);
	  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	  //display
	  Paint_NewImage(BlackImage, EPD_2in13_V4_WIDTH, EPD_2in13_V4_HEIGHT, 270, BLACK);
	  Paint_Clear(WHITE);

	  //title
	  Paint_DrawString_EN(40, 5, "THERMOSTAT", &Font24, BLACK, WHITE);
	  Paint_DrawLine(10, 30, 240, 30, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

	  //temperature
	  char temp_str[50];
	  sprintf(temp_str, "Temp: %.1fC / %.1fF", temperature, temperature_f);
	  Paint_DrawString_EN(10, 40, temp_str, &Font16, BLACK, WHITE);

	  //Pressure
	  char press_str[50];
	  sprintf(press_str, "Press: %.1f hPa", pressure);
	  Paint_DrawString_EN(10, 65, press_str, &Font16, BLACK, WHITE);

	  //Humidity
	  char humid_str[50];
	  sprintf(humid_str, "Humid: %.1f %%", humidity);
	  Paint_DrawString_EN(10, 90, humid_str, &Font16, BLACK, WHITE);

	  //border
	  Paint_DrawRectangle(5, 35, 245, 115, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

	  //display refresh
	  EPD_2in13_V4_Display_Fast(BlackImage);

	  sprintf(msg, "Display updated!\r\n");
	  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);

	  HAL_Delay(30000);
  }
  /* USER CODE END 3 */
}

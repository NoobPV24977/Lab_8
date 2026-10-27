#include "MY_LIS3DSH.h"

// Стандартний пін Chip Select (CS) для акселерометра на STM32F407-Discovery
#define LIS3DSH_CS_PORT GPIOE
#define LIS3DSH_CS_PIN  GPIO_PIN_3

SPI_HandleTypeDef *lis3dsh_spi;

// Локальна функція для запису в регістр
static void LIS3DSH_WriteReg(uint8_t reg, uint8_t data) {
    uint8_t tx_data[2] = {reg, data}; // MSB = 0 для запису
    HAL_GPIO_WritePin(LIS3DSH_CS_PORT, LIS3DSH_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(lis3dsh_spi, tx_data, 2, 50);
    HAL_GPIO_WritePin(LIS3DSH_CS_PORT, LIS3DSH_CS_PIN, GPIO_PIN_SET);
}

// Локальна функція для читання з регістрів
static void LIS3DSH_ReadRegs(uint8_t reg, uint8_t *data, uint16_t size) {
    uint8_t tx_data = reg | 0x80; // MSB = 1 для читання
    HAL_GPIO_WritePin(LIS3DSH_CS_PORT, LIS3DSH_CS_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(lis3dsh_spi, &tx_data, 1, 50);
    HAL_SPI_Receive(lis3dsh_spi, data, size, 50);
    HAL_GPIO_WritePin(LIS3DSH_CS_PORT, LIS3DSH_CS_PIN, GPIO_PIN_SET);
}

// Ініціалізація датчика
void LIS3DSH_Init(SPI_HandleTypeDef *hspi) {
    lis3dsh_spi = hspi;
    // Вмикаємо осі X, Y, Z та встановлюємо частоту 100 Гц (регістр CTRL_REG4 = 0x20, значення 0x67)
    LIS3DSH_WriteReg(0x20, 0x67);
}

// Зчитування даних
void LIS3DSH_GetData(LIS3DSH_Data_t *data) {
    uint8_t rx_data[6];

    // Читаємо 6 байт координат починаючи з регістру OUT_X_L (0x28)
    LIS3DSH_ReadRegs(0x28, rx_data, 6);

    // Збираємо 16-бітні значення з двох 8-бітних регістрів
    data->X = (int16_t)((rx_data[1] << 8) | rx_data[0]);
    data->Y = (int16_t)((rx_data[3] << 8) | rx_data[2]);
    data->Z = (int16_t)((rx_data[5] << 8) | rx_data[4]);
}

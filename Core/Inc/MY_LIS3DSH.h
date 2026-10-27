#ifndef INC_MY_LIS3DSH_H_
#define INC_MY_LIS3DSH_H_

#include "main.h"
#include "spi.h"

// Структура для зберігання координат
typedef struct {
    int16_t X;
    int16_t Y;
    int16_t Z;
} LIS3DSH_Data_t;

// Прототипи функцій
void LIS3DSH_Init(SPI_HandleTypeDef *hspi);
void LIS3DSH_GetData(LIS3DSH_Data_t *data);

#endif /* INC_MY_LIS3DSH_H_ */

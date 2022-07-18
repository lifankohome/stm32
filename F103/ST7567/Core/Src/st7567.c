#include "st7567.h"
#include "font.h"
#include "string.h"


#define screen_width 120
#define screen_height 48

#define delay_ms(n) HAL_Delay(n)

#define LCD_RST_LOW() HAL_GPIO_WritePin(rst_GPIO_Port, rst_Pin, GPIO_PIN_RESET)
#define LCD_RST_HIGH() HAL_GPIO_WritePin(rst_GPIO_Port, rst_Pin, GPIO_PIN_SET)

#define LCD_CS_LOW() HAL_GPIO_WritePin(cs_GPIO_Port, cs_Pin, GPIO_PIN_RESET)
#define LCD_CS_HIGH() HAL_GPIO_WritePin(cs_GPIO_Port, cs_Pin, GPIO_PIN_SET)

#define LCD_RS_LOW() HAL_GPIO_WritePin(dc_GPIO_Port, dc_Pin, GPIO_PIN_RESET)
#define LCD_RS_HIGH() HAL_GPIO_WritePin(dc_GPIO_Port, dc_Pin, GPIO_PIN_SET)

#define LCD_SCL_LOW() HAL_GPIO_WritePin(sclk_GPIO_Port, sclk_Pin, GPIO_PIN_RESET)
#define LCD_SCL_HIGH() HAL_GPIO_WritePin(sclk_GPIO_Port, sclk_Pin, GPIO_PIN_SET)

#define LCD_SDA_LOW() HAL_GPIO_WritePin(sda_GPIO_Port, sda_Pin, GPIO_PIN_RESET)
#define LCD_SDA_HIGH() HAL_GPIO_WritePin(sda_GPIO_Port, sda_Pin, GPIO_PIN_SET)

void delay_us(uint16_t us) {
    uint8_t i;

    while (us--) {
        i = 0x08;
        while (--i);
    }
}

void LCD_Writ_Bus(uint8_t dat) {
    uint8_t i;

    for (i = 0; i < 8; i++) {
        LCD_SCL_LOW();
        delay_us(1);

        if (dat & 0x80) {
            LCD_SDA_HIGH();
        } else {
            LCD_SDA_LOW();
        }

        LCD_SCL_HIGH();
        delay_us(1);
        dat = dat << 1;
    }
}

void LCD_WR_REG(uint8_t cmd) {
    LCD_CS_LOW();
    LCD_RS_LOW();
    LCD_Writ_Bus(cmd);
    LCD_CS_HIGH();
}

void LCD_WR_DATA(uint8_t dat) {
    LCD_CS_LOW();
    LCD_RS_HIGH();
    LCD_Writ_Bus(dat);
    LCD_CS_HIGH();
}

void LCD_SetPos(uint8_t page, uint8_t column) {
    LCD_WR_REG(0xb0 + page);
    LCD_WR_REG(column & 0x0f);
    column >>= 4;
    column = column | 0x10;
    LCD_WR_REG(column);
}

void LCD_Clear(void) {
    uint8_t page, col;

    for (page = 0; page < screen_height / 8; page++) {
        LCD_SetPos(page, 0);

        for (col = 0; col < screen_width; col++) {
            LCD_WR_DATA(0x00);
        }
    }

    LCD_SetPos(8, 0);
}

void icon_set(uint8_t icon) {
    //static uint8_t icon_data = 0x00;
    uint8_t col;

    //icon_data = icon;
    if (icon & 0x80) {
        // no buffer
        for (col = 0; col < screen_width; col++) {
            LCD_WR_DATA(0x00);
        }
    }
}

void st7567_init() {
    LCD_RST_LOW();
    delay_us(100);
    LCD_RST_HIGH();
    delay_ms(100);

    LCD_WR_REG(0X2F);
    LCD_WR_REG(0X81);
    LCD_WR_REG(0X28);
    LCD_WR_REG(0XEB);
    LCD_WR_REG(0XC8);
    LCD_WR_REG(0X40);
    LCD_WR_REG(0XAF);
}

void disp_char(uint8_t x, uint8_t y, char ch) {
    uint8_t i;

    LCD_SetPos(y, x);

    for (i = 0; i < font6x8[1]; i++) {
        LCD_WR_DATA(font6x8[(ch - 31) * 6 + i]);
    }
}

void disp_str(uint8_t x, uint8_t y, char *str) {
    uint8_t i;
    uint8_t len = strlen(str);

    for (i = 0; i < len; i++) {
        disp_char(x + i * font6x8[1], y, *str++);
    }
}

void st7567_display() {
    LCD_Clear();
    icon_set(0x80);

    disp_str(0, 0, "Sys Starting...");
}

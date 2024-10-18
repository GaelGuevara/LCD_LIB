#include "i2cLCD.h"
#include "MKL25Z4.h"

#define LCD_ADDRESS 0x27

int rowAdd[2]={0x00, 0x40};

void LCD_sendCmd(uint8_t command){
    uint8_t parteMS = command & 0xF0;
    uint8_t parteLS = (command << 4) &0xF0;

    LCD_write(parteMS | 0x08);
    LCD_write(parteLS | 0x08);
}

void LCD_init(){
    SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM -> SCGC4 |= SIM_SCGC4_I2C0_MASK;
    PORTE -> PCR[8] = PORT_PCR_MUX(2);
    PORTE -> PCR[9] = PORT_PCR_MUX(2);
    I2C0 -> C1 = 0;
    I2C0 -> F = 0x14;
    I2C0 -> C1 = I2C_C1_IICEN_MASK;
    LCD_sendCmd(0x0F);
}
void LCD_clear(){
    LCD_sendCmd(0x01);
}

void LCD_scrollDisplayLeft(){
    LCD_sendCmd(0x07);
}
void LCD_scrollDisplayRight(){
    LCD_sendCmd(0x05);
}

void LCD_write(uint8_t data){
    I2C0->C1 |= I2C_C1_TX_MASK;  //modo de transmision
    I2C0->C1 |= I2C_C1_MST_MASK; // bit de inicio
    
    I2C0->D = (LCD_ADDRESS << 1); //digo el address del modulo a usar 
    while (!(I2C0->S & I2C_S_IICIF_MASK)); // espero a que termine la interrupcion
    I2C0->S |= I2C_S_IICIF_MASK; // limpia el flag

    I2C0->D = data;
    while (!(I2C0->S & I2C_S_IICIF_MASK));
    I2C0->S |= I2C_S_IICIF_MASK;

    I2C0->C1 |= ~I2C_C1_TX_MASK;
    I2C0->C1 |= ~I2C_C1_MST_MASK;
}

void LCD_setCursor(int row, int col){
        int address = rowAdd[row] + col;
        LCD_sendCmd(address | 0x80); 
}
void LCD_createChar(uint8_t nByte, uint8_t nuevoCaracter[]){
    nByte &= 0x7;
    
}

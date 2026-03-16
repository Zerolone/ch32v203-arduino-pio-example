#include "ch32v20x.h"
#include "ch32v20x_gpio.h"
#include "ch32v20x_spi.h"
#include "ch32v20x_rcc.h"
#include <stdint.h>

// 屏幕参数
#define ST7735_WIDTH  128
#define ST7735_HEIGHT 160
#define COLOR_WHITE   0xFFFF
#define COLOR_RED     0xF800

// 引脚定义
#define RES_PIN    GPIO_Pin_0
#define DC_PIN     GPIO_Pin_1
#define CS_PIN     GPIO_Pin_2
#define BL_PIN     GPIO_Pin_3
#define GPIO_PORT  GPIOA

/************************* 三线 SPI 核心配置 *************************/
void SPI_Init_3Wire(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;
    
    // 1. 使能时钟（仅需 SCK/MOSI 时钟，无需 MISO）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);
    
    // 2. 配置 SCK(PA5)、MOSI(PA7) 为复用推挽输出（无 MISO！）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 3. 三线 SPI 核心配置（仅发送模式）
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx; // 仅发送（关键！三线SPI核心）
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;             // 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;         // 8位数据
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                // 时钟极性：低电平空闲
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;              // 时钟相位：第一个边沿采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                 // 软件片选（省CS引脚也可接GND）
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // 18MHz（72/4）
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;        // 高位先行
    SPI_Init(SPI1, &SPI_InitStructure);
    
    // 4. 使能 SPI
    SPI_Cmd(SPI1, ENABLE);
}

// SPI 发送单字节（三线 SPI 仅发送，无需接收）
void SPI_Send_Byte(uint8_t byte)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); // 等待发送缓冲区空
    SPI_I2S_SendData(SPI1, byte);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);   // 等待发送完成
}

/************************* ST7735S 基础驱动 *************************/
void GPIO_Init_Screen(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 配置 RES/DC/CS/BL 为推挽输出
    GPIO_InitStructure.GPIO_Pin = RES_PIN | DC_PIN | CS_PIN | BL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
    
    // 初始电平
    GPIO_SetBits(GPIO_PORT, RES_PIN); // 复位释放
    GPIO_SetBits(GPIO_PORT, CS_PIN);  // 片选释放
    GPIO_SetBits(GPIO_PORT, BL_PIN);  // 点亮背光
}

void ST7735_Write_Cmd(uint8_t cmd)
{
    GPIO_ResetBits(GPIO_PORT, CS_PIN);
    GPIO_ResetBits(GPIO_PORT, DC_PIN); // DC=0：命令
    SPI_Send_Byte(cmd);
    GPIO_SetBits(GPIO_PORT, CS_PIN);
}

void ST7735_Write_Data(uint8_t data)
{
    GPIO_ResetBits(GPIO_PORT, CS_PIN);
    GPIO_SetBits(GPIO_PORT, DC_PIN);   // DC=1：数据
    SPI_Send_Byte(data);
    GPIO_SetBits(GPIO_PORT, CS_PIN);
}

void ST7735_Init(void)
{
    GPIO_Init_Screen();
    SPI_Init_3Wire();
    
    // 硬件复位
    GPIO_ResetBits(GPIO_PORT, RES_PIN);
    for(uint32_t i=0; i<10000; i++);
    GPIO_SetBits(GPIO_PORT, RES_PIN);
    for(uint32_t i=0; i<10000; i++);
    
    // ST7735S 初始化指令（固定流程）
    ST7735_Write_Cmd(0x11); // 退出睡眠
    for(uint32_t i=0; i<100000; i++);
    ST7735_Write_Cmd(0x3A); // 像素格式：16位RGB565
    ST7735_Write_Data(0x05);
    ST7735_Write_Cmd(0x29); // 开启显示
}

void ST7735_Clear_Screen(uint16_t color)
{
    uint16_t i, j;
    // 设置全屏窗口
    ST7735_Write_Cmd(0x2A); // 列地址
    ST7735_Write_Data(0x00);
    ST7735_Write_Data(0x00);
    ST7735_Write_Data(0x00);
    ST7735_Write_Data(ST7735_WIDTH-1);
    ST7735_Write_Cmd(0x2B); // 行地址
    ST7735_Write_Data(0x00);
    ST7735_Write_Data(0x00);
    ST7735_Write_Data(0x00);
    ST7735_Write_Data(ST7735_HEIGHT-1);
    ST7735_Write_Cmd(0x2C); // 写显存
    
    // 填充颜色
    GPIO_ResetBits(GPIO_PORT, CS_PIN);
    GPIO_SetBits(GPIO_PORT, DC_PIN);
    for(i=0; i<ST7735_HEIGHT; i++)
    {
        for(j=0; j<ST7735_WIDTH; j++)
        {
            SPI_Send_Byte(color >> 8);   // 高8位
            SPI_Send_Byte(color & 0xFF); // 低8位
        }
    }
    GPIO_SetBits(GPIO_PORT, CS_PIN);
}

/************************* 主函数 *************************/
void setup(void)
{
    SystemInit(); // 系统时钟初始化（72MHz）
    ST7735_Init();// 初始化屏幕
    
    // 点亮屏幕：全屏红色
    ST7735_Clear_Screen(COLOR_RED);
    
    while(1)
    {
        // 无需额外操作，屏幕保持红色
    }
}

void loop()
{
}
#include "debug.h"
/*
 * OUT PP
 * R4 PD11
 * R3 PD9
 * R2 PE15
 * R1 PE13
 *
 * IPU
 * C1 PE11
 * C2 PE9
 * C3 PE7
 * C4 PC5
 */
void key_init() {
    // 使能各GPIO时钟（仅一次）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置行线为推挽输出（R4-R1）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_13;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    // 配置列线为上拉输入（C1-C4）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_9 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    Delay_Ms(10);  // 等待初始化稳定
}

u8 key_read()
{
    u8 temp=0;
    GPIO_ResetBits(GPIOD, GPIO_Pin_11); GPIO_SetBits(GPIOD, GPIO_Pin_9); GPIO_SetBits(GPIOE, GPIO_Pin_15); GPIO_SetBits(GPIOE, GPIO_Pin_13);
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=4;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=3;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=2;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=1;

    GPIO_SetBits(GPIOD, GPIO_Pin_11); GPIO_ResetBits(GPIOD, GPIO_Pin_9); GPIO_SetBits(GPIOE, GPIO_Pin_15); GPIO_SetBits(GPIOE, GPIO_Pin_13);
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=8;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=7;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=6;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=5;

    GPIO_SetBits(GPIOD, GPIO_Pin_11); GPIO_SetBits(GPIOD, GPIO_Pin_9); GPIO_ResetBits(GPIOE, GPIO_Pin_15); GPIO_SetBits(GPIOE, GPIO_Pin_13);
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=12;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=11;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=10;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=9;

    GPIO_SetBits(GPIOD, GPIO_Pin_11); GPIO_SetBits(GPIOD, GPIO_Pin_9); GPIO_SetBits(GPIOE, GPIO_Pin_15); GPIO_ResetBits(GPIOE, GPIO_Pin_13);
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_11)==0)temp=16;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_9)==0)temp=15;
    if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_7)==0)temp=14;
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)==0)temp=13;

    return temp;
}



